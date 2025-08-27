#[cfg(test)]
mod tests {
    use crate::rot47::ROT47;

    #[test]
    fn test_basic_encode() {
        let rot = ROT47::new(String::from("Hello, World!"));
        assert_eq!(rot.encode(), "w6==@[ (@C=5P");
    }

    #[test]
    fn test_basic_decode() {
        let rot = ROT47::new(String::new());
        let encoded = String::from("w6==@ (@C=5P!");
        assert_eq!(rot.decode(&encoded), "Hello World!P");
    }

    #[test]
    fn test_empty_string() {
        let rot = ROT47::new(String::new());
        assert_eq!(rot.encode(), "");
        assert_eq!(rot.decode(&String::new()), "");
    }

    #[test]
    fn test_whitespace_and_punctuation() {
        let rot = ROT47::new(String::from(" !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"));
        assert_eq!(rot.encode(), " PQRSTUVWXYZ[\\]^ijklmno,-./01LMNO");
    }

    #[test]
    fn test_encode_decode_cycle() {
        let input = String::from("RustLang2025!");
        let rot = ROT47::new(input.clone());
        let encoded = rot.encode();
        let decoded = rot.decode(&encoded);
        assert_eq!(decoded, input);
    }

    #[test]
    fn test_numbers_are_encoded() {
        let rot = ROT47::new("12345".to_string());
        assert_eq!(rot.encode(), "`abcd"); 
    }
}
