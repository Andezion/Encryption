use std::fmt::Debug;
use std::sync::atomic::{AtomicPtr, AtomicUsize, Ordering};

const FIRST_LEAF_NODE_ID: usize = 2;
const MAPPING_TABLE_SIZE: usize = 1 << 20;

pub trait KeyType: Ord {
    const MINIMUM: Self;
}

impl KeyType for u64 {
    const MINIMUM: Self = u64::MIN;
}

pub type NodeID = usize;

#[derive(Default)]
pub struct BwTree<K, V>
where
    K: KeyType + Debug,
    V: Clone + Debug,
{
    root_id: usize,
    mapping_table: MappingTable<K, V>,
    next_unused_node_id: AtomicUsize,
}

impl<K, V> BwTree<K, V>
where
    K: KeyType + Debug,
    V: Clone + Debug,
{
    pub fn new() -> Self {
        let ret: BwTree<K, V> = BwTree {
            root_id: 1,
            mapping_table: MappingTable::new(),
            next_unused_node_id: AtomicUsize::new(1),
        };

        let root_id = ret.get_next_node_id();
        assert_eq!(1, root_id);
        let first_leaf_id = ret.get_next_node_id();
        assert_eq!(FIRST_LEAF_NODE_ID, first_leaf_id);

        let left_most_leaf = Node::Leaf(LeafNode::new());
        let mut root = InnerNode::new();
        root.insert(KeyType::MINIMUM, first_leaf_id);

        ret.mapping_table.insert(root_id, Node::Inner(root));
        ret.mapping_table.insert(first_leaf_id, left_most_leaf);

        ret
    }

    fn get_next_node_id(&self) -> NodeID {
        self.next_unused_node_id.fetch_add(1, Ordering::SeqCst)
    }

    pub fn insert(&self, key: K, value: V) -> bool {
        let root = self.mapping_table.get(self.root_id);
        match root {
            Node::Inner(_) => {
                let delta = DeltaNode::new();
                delta.insert(key, value);
                let delta = Node::Delta(delta);
                self.mapping_table.insert(self.root_id, delta);
            }
            Node::Delta(delta) => {
                delta.insert(key, value);
            }
            Node::Leaf(_) => todo!(),
        }
        true
    }

    pub fn get(&self, key: K) -> Option<&V> {
        let root = self.mapping_table.get(self.root_id);
        root.get(&key)
    }
}

#[derive(Default)]
pub struct MappingTable<K: Ord, V: Clone> {
    entries: Vec<AtomicPtr<Node<K, V>>>,
}

impl<K: Ord, V: Clone> MappingTable<K, V> {
    pub fn new() -> Self {
        let mut entries = Vec::default();
        entries.resize_with(MAPPING_TABLE_SIZE, AtomicPtr::default);
        MappingTable { entries }
    }

    fn get(&self, id: usize) -> &Node<K, V> {
        assert!(id < MAPPING_TABLE_SIZE);
        let entry = self.entries[id].load(Ordering::Acquire);
        assert!(!entry.is_null());
        unsafe { &*entry }
    }

    fn insert(&self, id: usize, node: Node<K, V>) -> bool {
        assert!(id < MAPPING_TABLE_SIZE);
        let entry = &self.entries[id];
        let old = entry.load(Ordering::Acquire);
        let new = Box::leak(Box::new(node));
        match entry.compare_exchange(old, new, Ordering::SeqCst, Ordering::SeqCst) {
            Ok(_old) => {
                true
            }
            Err(new) => {
                std::mem::drop(Box::from(new));
                false
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
{
    fn get(&self, key: &K) -> Option<&V> {
        match self {
            Node::Inner(_) => todo!(),
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

impl<K> InnerNode<K> {
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
}

#[derive(Debug)]
struct DeltaNode<K, V> {
    records: LinkedList<DeltaRecord<K, V>>,
}

impl<K, V> DeltaNode<K, V>
where
    K: KeyType,
{
    fn new() -> Self {
        DeltaNode {
            records: LinkedList::new(),
        }
    }

    fn insert(&self, key: K, value: V) {
        self.records.push_front(DeltaRecord::Insert(key, value));
    }

    fn get(&self, key: &K) -> Option<&V> {
        for ref record in self.records.iter() {
            match record {
                DeltaRecord::Insert(k, v) => {
                    if key == k {
                        return Some(v);
                    }
                }
            }
        }
        None
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
{
    fn new() -> Self {
        LeafNode {
            count: 0,
            keys: Vec::new(),
            values: Vec::new(),
        }
    }

    fn get(&self, key: &K) -> Option<&V> {
        for i in 0..self.count {
            if key == &self.keys[i] {
                return Some(&self.values[i]);
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
                (*new_node).next.store(head, Ordering::Relaxed);
            }
            if self
                .head
                .compare_exchange_weak(head, new_node, Ordering::Release, Ordering::Relaxed)
                .is_ok()
            {
                break;
            }
        }
    }

    pub fn iter(&self) -> LinkedListIter<'_, T> {
        let next = unsafe { self.head.load(Ordering::Relaxed).as_ref() };
        LinkedListIter { next }
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
            let next = unsafe { node.next.load(Ordering::Relaxed).as_ref() };
            self.next = next;
            Some(&node.value)
        } else {
            None
        }
    }
}
