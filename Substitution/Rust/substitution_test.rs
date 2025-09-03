#[cfg(test)]
mod tests {
    use super::substitution::Substitution;

    #[test]
    fn test_basic_encoding() {
        let sub = Substitution::new("abc".to_string(), "zyxwvutsrqponmlkjihgfedcba".to_string());
        assert_eq!(sub.encode(), "zyx");
    }

    #[test]
    fn test_basic_decoding() {
        let sub = Substitution::new("abc".to_string(), "zyxwvutsrqponmlkjihgfedcba".to_string());
        let encoded = sub.encode();
        let decoded = sub.decode(&encoded);
        assert_eq!(decoded, "abc");
    }

    #[test]
    fn test_mixed_case_and_symbols() {
        let sub = Substitution::new("Hello, World!".to_string(), "phqgiumeaylnofdxjkrcvstzwb".to_string());
        assert_eq!(sub.encode(), "Eainn, Vncfk!");
    }

    #[test]
    fn test_empty_string() {
        let sub = Substitution::new("".to_string(), "abcdefghijklmnopqrstuvwxyz".to_string());
        assert_eq!(sub.encode(), "");
        assert_eq!(sub.decode(&"".to_string()), "");
    }

    #[test]
    fn test_encode_decode_reversibility() {
        let input = "The quick brown fox jumps over 13 lazy dogs.";
        let key = "mnbvcxzlkjhgfdsapoiuytrewq";
        let sub = Substitution::new(input.to_string(), key.to_string());
        let encoded = sub.encode();
        let decoded = sub.decode(&encoded);
        assert_eq!(decoded, input);
    }

    #[test]
    fn test_key_shorter_than_26_letters() {
        let sub = Substitution::new("abc".to_string(), "xyz".to_string());
        
        assert_eq!(sub.encode(), "xyz");
        assert_eq!(sub.decode(&"xyz".to_string()), "abc");
    }

    #[test]
    fn test_key_with_repeated_characters() {
        let sub = Substitution::new("abcabc".to_string(), "aaaaaaaaaaaaaaaaaaaaaaaaaa".to_string());
        
        let encoded = sub.encode();
        assert_eq!(encoded, "aaaaaa");
        let decoded = sub.decode(&encoded);
        
        assert_eq!(decoded, "aaaaaa");
    }
}
