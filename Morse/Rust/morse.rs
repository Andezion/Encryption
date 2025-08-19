use std::collections::HashMap;

pub struct Morse {
    input: String,
}

impl Morse {
    pub fn new(input: String) -> Self {
        Self { input }
    }

    fn encode_map() -> HashMap<char, &'static str> {
        HashMap::from([
            ('A', "._"), ('B', "_..."), ('C', "_._."), ('D', "_.."), ('E', "."),
            ('F', ".._."), ('G', "__."), ('H', "...."), ('I', ".."), ('J', ".___"),
            ('K', "_._"), ('L', "._.."), ('M', "__"), ('N', "_."), ('O', "___"),
            ('P', ".__."), ('Q', "__._"), ('R', "._."), ('S', "..."), ('T', "_"),
            ('U', ".._"), ('V', "..._"), ('W', ".__"), ('X', "_.._"), ('Y', "_.__"),
            ('Z', "__.."),
            ('0', "_____"), ('1', ".____"), ('2', "..___"), ('3', "...__"),
            ('4', "...._"), ('5', "....."), ('6', "_...."), ('7', "__..."),
            ('8', "___.."), ('9', "____."),
            ('.', "._._._"), (',', "__..__"), ('?', "..__.."), ('!', "_._.__"),
            (' ', "/"),
        ])
    }

    fn decode_map() -> HashMap<&'static str, char> {
        HashMap::from([
            ("._", 'A'), ("_...", 'B'), ("_._.", 'C'), ("_..", 'D'), (".", 'E'),
            (".._.", 'F'), ("__.", 'G'), ("....", 'H'), ("..", 'I'), (".___", 'J'),
            ("_._", 'K'), ("._..", 'L'), ("__", 'M'), ("_.", 'N'), ("___", 'O'),
            (".__.", 'P'), ("__._", 'Q'), ("._.", 'R'), ("...", 'S'), ("_", 'T'),
            (".._", 'U'), ("..._", 'V'), (".__", 'W'), ("_.._", 'X'), ("_.__", 'Y'),
            ("__..", 'Z'),
            ("_____", '0'), (".____", '1'), ("..___", '2'), ("...__", '3'),
            ("...._", '4'), (".....", '5'), ("_....", '6'), ("__...", '7'),
            ("___..", '8'), ("____.", '9'),
            ("._._._", '.'), ("__..__", ','), ("..__..", '?'), ("_._.__", '!'),
            ("/", ' '),
        ])
    }

    pub fn encode(&self) -> String {
        let map = Self::encode_map();
        let mut output = String::new();

        for c in self.input.chars() {
            if let Some(code) = map.get(&c.to_ascii_uppercase()) {
                output.push_str(code);
                output.push(' ');
            }
        }
        output.trim_end().to_string()
    }

    pub fn decode(&self, encoded_version: &str) -> String {
        let map = Self::decode_map();
        let mut output = String::new();

        for word in encoded_version.split_whitespace() {
            if let Some(ch) = map.get(word) {
                output.push(*ch);
            }
        }
        output
    }
}
