#[cfg(test)]
mod tests {
    use crate::polybius_method1::Polybius;

    #[test]
    fn test_basic_encode() {
        let p = Polybius::new("ABCDE".to_string());
        let encoded = p.encode();
        assert_eq!(encoded, "FGHIK");
    }

    #[test]
    fn test_basic_decode() {
        let p = Polybius::new("FGHIK".to_string());
        let decoded = p.decode(&"FGHIK".to_string());
        assert_eq!(decoded, "ABCDE");
    }

    #[test]
    fn test_wraparound_encode_bottom_row() {
        let p = Polybius::new("VWXYZ".to_string());
        let encoded = p.encode();
        assert_eq!(encoded, "ABCDE");
    }

    #[test]
    fn test_wraparound_decode_top_row() {
        let p = Polybius::new("ABCDE".to_string());
        let decoded = p.decode(&"ABCDE".to_string());
        assert_eq!(decoded, "VWXYZ");
    }

    #[test]
    fn test_ignore_non_alphabetic() {
        let p = Polybius::new("A1B!C".to_string());
        let encoded = p.encode();
        assert_eq!(encoded, "FGH");
    }

    #[test]
    fn test_letter_j_skipped_in_table() {
        let p = Polybius::new("IJ".to_string());
        let encoded = p.encode();
        assert_eq!(encoded, "O");
    }
}
