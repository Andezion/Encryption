#[derive(Default)]
pub(crate) struct Substitution {
    pub input: String,
    pub key: String
}

impl Substitution {
    pub fn new(input: String, key: String) -> Self {
        Self { input, key }
    }

    pub(crate) fn encode(&self) -> String {
        let mut output: String = String::new();

        for c in self.input.chars() {
            if c.is_lowercase() {
                let index: usize = (c as u8 - b'a') as usize;

                if let Some(encoded) = self.key.chars().nth(index) {
                    output.push(encoded);
                }
            }
            else if c.is_uppercase() {
                let index: usize = (c as u8 - b'A') as usize;

                if let Some(encoded) = self.key.chars().nth(index) {
                    output.push(encoded);
                }
            }
            else {
                output.push(c);
            }
        }

        output
    }

    pub(crate) fn decode(&self, encoded_version: &String) -> String {
        let mut output: String = String::new();

        for c in encoded_version.chars() {
            if c.is_lowercase() {
                if let Some(encoded) = self.key.find(c) {
                    output.push((encoded as u8 + 'a' as u8) as char);
                }
            }
            else if c.is_uppercase() {
                if let Some(encoded) = self.key.find(c) {
                    output.push((encoded as u8 + 'A' as u8) as char);
                }
            }
            else {
                output.push(c);
            }
        }

        output
    }
}
