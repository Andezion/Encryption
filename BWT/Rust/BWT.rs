
use std::fmt::Debug;
use std::sync::atomic::{AtomicPtr, AtomicUsize, Ordering};

const FIRST_LEAF_NODE_ID: usize = 2;
const MAPPING_TABLE_SIZE: usize = 1 << 20;
const CONSOLIDATE_THRESHOLD: usize = 5;

pub trait KeyType: Ord + Copy + Debug {
    const MINIMUM: Self;
}

impl KeyType for u64 {
    const MINIMUM: Self = u64::MIN;
}

impl KeyType for i32 {
    const MINIMUM: Self = i32::MIN;
}

pub type NodeID = usize;

pub struct BwTree<K, V>
where
    K: KeyType,
    V: Clone + Debug,
{
    root_id: NodeID,
    mapping_table: MappingTable<K, V>,
    next_unused_node_id: AtomicUsize,
}

impl<K, V> BwTree<K, V>
where
    K: KeyType,
    V: Clone + Debug,
{
    pub fn new() -> Self {
        let mapping_table = MappingTable::new();
        let next_unused_node_id = AtomicUsize::new(1);

        let root_id = next_unused_node_id.fetch_add(1, Ordering::SeqCst);
        let first_leaf_id = next_unused_node_id.fetch_add(1, Ordering::SeqCst);

        assert_eq!(1, root_id);
        assert_eq!(FIRST_LEAF_NODE_ID, first_leaf_id);

        let left_most_leaf = Node::Leaf(LeafNode::new());
        let mut root = InnerNode::new();
        root.insert(K::MINIMUM, first_leaf_id);

        mapping_table.cas_insert(root_id, std::ptr::null_mut(), Node::Inner(root));
        mapping_table.cas_insert(first_leaf_id, std::ptr::null_mut(), left_most_leaf);

        BwTree {
            root_id,
            mapping_table,
            next_unused_node_id,
        }
    }

    fn get_next_node_id(&self) -> NodeID {
        self.next_unused_node_id.fetch_add(1, Ordering::SeqCst)
    }

    pub fn insert(&self, key: K, value: V) -> bool {
        loop {
            let leaf_id = self.find_leaf(key);
            
            if self.install_insert_delta(leaf_id, key, value.clone()) {
                return true;
            }
        }
    }

    fn find_leaf(&self, key: K) -> NodeID {
        let mut current_id = self.root_id;

        loop {
            let node = self.mapping_table.get(current_id);
            
            match node {
                Node::Inner(inner) => {
                    current_id = inner.find_child(key);
                }
                Node::Leaf(_) => {
                    return current_id;
                }
                Node::Delta(delta) => {
                    if let Some(base_id) = delta.base_node_id {
                        current_id = base_id;
                    } else {
                        return current_id;
                    }
                }
            }
        }
    }

    fn install_insert_delta(&self, node_id: NodeID, key: K, value: V) -> bool {
        let old_ptr = self.mapping_table.get_ptr(node_id);
        
        let mut delta = DeltaNode::new();
        delta.insert(key, value);
        delta.base_node_id = None;
        
        let new_node = Node::Delta(delta);
        
        if self.mapping_table.cas_insert(node_id, old_ptr, new_node) {
            self.maybe_consolidate(node_id);
            true
        } else {
            false
        }
    }

    fn maybe_consolidate(&self, node_id: NodeID) {
        let node = self.mapping_table.get(node_id);
        
        if let Node::Delta(delta) = node {
            if delta.chain_length() >= CONSOLIDATE_THRESHOLD {
                self.consolidate(node_id);
            }
        }
    }

    fn consolidate(&self, node_id: NodeID) {
        let old_ptr = self.mapping_table.get_ptr(node_id);
        let node = unsafe { &*old_ptr };
        
        let consolidated = self.consolidate_node(node);
        
        self.mapping_table.cas_insert(node_id, old_ptr, consolidated);
    }

    fn consolidate_node(&self, node: &Node<K, V>) -> Node<K, V> {
        let mut keys = Vec::new();
        let mut values = Vec::new();
        
        self.collect_records(node, &mut keys, &mut values);
        
        Node::Leaf(LeafNode {
            count: keys.len(),
            keys,
            values,
        })
    }

    fn collect_records(&self, node: &Node<K, V>, keys: &mut Vec<K>, values: &mut Vec<V>) {
        match node {
            Node::Delta(delta) => {
                for record in delta.records.iter() {
                    match record {
                        DeltaRecord::Insert(k, v) => {
                            if !keys.contains(k) {
                                keys.push(*k);
                                values.push(v.clone());
                            }
                        }
                    }
                }
            }
            Node::Leaf(leaf) => {
                for i in 0..leaf.count {
                    if !keys.contains(&leaf.keys[i]) {
                        keys.push(leaf.keys[i]);
                        values.push(leaf.values[i].clone());
                    }
                }
            }
            Node::Inner(_) => {}
        }
    }

    pub fn get(&self, key: K) -> Option<V> {
        let leaf_id = self.find_leaf(key);
        let node = self.mapping_table.get(leaf_id);
        node.get(&key)
    }
}

