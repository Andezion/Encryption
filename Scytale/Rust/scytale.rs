use std::ascii::AsciiExt;

#[derive(Default)]
pub(crate) struct Scytale {
    pub input: String,
    pub cols: usize,
    pub rev: bool,
}

impl Scytale {
    pub fn new(input: String, cols: usize, rev: bool) -> Self {
        for mut i in input.chars() {
            if i.is_alphabetic() {
                i = i.to_ascii_lowercase();
            }
        }
        Self { input, cols, rev }
    }

    pub(crate) fn encode(&self) -> String {
        let mut matrix: Vec<Vec<char>> = vec![vec![]];
        let mut row: Vec<char> = vec![];

        for i in self.input.chars() {
            row.push(i);

            if row.len() == self.cols {
                matrix.push(row.clone());
                row.clear();
            }
        }

        if row.is_empty() != true {
            while row.len() < self.cols {
                row.push('x');
            }
            matrix.push(row.clone());
        }

        let mut output: String = String::new();

        if self.rev == true {
            for j in (0..self.cols - 1).rev() {
                for i in 0..matrix.len() {
                    output.push(matrix[i][j].to_ascii_uppercase());
                }
            }
        }
        else {
            for j in 0..self.cols {
                for i in 0..matrix.len() {
                    output.push(matrix[i][j].to_ascii_uppercase());
                }
            }
        }

        output
    }

    pub(crate) fn decode(&self, encoded_version: &String, cols: usize, rev: bool) -> String {
        let rows: usize = encoded_version.len() / cols;

        let mut matrix: Vec<Vec<char>> = vec![vec![0 as char; cols]; rows];
        let mut index: usize = 0;

        if self.rev {
            for i in (0..cols).rev() {
                for j in 0..rows {
                    matrix[i][j] = encoded_version.chars().nth(index).unwrap().to_ascii_lowercase();
                    index += 1;
                }
            }
        }
        else {
            for i in 0..cols {
                for j in 0..rows {
                    matrix[i][j] = encoded_version.chars().nth(index).unwrap().to_ascii_lowercase();
                    index += 1;
                }
            }
        }

        let mut output: String = String::new();
        for i in 0..rows {
            for j in 0..cols {
                output.push(matrix[i][j]);
            }
        }

        while output.is_empty() != true && output.chars().nth(output.len() - 1) == Option::from('x') {
            output.pop();
        }

        output
    }
}
