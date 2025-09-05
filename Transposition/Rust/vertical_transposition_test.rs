#[cfg(test)]
mod tests {
    use super::vertical_transposition::VerticalTransposition;

    #[test]
    fn test_encode_basic() {
        let vt = VerticalTransposition::new("HELLOWORLD".to_string(), vec![0, 1, 2, 3, 4]);
        let encoded = vt.encode();
        assert!(!encoded.is_empty(), "Encoded string should not be empty");
    }

    #[test]
    fn test_encode_with_non_alpha() {
        let vt = VerticalTransposition::new("HELLO WORLD!!!".to_string(), vec![0, 1, 2, 3, 4]);
        let encoded = vt.encode();
        assert!(encoded.chars().all(|c| c.is_ascii_uppercase()), "All letters must be uppercase alphabetic");
    }

    #[test]
    fn test_encode_length_multiple_of_five() {
        let vt = VerticalTransposition::new("ABCDEFGHIJ".to_string(), vec![0, 1, 2, 3, 4]);
        let encoded = vt.encode();
        assert_eq!(encoded.len() % 5, 0, "Encoded length must be multiple of 5");
    }

    #[test]
    fn test_encode_with_indexes_permutation() {
        let vt = VerticalTransposition::new("ABCDEFGHIJ".to_string(), vec![4, 3, 2, 1, 0]);
        let encoded = vt.encode();
        assert!(!encoded.is_empty());
        assert_ne!(encoded, "ABCDEFGHIJ");
    }

    #[test]
    fn test_decode_should_fail_on_invalid_data() {
        let vt = VerticalTransposition::new("TESTDATA".to_string(), vec![0, 1, 2, 3, 4]);
        let result = std::panic::catch_unwind(|| {
            vt.decode(&"SHORT".to_string());
        });
        assert!(result.is_err(), "Decode should panic on invalid input");
    }

    #[test]
    fn test_encode_decode_roundtrip() {
        let vt = VerticalTransposition::new("HELLOWORLDABCDE".to_string(), vec![0, 1, 2, 3, 4]);
        let encoded = vt.encode();
        let decoded = vt.decode(&encoded);
        assert_eq!(decoded.len(), 15);
    }

    #[test]
    fn test_empty_input() {
        let vt = VerticalTransposition::new("".to_string(), vec![0, 1, 2, 3, 4]);
        let encoded = vt.encode();
        assert!(encoded.is_empty());
    }
}
