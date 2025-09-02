#[cfg(test)]
mod tests {
    use crate::rot13::ROT13;

    #[test]
    fn test_encode_lowercase() {
        let rot = ROT13::new("hello".to_string());
        assert_eq!(rot.encode(), "uryyb");
    }

    #[test]
    fn test_encode_uppercase() {
        let rot = ROT13::new("WORLD".to_string());
        assert_eq!(rot.encode(), "JBEYQ");
    }

    #[test]
    fn test_encode_mixed() {
        let rot = ROT13::new("Hello, World!".to_string());
        assert_eq!(rot.encode(), "Uryyb, Jbeyq!");
    }

    #[test]
    fn test_decode_lowercase() {
        let rot = ROT13::new("hello".to_string());
        let encoded = rot.encode();
        assert_eq!(rot.decode(&encoded), "hello");
    }

    #[test]
    fn test_decode_uppercase() {
        let rot = ROT13::new("WORLD".to_string());
        let encoded = rot.encode();
        assert_eq!(rot.decode(&encoded), "WORLD");
    }

    #[test]
    fn test_empty_string() {
        let rot = ROT13::new("".to_string());
        let encoded = rot.encode();
        assert_eq!(encoded, "");
        assert_eq!(rot.decode(&encoded), "");
    }

    #[test]
    #[should_panic(expected = "Error! Unknown symbol")]
    fn test_invalid_character() {
        let rot = ROT13::new("hello🙂".to_string());
        rot.encode();
    }
}
