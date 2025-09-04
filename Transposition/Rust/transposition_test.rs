#[cfg(test)]
mod tests {
    use super::transposition::Transposition;

    #[test]
    fn test_new_creates_instance_with_input() {
        let t = Transposition::new("hello".to_string());
        assert_eq!(t.input, "hello");
    }

    #[test]
    fn test_encode_basic_input() {
        let t = Transposition::new("hello world".to_string());
        let encoded = t.encode();
        assert!(!encoded.is_empty());
      
    }

    #[test]
    fn test_encode_removes_non_alphabetic() {
        let t = Transposition::new("a!b@c#d$e%f^".to_string());
        let encoded = t.encode();
        assert!(!encoded.contains('!'));
        assert!(!encoded.contains('@'));
        assert!(encoded.chars().all(|c| c.is_alphabetic()));
    }

    #[test]
    fn test_encode_uppercase_output() {
        let t = Transposition::new("abcde".to_string());
        let encoded = t.encode();
        assert!(encoded.chars().all(|c| c.is_uppercase()));
    }

    #[test]
    fn test_encode_empty_string() {
        let t = Transposition::new("".to_string());
        let encoded = t.encode();
        assert_eq!(encoded, "");
    }

    #[test]
    fn test_decode_reverses_encoding() {
        let input = "thequickbrownfox";
        let t = Transposition::new(input.to_string());
        let encoded = t.encode();
        let decoded = t.decode(&encoded);
        for c in input.chars().filter(|c| c.is_alphabetic()) {
            assert!(decoded.contains(c.to_ascii_uppercase()));
        }
    }

    #[test]
    fn test_encode_non_multiple_of_5_characters() {
        let t = Transposition::new("abcdefg".to_string());
        let encoded = t.encode();
        assert!(encoded.len() >= 7);
    }
}