pub struct MappingTable<K: Ord, V: Clone> {
    entries: Vec<AtomicPtr<Node<K, V>>>,
}

impl<K: Ord, V: Clone> MappingTable<K, V> {
    pub fn new() -> Self {
        let mut entries = Vec::with_capacity(MAPPING_TABLE_SIZE);
        for _ in 0..MAPPING_TABLE_SIZE {
            entries.push(AtomicPtr::new(std::ptr::null_mut()));
        }
        MappingTable { entries }
    }

    fn get(&self, id: usize) -> &Node<K, V> {
        assert!(id < MAPPING_TABLE_SIZE);
        let ptr = self.entries[id].load(Ordering::Acquire);
        assert!(!ptr.is_null(), "Accessed null node at id {}", id);
        unsafe { &*ptr }
    }

    fn get_ptr(&self, id: usize) -> *mut Node<K, V> {
        assert!(id < MAPPING_TABLE_SIZE);
        self.entries[id].load(Ordering::Acquire)
    }

    fn cas_insert(&self, id: usize, expected: *mut Node<K, V>, node: Node<K, V>) -> bool {
        assert!(id < MAPPING_TABLE_SIZE);
        let entry = &self.entries[id];
        let new = Box::into_raw(Box::new(node));
        
        match entry.compare_exchange(expected, new, Ordering::SeqCst, Ordering::SeqCst) {
            Ok(_old) => {
                true
            }
            Err(_) => {
                unsafe { drop(Box::from_raw(new)); }
                false
            }
        }
    }
}

impl<K: Ord, V: Clone> Drop for MappingTable<K, V> {
    fn drop(&mut self) {
        for entry in &self.entries {
            let ptr = entry.load(Ordering::Relaxed);
            if !ptr.is_null() {
                unsafe {
                    drop(Box::from_raw(ptr));
                }
            }
        }
    }
}

#[derive(Debug)]
enum Node<K, V> {
    Inner(InnerNode<K>),
    Delta(DeltaNode<K, V>),
    Leaf(LeafNode<K, V>),
}

impl<K, V> Node<K, V>
where
    K: KeyType,
    V: Clone,
{
    fn get(&self, key: &K) -> Option<V> {
        match self {
            Node::Inner(_) => None,
            Node::Delta(node) => node.get(key),
            Node::Leaf(node) => node.get(key),
        }
    }
}

#[derive(Debug)]
struct InnerNode<K> {
    keys: Vec<K>,
    children: Vec<NodeID>,
}

impl<K: Ord + Copy> InnerNode<K> {
    fn new() -> Self {
        InnerNode {
            keys: Vec::new(),
            children: Vec::new(),
        }
    }

    fn insert(&mut self, key: K, node_id: NodeID) {
        self.keys.push(key);
        self.children.push(node_id);
    }

