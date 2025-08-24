#[cfg(test)]
mod tests {
    use crate::affine::Affine;

    #[test]
    fn test_encode_decode_basic() {
        let affine = Affine::new(5, 8, "hello".to_string());
        let encoded = affine.encode();
        let decoded = affine.decode(&encoded);
        assert_eq!(decoded, "hello");
    }

    #[test]
    fn test_preserves_case() {
        let affine = Affine::new(5, 8, "HelloWorld".to_string());
        let encoded = affine.encode();
        let decoded = affine.decode(&encoded);
        assert_eq!(decoded, "HelloWorld");
    }

    #[test]
    fn test_ignores_spaces_and_punctuation() {
        let affine = Affine::new(7, 3, "attack at dawn!".to_string());
        let encoded = affine.encode();
        let decoded = affine.decode(&encoded);
        assert_eq!(decoded, "attack at dawn!");
    }

    #[test]
    #[should_panic(expected = "Error! 'a' must be coprime with 26")]
    fn test_invalid_a_panics() {
        // gcd(13, 26) != 1
        let _ = Affine::new(13, 5, "test".to_string());
    }

    #[test]
    #[should_panic(expected = "Error! Unknown symbol")]
    fn test_invalid_symbol_panics() {
        let affine = Affine::new(5, 8, "héllo".to_string());
        let _ = affine.encode();
    }

    #[test]
    fn test_different_keys() {
        let affine1 = Affine::new(7, 2, "rust".to_string());
        let encoded1 = affine1.encode();
        let decoded1 = affine1.decode(&encoded1);
        assert_eq!(decoded1, "rust");

        let affine2 = Affine::new(11, 15, "AffineCipher".to_string());
        let encoded2 = affine2.encode();
        let decoded2 = affine2.decode(&encoded2);
        assert_eq!(decoded2, "AffineCipher");
    }
}
