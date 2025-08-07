#[derive(Default)]
pub(crate) struct Transposition {
    pub input: String,
}

impl Transposition {
    pub fn new(input: String) -> Self {
        Self { input }
    }

    pub(crate) fn encode(&self) -> String {
        let mut output: String = String::new();

        let mut matrix: Vec<Vec<char>> = vec![vec![]];
        let mut row: Vec<char> = vec![];

        for letter in self.input.chars() {
            if letter.is_alphabetic() {
                row.push(letter);
                if row.len() == 5 {
                    matrix.push(row.clone());
                    row.clear();
                }
            }
        }

        for i in (0..matrix[0].len()).rev() {
            for j in 0..matrix.len() {
                output.push(matrix[i][j].to_ascii_uppercase());
            }
        }

        output
    }

    pub(crate) fn decode(&self, encoded_version: &String) -> String {
        let mut output: String = String::new();

        let rows: usize = encoded_version.len() / 5;
        let mut matrix: Vec<Vec<char>> = vec![vec![]];

        let mut index: i32 = 0;
        for i in (0..4).rev() {
            for j in 0..rows {
                matrix[i][j] = encoded_version.chars().nth(index as usize).expect("smth");
                index += 1;
            }
        }

        for i in 0..rows {
            for j in 0..5 {
                output.push(matrix[i][j]);
            }
        }

        output
    }
}
