use std::{collections::HashSet, hash::Hash};

#[derive(Default)]
pub(crate) struct Playfair {
    pub key: String,
    pub input: String,

    pub matrix: [[char; 5]; 5],
}

impl Playfair {
    pub fn new(input: String, key: String, matrix: &mut [[char; 5]; 5]) -> Self {
        let mut combined: String = String::new();
        let mut seen: HashSet<char> = HashSet::new();

        for c in key.chars() {
            let mut c_mut: char = c.to_ascii_uppercase();
            if c_mut == 'J' {
                c_mut = 'I';
            }

            if !seen.contains(&c_mut) && c_mut.is_alphabetic() {
                combined.push(c_mut);
                seen.insert(c_mut);
            }
        }

        let mut j: u8 = 'A' as u8;
        while j <= 'Z' as u8 {
            if j == 'J' as u8 {
                continue;
            }
            if !seen.contains(&(j as char)) {
                combined.push(j as char);
                seen.insert(j as char);
            }

            j += 1;
        }

        for i in 0..25 {
            matrix[i / 5][i % 5] = combined.chars().nth(i).expect("no way this falls");
        }

        for i in 0..5 {
            for j in 0..5 {
                print!("{} ",matrix[i][j]);
            }
            println!();
        }

        Self { input, key, matrix: *matrix }
    }

    fn find_position(&self, c: Option<char>) -> (i32, i32) {
        for i in 0..5 {
            for j in 0..5 {
                if self.matrix[i][j] == c.expect("i hate this") {
                    return (i as i32, j as i32);
                }
            }
        }

        (-1, -1)
    }

    pub(crate) fn encode(&self) -> String {
        let mut cleaned: String = String::new();
        for mut c in self.input.chars() {
            c = c.to_ascii_uppercase();
            if c == 'J' {
                c = 'I';
            }
            if c.is_alphabetic() {
                cleaned.push(c);
            }
        }

        let mut i: usize = 0;
        while i < cleaned.len() {
            if i + 1 == cleaned.len() {
                break;
            }

            if cleaned.chars().nth(i) == cleaned.chars().nth(i + 1) {
                cleaned.insert(i + 1, 'X');
            }

            i += 2;
        }

        if cleaned.len() % 2 != 0 {
            cleaned.push('X');
        }

        let mut output: String = String::new();

        i = 0;
        while i < cleaned.len() {
            let a: Option<char> = cleaned.chars().nth(i);
            let b: Option<char> = cleaned.chars().nth(i + 1);

            let (row1, col1): (i32, i32) = self.find_position(a);
            let (row2, col2): (i32, i32) = self.find_position(b);

            if row1 == row2 {
                output.push(self.matrix[row1 as usize][(col1 as usize + 1) % 5]);
                output.push(self.matrix[row2 as usize][(col2 as usize+ 1) % 5]);
            }
            else if col1 == col2 {
                output.push(self.matrix[(row1 as usize + 1) % 5][col1 as usize]);
                output.push(self.matrix[(row2 as usize + 1) % 5][col2 as usize]);
            }
            else {
                output.push(self.matrix[row1 as usize][col2 as usize]);
                output.push(self.matrix[row2 as usize][col1 as usize]);
            }

            i += 2;
        }

        output
    }

    pub(crate) fn decode(&self, encoded_version: &String) -> String {
        let mut output: String = String::new();

        let mut i: usize = 0;
        while i < encoded_version.len() {
            let a: Option<char> = encoded_version.chars().nth(i);
            let b: Option<char> = encoded_version.chars().nth(i + 1);

            let (row1, col1): (i32, i32) = self.find_position(a);
            let (row2, col2): (i32, i32) = self.find_position(b);

            if row1 == row2 {
                output.push(self.matrix[row1 as usize][(col1 as usize + 4) % 5]);
                output.push(self.matrix[row2 as usize][(col2 as usize + 4) % 5]);
            }
            else if col1 == col2 {
                output.push(self.matrix[(row1 as usize + 4) % 5][col1 as usize]);
                output.push(self.matrix[(row2 as usize + 4) % 5][col2 as usize]);
            }
            else {
                output.push(self.matrix[row1 as usize][col2 as usize]);
                output.push(self.matrix[row2 as usize][col1 as usize]);
            }

            i += 2;
        }

        output
    }
}
