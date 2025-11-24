#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_insert_and_get() {
        let tree: BwTree<u64, &str> = BwTree::new();
        
        assert!(tree.insert(1, "A"));
        assert_eq!(tree.get(1), Some("A"));
        
        assert!(tree.insert(2, "B"));
        assert_eq!(tree.get(2), Some("B"));
        
        assert!(tree.insert(3, "C"));
        assert_eq!(tree.get(3), Some("C"));
        
        assert!(tree.insert(4, "D"));
        assert_eq!(tree.get(4), Some("D"));
    }

    #[test]
    fn test_insert_retains_existing_entries() {
        let tree: BwTree<u64, &str> = BwTree::new();
        
        assert!(tree.insert(1, "A"));
        assert_eq!(tree.get(1), Some("A"));
        
        assert!(tree.insert(2, "B"));
        assert_eq!(tree.get(1), Some("A"));
        assert_eq!(tree.get(2), Some("B"));
        
        assert!(tree.insert(3, "C"));
        assert_eq!(tree.get(1), Some("A"));
        assert_eq!(tree.get(2), Some("B"));
        assert_eq!(tree.get(3), Some("C"));
    }

    #[test]
    fn test_update_existing_key() {
        let tree: BwTree<u64, &str> = BwTree::new();
        
        assert!(tree.insert(1, "A"));
        assert_eq!(tree.get(1), Some("A"));
        
        assert!(tree.insert(1, "A_updated"));
        assert_eq!(tree.get(1), Some("A_updated"));
    }

    #[test]
    fn test_many_insertions() {
        let tree: BwTree<u64, u64> = BwTree::new();
        
        for i in 0..100 {
            assert!(tree.insert(i, i * 2));
        }
        
        for i in 0..100 {
            assert_eq!(tree.get(i), Some(i * 2));
        }
    }

    #[test]
    fn test_consolidation() {
        let tree: BwTree<u64, u64> = BwTree::new();
        
        for i in 0..10 {
            tree.insert(i, i);
        }
        
        for i in 0..10 {
            assert_eq!(tree.get(i), Some(i));
        }
    }
}
