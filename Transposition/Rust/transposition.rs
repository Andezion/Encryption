#[derive(Default)]
pub(crate) struct Transposition {
    pub input: String,
}

impl Transposition {
    pub fn new(input: String) -> Self {
        Self { input }
    }

    pub(crate) fn encode(&self) -> String {
        let mut output = String::new();

        let letters: Vec<char> = self.input
            .chars()
            .filter(|c| c.is_alphabetic())
            .map(|c| c.to_ascii_uppercase())
            .collect();

        if letters.is_empty() {
            return output;
        }

        let cols = 5;
        let rows = (letters.len() + cols - 1) / cols; 
        let mut matrix: Vec<Vec<char>> = vec![vec![' '; cols]; rows];

        for (i, ch) in letters.iter().enumerate() {
            let row = i / cols;
            let col = i % cols;
            matrix[row][col] = *ch;
        }

        for col in 0..cols {
            for row in 0..rows {
                let ch = matrix[row][col];
                if ch != ' ' {
                    output.push(ch);
                }
            }
        }

        output
    }

    pub(crate) fn decode(&self, encoded_version: &String) -> String {
        let mut output = String::new();

        let total_chars = encoded_version.len();
        if total_chars == 0 {
            return output;
        }

        let cols = 5;
        let rows = (total_chars + cols - 1) / cols;

        let mut col_lengths = vec![rows; cols];
        let extra = cols * rows - total_chars;
        for i in (cols - extra)..cols {
            col_lengths[i] -= 1;
        }

        let mut columns: Vec<Vec<char>> = vec![];
        let mut idx = 0;
        for &len in &col_lengths {
            let col: Vec<char> = encoded_version[idx..idx + len].chars().collect();
            columns.push(col);
            idx += len;
        }

        for row in 0..rows {
            for col in 0..cols {
                if row < columns[col].len() {
                    output.push(columns[col][row]);
                }
            }
        }

        output
    }
}
