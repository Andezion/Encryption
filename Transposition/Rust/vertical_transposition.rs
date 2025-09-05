#[derive(Default)]
pub(crate) struct VerticalTransposition {
    pub input: String,
    pub indexes: Vec<usize>,
}

impl VerticalTransposition {
    pub fn new(input: String, indexes: Vec<usize>) -> Self {
        Self { input, indexes }
    }

    pub(crate) fn encode(&self) -> String {
        let mut output = String::new();

        let filtered: String = self.input.chars().filter(|c| c.is_alphabetic()).collect();
        let mut chars: Vec<char> = filtered.chars().collect();
    
        while chars.len() % 5 != 0 {
            chars.push('X');
        }

        let rows = chars.len() / 5;
        let mut matrix: Vec<Vec<char>> = Vec::with_capacity(rows);
        for i in 0..rows {
            matrix.push(chars[i * 5..(i + 1) * 5].to_vec());
        }
        
        for &col in &self.indexes {
            for row in &matrix {
                output.push(row[col].to_ascii_uppercase());
            }
        }

        output
    }

    pub(crate) fn decode(&self, encoded_version: &String) -> String {
        let mut output = String::new();

        let rows = encoded_version.len() / self.indexes.len();
        let mut matrix: Vec<Vec<char>> = vec![vec![' '; self.indexes.len()]; rows];

        let mut idx = 0;
        
        for (col_idx, &col) in self.indexes.iter().enumerate() {
            for row in 0..rows {
                matrix[row][col] = encoded_version.chars().nth(idx).unwrap();
                idx += 1;
            }
        }

        for row in matrix {
            for c in row {
                output.push(c);
            }
        }

        output
    }
}
