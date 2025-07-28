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
