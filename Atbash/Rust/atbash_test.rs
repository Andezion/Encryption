#[cfg(test)]
mod tests {
    use crate::atbash::Atbash;

    #[test]
    fn test_encode_lowercase() {
        let atbash = Atbash::new("abcxyz".to_string());
        assert_eq!(atbash.encode(), "zyxcba");
    }

    #[test]
    fn test_encode_uppercase() {
        let atbash = Atbash::new("ABCXYZ".to_string());
        assert_eq!(atbash.encode(), "ZYXCBA");
    }

    #[test]
    fn test_encode_mixed_case() {
        let atbash = Atbash::new("AbCxYz".to_string());
        assert_eq!(atbash.encode(), "ZyXcBa");
    }

    #[test]
    fn test_encode_with_spaces_and_digits() {
        let atbash = Atbash::new("Hello World 123".to_string());
        assert_eq!(atbash.encode(), "Svool Dliow 123");
    }

    #[test]
    fn test_decode_basic() {
        let atbash = Atbash::default(); 
        let encoded = "zyxcba".to_string();
        assert_eq!(atbash.decode(&encoded), "abcxyz");
    }

    #[test]
    fn test_decode_with_spaces_and_digits() {
        let atbash = Atbash::default();
        let encoded = "Svool Dliow 123".to_string();
        assert_eq!(atbash.decode(&encoded), "Hello World 123");
    }

    #[test]
    #[should_panic(expected = "Error! Unknown symbol: @")]
    fn test_decode_with_invalid_symbol() {
        let atbash = Atbash::default();
        let encoded = "Svo@l".to_string();
        atbash.decode(&encoded);
    }

    #[test]
    fn test_empty_string() {
        let atbash = Atbash::new("".to_string());
        assert_eq!(atbash.encode(), "");

        let decoded = atbash.decode(&"".to_string());
        assert_eq!(decoded, "");
    }
}
