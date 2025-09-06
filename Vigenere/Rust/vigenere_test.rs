#[cfg(test)]
mod tests {
    use super::vigenere::Vigenere;

    fn make_table() -> [[char; 26]; 26] {
        [['a'; 26]; 26]
    }

    #[test]
    fn encode_simple() {
        let mut table = make_table();
        let v = Vigenere::new("attackatdawn".to_string(), "lemon".to_string(), &mut table);
        let encoded = v.encode();
        assert_eq!(encoded, "lxfopvefrnhr");
    }

    #[test]
    fn encode_mixed_case() {
        let mut table = make_table();
        let v = Vigenere::new("AttackAtDawn".to_string(), "LEMON".to_string(), &mut table);
        let encoded = v.encode();
        assert_eq!(encoded, "lxfopvefrnhr");
    }

    #[test]
    fn encode_with_spaces_and_symbols() {
        let mut table = make_table();
        let v = Vigenere::new("attack at dawn!".to_string(), "lemon".to_string(), &mut table);
        let encoded = v.encode();
        assert_eq!(encoded, "lxfopv efrnhr!");
    }

    #[test]
    fn encode_single_char_key() {
        let mut table = make_table();
        let v = Vigenere::new("abcxyz".to_string(), "b".to_string(), &mut table);
        let encoded = v.encode();
        assert_eq!(encoded, "bcdyza");
    }

    #[test]
    fn encode_long_key() {
        let mut table = make_table();
        let v = Vigenere::new("dog".to_string(), "superlongkey".to_string(), &mut table);
        let encoded = v.encode();
        assert_eq!(encoded, "vcl");
    }

    #[test]
    fn encode_empty_input() {
        let mut table = make_table();
        let v = Vigenere::new("".to_string(), "key".to_string(), &mut table);
        assert_eq!(v.encode(), "");
    }

    #[test]
    fn encode_uppercase_key() {
        let mut table = make_table();
        let v = Vigenere::new("hello".to_string(), "Z".to_string(), &mut table);
        let encoded = v.encode();
        assert_eq!(encoded, "gdkkn");
    }
}
