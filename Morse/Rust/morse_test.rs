#[cfg(test)]
mod tests {
    use crate::mouse::Morse;

    #[test]
    fn test_encode_simple_word() {
        let morse = Morse::new("SOS".to_string());
        assert_eq!(morse.encode(), "... ___ ...");
    }

    #[test]
    fn test_decode_simple_word() {
        let morse = Morse::new("".to_string());
        assert_eq!(morse.decode("... ___ ..."), "SOS");
    }

    #[test]
    fn test_case_insensitivity() {
        let morse_upper = Morse::new("HELLO".to_string());
        let morse_lower = Morse::new("hello".to_string());
        assert_eq!(morse_upper.encode(), morse_lower.encode());
    }

    #[test]
    fn test_phrase_with_space() {
        let morse = Morse::new("HELLO WORLD".to_string());
        assert_eq!(morse.encode(), ".... . ._.. ._.. ___ / .__ ___ ._. ._.. _..");
    }

    #[test]
    fn test_numbers() {
        let morse = Morse::new("123".to_string());
        assert_eq!(morse.encode(), ".____ ..___ ...__");
        assert_eq!(morse.decode(".____ ..___ ...__"), "123");
    }

    #[test]
    fn test_punctuation() {
        let morse = Morse::new("HELLO!".to_string());
        assert!(morse.encode().ends_with("_._.__"));
        assert_eq!(morse.decode(".... . ._.. ._.. ___ _._.__"), "HELLO!");
    }

    #[test]
    fn test_round_trip() {
        let text = "TEST 123!";
        let morse = Morse::new(text.to_string());
        let encoded = morse.encode();
        let decoded = morse.decode(&encoded);
        assert_eq!(decoded, text);
    }
}
