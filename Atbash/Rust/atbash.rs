#[derive(Default)]
pub(crate) struct Atbash {
    pub input: String
}

impl Atbash {
    pub fn new(input: String) -> Self {
        Self { input }
    }

    pub(crate) fn encode(&self) -> String {
        let mut output: String = String::new();

        for c in self.input.chars() {
            if c.is_lowercase() {
                let encoded = 'z' as usize - (c as usize - 'a' as usize);
                output.push(encoded as u8 as char);
            }
            else if c.is_uppercase() {
                let encoded = 'Z' as usize - (c as usize - 'A' as usize);
                output.push(encoded as u8 as char);
            }
            else if c.is_whitespace() || c.is_ascii_whitespace() || c.is_numeric() {
                output.push(c);
            }
            else {
                panic!("Error! Unknown symbol: {}", c);
            }
        }

        output
    }

    pub(crate) fn decode(&self, encoded_version: &String) -> String {
        let mut output = String::new();

        for c in encoded_version.chars() {
            if c.is_lowercase() {
                let encoded = 'z' as usize - (c as usize - 'a' as usize);
                output.push(encoded as u8 as char);
            }
            else if c.is_uppercase() {
                let encoded = 'Z' as usize - (c as usize - 'A' as usize);
                output.push(encoded as u8 as char);
            }
            else if c.is_whitespace() || c.is_ascii_whitespace() || c.is_numeric() {
                output.push(c);
            }
            else {
                panic!("Error! Unknown symbol: {}", c);
            }
        }

        output
    }
}
