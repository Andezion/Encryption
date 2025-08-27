#[derive(Default)]
pub(crate) struct ROT47 {
    pub input: String
}

impl ROT47 {
    pub fn new(input: String) -> Self {
        Self { input }
    }

    pub(crate) fn encode(&self) -> String {
        let mut output = String::new();
        for c in self.input.chars() {
            if c.is_ascii() && (c as u8) >= 33 && (c as u8) <= 126 {
                let encoded = ((c as u8 - 33 + 47) % 94) + 33;
                output.push(encoded as char);
            } else {
                output.push(c);
            }
        }
        output
    }

    pub(crate) fn decode(&self, encoded_version: &String) -> String {
        let mut output = String::new();
        for c in encoded_version.chars() {
            if c.is_ascii() && (c as u8) >= 33 && (c as u8) <= 126 {
                let decoded = ((c as u8 - 33 + 47) % 94) + 33;
                output.push(decoded as char);
            } else {
                output.push(c);
            }
        }
        output
    }
}
