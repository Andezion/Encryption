#[cfg(test)]
mod tests {
    use super::scytale::Scytale;
  
    #[test]
    fn encode_decode_basic() 
    {
        let scytale = Scytale::new("helloworld".to_string(), 4, false);
        let encoded = scytale.encode();
        let decoded = scytale.decode(&encoded, 4, false);
        assert_eq!(decoded, "helloworld");
    }

    #[test]
    fn encode_decode_reverse() 
    {
        let scytale = Scytale::new("rustlang".to_string(), 3, true);
        let encoded = scytale.encode();
        let decoded = scytale.decode(&encoded, 3, true);
        assert_eq!(decoded, "rustlang");
    }

    #[test]
    fn padding_added() 
    {
        let scytale = Scytale::new("abc".to_string(), 5, false);
        let encoded = scytale.encode();
        assert_eq!(encoded.len() % 5, 0);
        let decoded = scytale.decode(&encoded, 5, false);
        assert_eq!(decoded, "abc");
    }

    #[test]
    fn empty_string() 
    {
        let scytale = Scytale::new("".to_string(), 3, false);
        let encoded = scytale.encode();
        let decoded = scytale.decode(&encoded, 3, false);
        assert_eq!(decoded, "");
    }

    #[test]
    fn case_insensitive() 
    {
        let scytale = Scytale::new("HeLLo".to_string(), 2, false);
        let encoded = scytale.encode();
        let decoded = scytale.decode(&encoded, 2, false);
        assert_eq!(decoded, "hello");
    }

    #[test]
    fn no_padding_needed() 
    {
        let scytale = Scytale::new("abcd".to_string(), 2, false);
        let encoded = scytale.encode();
        let decoded = scytale.decode(&encoded, 2, false);
        assert_eq!(decoded, "abcd");
    }

    #[test]
    fn many_columns() 
    {
        let scytale = Scytale::new("encryption".to_string(), 10, false);
        let encoded = scytale.encode();
        let decoded = scytale.decode(&encoded, 10, false);
        assert_eq!(decoded, "encryption");
    }
}
