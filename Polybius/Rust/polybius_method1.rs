#[derive(Default)]
pub(crate) struct Polybius {
    pub input: String,
    pub storage: [[char; 5]; 5],
}

impl Polybius {
    pub fn new(input: String) -> Self {
        let mut storage: [[char; 5]; 5] = [['a'; 5]; 5];
        let mut letter: char = 'A';
        for i in 0.. 5 {
            for j in 0..5 {
                if letter == 'J' {
                    letter = (letter as u8 + 1) as char;
                }
                storage[i][j] = letter;
                letter = (letter as u8 + 1) as char;
            }
        }

        Self { input, storage }
    }

    pub(crate) fn encode(&self) -> String {
        let mut output: String = String::new();

        for c in self.input.chars() {
            let mut letter = c;
            if letter.is_alphabetic() {
                letter = c.to_ascii_uppercase();
            }

            for i in 0..5 {
                for j in 0..5 {
                    if letter == self.storage[i][j] {
                        if i == 4 {
                            output.push(self.storage[0][j]);
                        }
                        else {
                            output.push(self.storage[i + 1][j]);
                        }
                    }
                }
            }
        }

        output
    }

    pub(crate) fn decode(&self, encoded_version: &String) -> String {
        let mut output: String = String::new();

        for c in self.input.chars() {
            for i in 0..5 {
                for j in 0..5 {
                    if c == self.storage[i][j] {
                        if i == 0 {
                            output.push(self.storage[4][j]);
                        }
                        else {
                            output.push(self.storage[i - 1][j]);
                        }
                    }
                }
            }
        }

        output
    }
}
