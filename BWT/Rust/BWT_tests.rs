#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_insert_and_get() {
        let tree = BwTree::new();
        assert!(tree.insert(1, "A"));
        assert_eq!(tree.get(1), Some(&"A"));
        assert!(tree.insert(2, "B"));
        assert_eq!(tree.get(2), Some(&"B"));
        assert!(tree.insert(3, "C"));
        assert_eq!(tree.get(3), Some(&"C"));
        assert!(tree.insert(4, "D"));
        assert_eq!(tree.get(4), Some(&"D"));
    }

    #[test]
    fn test_insert_retains_existing_entries() {
        let tree = BwTree::new();
        assert!(tree.insert(1, "A"));
        assert_eq!(tree.get(1), Some(&"A"));
        assert!(tree.insert(2, "B"));
        assert_eq!(tree.get(1), Some(&"A"));
        assert!(tree.insert(2, "B"));
    }
}
