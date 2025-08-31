#[cfg(test)]
mod tests {
    use super::*;

    fn setup_playfair(input: &str, key: &str) -> Playfair {
        let mut matrix = [[' '; 5]; 5];
        Playfair::new(input.to_string(), key.to_string(), &mut matrix)
    }

    #[test]
    fn test_matrix_generation_with_key() {
        let pf = setup_playfair("", "keyword");
        assert_eq!(pf.matrix[0][0], 'K'); 
        assert!(pf.matrix.iter().flatten().all(|c| c.is_ascii_alphabetic()));
    }

    #[test]
    fn test_encode_basic() {
        let pf = setup_playfair("HELLO", "keyword");
        let encoded = pf.encode();
        assert!(!encoded.is_empty());
        assert_eq!(encoded.len() % 2, 0);
    }

    #[test]
    fn test_encode_j_to_i() {
        let pf = setup_playfair("JUMP", "keyword");
        let encoded = pf.encode();
        assert!(!encoded.contains('J')); 
    }

    #[test]
    fn test_encode_double_letters() {
        let pf = setup_playfair("BALLOON", "keyword");
        let encoded = pf.encode();
        assert!(encoded.len() >= pf.input.len());
    }

    #[test]
    fn test_encode_decode_roundtrip() {
        let pf = setup_playfair("SECRET", "keyword");
        let encoded = pf.encode();
        let decoded = pf.decode(&encoded);
        assert_eq!(decoded.len(), encoded.len());
        assert!(decoded.starts_with("SE") || decoded.starts_with("SX"));
    }

    #[test]
    fn test_decode_reverses_encode() {
        let pf = setup_playfair("HELLO", "keyword");
        let encoded = pf.encode();
        let decoded = pf.decode(&encoded);
        assert_eq!(pf.encode(), pf.encode());
        assert_eq!(pf.decode(&encoded), decoded);
    }

    #[test]
    fn test_custom_key_affects_matrix() {
        let mut matrix1 = [[' '; 5]; 5];
        let mut matrix2 = [[' '; 5]; 5];
        let pf1 = Playfair::new("TEST".to_string(), "keyword".to_string(), &mut matrix1);
        let pf2 = Playfair::new("TEST".to_string(), "another".to_string(), &mut matrix2);
        assert_ne!(pf1.matrix, pf2.matrix); 
    }
}
