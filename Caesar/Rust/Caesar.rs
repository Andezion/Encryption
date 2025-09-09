#[derive(Default)]
pub(crate) struct Caesar {
    pub input: String
}

impl Caesar {
    pub fn new(input: String) -> Self {
        Self { input }
    }

    pub(crate) fn encode(&self) -> String {
        let mut output: String = String::new();

        for c in self.input.chars() {
            if c.is_ascii_lowercase() {
                let encoded = ((c as usize - 'a' as usize) + 13) % 26 + 'a' as usize;
                output.push(encoded as u8 as char);
            } else if c.is_ascii_uppercase() {
                let encoded = ((c as usize - 'A' as usize) + 13) % 26 + 'A' as usize;
                output.push(encoded as u8 as char);
            } else if c.is_whitespace() || c.is_ascii_punctuation() {
                output.push(c);
            } else {
                panic!("Error! Unknown symbol: {}", c);
            }
        }

        output
    }

    pub(crate) fn decode(&self, encoded_version: &String) -> String {
        let mut output = String::new();

        for c in encoded_version.chars() {
            if c.is_ascii_lowercase() {
                let encoded = ((c as usize - 'a' as usize) + 13) % 26 + 'a' as usize;
                output.push(encoded as u8 as char);
            } else if c.is_ascii_uppercase() {
                let encoded = ((c as usize - 'A' as usize) + 13) % 26 + 'A' as usize;
                output.push(encoded as u8 as char);
            } else if c.is_whitespace() || c.is_ascii_punctuation() {
                output.push(c);
            } else {
                panic!("Error! Unknown symbol: {}", c);
            }
        }

        output
    }
}
