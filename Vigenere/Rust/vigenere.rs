#[derive(Default)]
pub(crate) struct Vigenere {
    pub input: String,
    pub key: String,
    pub table: [[char; 26]; 26],
}

impl Vigenere {
    pub fn new(input: String, key: String, table: &mut [[char; 26]; 26]) -> Self {
        for i in 0..26 {
            for j in 0..26 {
                table[i][j] = ('a' as u8 + (i as u8 + j as u8) % 26) as char;
            }
        }

        Self { input, key, table: *table }
    }

    pub(crate) fn encode(&self) -> String {
        let mut output: String = String::new();
        let key_len: usize = self.key.len();
        let mut key_index: usize = 0;

        for i in self.input.chars() {
            if i.is_lowercase() {
                let row: u8 = i as u8 - 'a' as u8;
                if let Some(col) = self.key.chars().nth(key_index % key_len) {
                    output.push(self.table[row as usize][(col.to_ascii_lowercase() as u8 - 'a' as u8) as usize]);
                    key_index += 1;
                }
            }
            else if i.is_uppercase() {
                let row: u8 = i.to_ascii_lowercase() as u8 - 'a' as u8;
                if let Some(col) = self.key.chars().nth(key_index % key_len) {
                    output.push(self.table[row as usize][(col.to_ascii_uppercase() as u8 - 'a' as u8) as usize]);
                    key_index += 1;
                }
            }
            else {
                output.push(i);
            }
        }

        output
    }
}
