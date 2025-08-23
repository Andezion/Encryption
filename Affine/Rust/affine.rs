#[derive(Default)]
pub(crate) struct Affine {
    a: usize,
    b: usize,
    pub input: String
}

impl Affine {
    pub fn new(a: usize, b: usize, input: String) -> Self {
        if Self::gcd(a, 26) != 1 {
            panic!("Error! 'a' must be coprime with 26");
        }

        Self { a, b, input }
    }

    fn gcd(a: usize, b: usize) -> usize {
        if b == 0 {
            return a;
        }
        Self::gcd(b, a % b)
    }

    pub(crate) fn encode(&self) -> String {
        let mut output = String::new();

        for c in self.input.chars() {
            if c.is_ascii_lowercase() {
                let encoded = ((c as usize - 'a' as usize) * self.a + self.b) % 26 + 'a' as usize;
                output.push(encoded as u8 as char);
            } else if c.is_ascii_uppercase() {
                let encoded = ((c as usize - 'A' as usize) * self.a + self.b) % 26 + 'A' as usize;
                output.push(encoded as u8 as char);
            } else if c.is_whitespace() || c.is_ascii_punctuation() {
                output.push(c);
            } else {
                panic!("Error! Unknown symbol: {}", c);
            }
        }

        output
    }

    fn mod_inverse(mut a: i32, m: i32) -> i32 {
        a = a % m;

        for i in 1..m {
            if a * i % m == 1 {
                return i;
            }
        }
        panic!("Error! No modular inverse")
    }

    pub(crate) fn decode(&self, encoded_version: &String) -> String {
        let mut output = String::new();
        let a_inv = Self::mod_inverse(self.a as i32, 26);

        for c in encoded_version.chars() {
            if c.is_ascii_lowercase() {
                let decoded = (a_inv * (c as i32 - 'a' as i32 - self.b as i32 + 26) % 26) + 'a' as i32;
                output.push(decoded as u8 as char);
            } else if c.is_ascii_uppercase() {
                let decoded = (a_inv * (c as i32 - 'A' as i32 - self.b as i32 + 26) % 26) + 'A' as i32;
                output.push(decoded as u8 as char);
            } else if c.is_whitespace() || c.is_ascii_punctuation() {
                output.push(c);
            } else {
                panic!("Error! Unknown symbol: {}", c);
            }
        }

        output
    }
}


#[cfg(test)]
mod tests {
    use super::*;

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