    fn find_child(&self, key: K) -> NodeID {
        for i in 0..self.keys.len() {
            if i + 1 >= self.keys.len() || key < self.keys[i + 1] {
                return self.children[i];
            }
        }
        self.children[self.children.len() - 1]
    }
}

#[derive(Debug)]
struct DeltaNode<K, V> {
    records: LinkedList<DeltaRecord<K, V>>,
    base_node_id: Option<NodeID>,
}

impl<K, V> DeltaNode<K, V>
where
    K: KeyType,
    V: Clone,
{
    fn new() -> Self {
        DeltaNode {
            records: LinkedList::new(),
            base_node_id: None,
        }
    }

    fn insert(&mut self, key: K, value: V) {
        self.records.push_front(DeltaRecord::Insert(key, value));
    }

    fn get(&self, key: &K) -> Option<V> {
        for record in self.records.iter() {
            match record {
                DeltaRecord::Insert(k, v) => {
                    if key == k {
                        return Some(v.clone());
                    }
                }
            }
        }
        None
    }

    fn chain_length(&self) -> usize {
        self.records.len()
    }
}

#[derive(Debug)]
enum DeltaRecord<K, V> {
    Insert(K, V),
}

#[derive(Debug)]
struct LeafNode<K, V> {
    count: usize,
    keys: Vec<K>,
    values: Vec<V>,
}

impl<K, V> LeafNode<K, V>
where
    K: KeyType,
    V: Clone,
{
    fn new() -> Self {
        LeafNode {
            count: 0,
            keys: Vec::new(),
            values: Vec::new(),
        }
    }

    fn get(&self, key: &K) -> Option<V> {
        for i in 0..self.count {
            if key == &self.keys[i] {
                return Some(self.values[i].clone());
            }
        }
        None
    }
}

#[derive(Debug)]
pub struct LinkedList<T> {
    head: AtomicPtr<LinkedListNode<T>>,
}

impl<T> LinkedList<T> {
    pub fn new() -> Self {
        LinkedList {
            head: AtomicPtr::new(std::ptr::null_mut()),
        }
    }

    pub fn push_front(&self, value: T) {
        let new_node = Box::into_raw(Box::new(LinkedListNode {
            value,
            next: AtomicPtr::new(std::ptr::null_mut()),
        }));
        
        loop {
            let head = self.head.load(Ordering::Acquire);
            unsafe {
                (*new_node).next.store(head, Ordering::Release);
            }
            
            if self
                .head
                .compare_exchange_weak(head, new_node, Ordering::Release, Ordering::Acquire)
                .is_ok()
            {
                break;
            }
        }
    }

    pub fn iter(&self) -> LinkedListIter<'_, T> {
        let next = unsafe { self.head.load(Ordering::Acquire).as_ref() };
        LinkedListIter { next }
    }

    pub fn len(&self) -> usize {
        let mut count = 0;
        let mut current = self.head.load(Ordering::Acquire);
        
        while !current.is_null() {
            count += 1;
            current = unsafe { (*current).next.load(Ordering::Acquire) };
        }
        
        count
    }
}

impl<T> Drop for LinkedList<T> {
    fn drop(&mut self) {
        let mut current = self.head.load(Ordering::Relaxed);
        
        while !current.is_null() {
            unsafe {
                let node = Box::from_raw(current);
                current = node.next.load(Ordering::Relaxed);
            }
        }
    }
}

#[derive(Debug)]
struct LinkedListNode<T> {
    value: T,
    next: AtomicPtr<LinkedListNode<T>>,
}

pub struct LinkedListIter<'a, T> {
    next: Option<&'a LinkedListNode<T>>,
}

impl<'a, T> Iterator for LinkedListIter<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        if let Some(node) = self.next {
            let next = unsafe { node.next.load(Ordering::Acquire).as_ref() };
            self.next = next;
            Some(&node.value)
        } else {
            None
        }
    }
}
