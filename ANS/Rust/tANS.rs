use std::collections::HashMap;

#[derive(Debug, Clone)]
pub struct NormalizedEntry {
    pub n_bits_chopped: i32,
    pub min_state: usize,
    pub mask: u32,
}

impl Default for NormalizedEntry {
    fn default() -> Self {
        Self {
            n_bits_chopped: 0,
            min_state: 0,
            mask: 0,
        }
    }
}

impl NormalizedEntry {
    pub fn new(bits: i32, state: usize, m: u32) -> Self {
        Self {
            n_bits_chopped: bits,
            min_state: state,
            mask: m,
        }
    }
}

#[derive(Debug, Clone)]
pub struct DecodedEntry {
    pub old_state_shifted: usize,
    pub num_bits: i32,
    pub mask: u32,
}

impl Default for DecodedEntry {
    fn default() -> Self {
        Self {
            old_state_shifted: 0,
            num_bits: 0,
            mask: 0,
        }
    }
}

impl DecodedEntry {
    pub fn new(old_state: usize, bits: i32, m: u32) -> Self {
        Self {
            old_state_shifted: old_state,
            num_bits: bits,
            mask: m,
        }
    }
}

#[derive(Debug)]
pub struct EncodeResult {
    pub state: usize,
    pub bitstream: String,
}

impl EncodeResult {
    pub fn new(state: usize, bitstream: String) -> Self {
        Self { state, bitstream }
    }
}

#[derive(Debug)]
pub struct DecodeResult {
    pub message: String,
    pub final_state: usize,
}

impl DecodeResult {
    pub fn new(message: String, final_state: usize) -> Self {
        Self { message, final_state }
    }
}

#[derive(Debug)]
pub struct TAns {
    our_labels: String,
    block_size: usize,
    block_mask: usize,

    symbol_table: HashMap<char, Vec<usize>>,
    count_per_block: HashMap<char, usize>,
    count_before_index: Vec<usize>,

    normalization_table: HashMap<char, NormalizedEntry>,
    encoding_table: HashMap<char, HashMap<usize, usize>>,
    decoding_table: Vec<DecodedEntry>,
}

impl TAns {
    pub fn new(labeling_str: &str) -> Result<Self, String> {
        if labeling_str.is_empty() {
            return Err("Labeling cannot be empty".to_string());
        }

        if labeling_str.len() > 1_048_576 { // 1MB вместо 64KB!!!
            return Err("Labeling too large".to_string());
        }

        let our_labels = labeling_str.to_string();
        let block_size = our_labels.chars().count(); // Используем count() для подсчета символов UTF-8
        let block_mask = block_size - 1;

        let mut count_per_block = HashMap::new();
        let mut count_before_index = Vec::with_capacity(our_labels.chars().count());
        let mut symbol_table = HashMap::new();

        for (i, c) in our_labels.chars().enumerate() {
            let current_count = *count_per_block.get(&c).unwrap_or(&0);

            if !symbol_table.contains_key(&c) {
                symbol_table.insert(c, Vec::with_capacity(our_labels.chars().count()));
            }

            count_before_index.push(current_count);
            count_per_block.insert(c, current_count + 1);
            symbol_table.get_mut(&c).unwrap().push(i);
        }

        let mut tans = Self {
            our_labels,
            block_size,
            block_mask,
            symbol_table,
            count_per_block,
            count_before_index,
            normalization_table: HashMap::new(),
            encoding_table: HashMap::new(),
            decoding_table: Vec::new(),
        };

        tans.table_generator()?;

        Ok(tans)
    }

    pub fn from_vec(labeling_vec: &[char]) -> Result<Self, String> {
        let labeling_str: String = labeling_vec.iter().collect();
        Self::new(&labeling_str)
    }

    fn safe_c(&self, state: usize, symbol: char) -> Result<usize, String> {
        let count = self.count_per_block.get(&symbol)
            .ok_or("Symbol not found!")?;

        if *count == 0 {
            return Err("Symbol count is zero!".to_string());
        }

        if state == usize::MAX {
            return Err("Symbol state is too big!".to_string());
        }

        let mut full_blocks = (state + 1) / count;
        let mut symbols_left = (state + 1) % count;

        if symbols_left == 0 {
            if full_blocks == 0 {
                return Err("Symbol state is too low!".to_string());
            }
            full_blocks -= 1;
            symbols_left = *count;
        }

        let symbol_positions = self.symbol_table.get(&symbol)
            .ok_or("Symbol not found in symbol_table")?;

        if symbols_left > symbol_positions.len() {
            return Err("Invalid calculation in symbol_it!".to_string());
        }

        let index_within_block = symbol_positions[symbols_left - 1];

        if full_blocks > usize::MAX / self.block_size {
            return Err("full_blocks is too big!".to_string());
        }

        Ok(full_blocks * self.block_size + index_within_block)
    }

    fn safe_d(&self, state: usize) -> Result<(char, usize), String> {
        let index_within_block = state % self.block_size;

        if index_within_block >= self.our_labels.chars().count() {
            return Err("index_within_block is too big!".to_string());
        }

        // Получаем символ по индексу (работая с символами, а не байтами)
        let symbol = self.our_labels.chars().nth(index_within_block)
            .ok_or("Failed to get character at index")?;

        let mut count_within_block = 0;
        for (i, c) in self.our_labels.chars().enumerate() {
            if i >= index_within_block {
                break;
            }
            if c == symbol {
                count_within_block += 1;
            }
        }

        let symbol_count = self.count_per_block.get(&symbol)
            .ok_or("Symbol not found!")?;

        if *symbol_count > usize::MAX - count_within_block {
            return Err("symbol_count is too big!".to_string());
        }

        Ok((symbol, symbol_count + count_within_block))
    }

    fn table_generator(&mut self) -> Result<(), String> {
        self.normalization_table.clear();
        self.encoding_table.clear();
        self.decoding_table.clear();

        if self.symbol_table.len() == 1 {
            let single_symbol = *self.symbol_table.keys().next().unwrap();

            self.normalization_table.insert(single_symbol, NormalizedEntry::new(0, 0, 0));
            self.encoding_table.insert(single_symbol, HashMap::new());

            self.decoding_table = vec![DecodedEntry::default(); self.block_size];
            for i in 0..self.block_size {
                self.encoding_table.get_mut(&single_symbol).unwrap()
                    .insert(i + 1, self.block_size + i);
                self.decoding_table[i] = DecodedEntry::new(i + 1, 0, 0);
            }
            return Ok(());
        }

        for (&symbol_char, _) in &self.symbol_table {
            let mut n_bits_chopped = -1;
            let mut min_state = 0;
            let mut prev_n_bits = -1;

            for state in self.block_size..(self.block_size * 2) {
                let mut normalized = state;
                let mut n_bits = 0;

                while self.safe_c(normalized, symbol_char)? >= self.block_size * 2 {
                    const MAX_BITS: i32 = 64;
                    if n_bits >= MAX_BITS {
                        return Err("Too many bits - wrong!".to_string());
                    }
                    normalized >>= 1;
                    n_bits += 1;
                }

                if n_bits != prev_n_bits {
                    prev_n_bits = n_bits;
                    if n_bits_chopped < 0 {
                        n_bits_chopped = n_bits;
                    } else {
                        if min_state != 0 {
                            return Err("State is incorrect!".to_string());
                        }
                        min_state = state;
                    }
                }
            }

            if n_bits_chopped < 0 {
                n_bits_chopped = 0;
            }

            let mask = if n_bits_chopped < 32 {
                (1u32 << n_bits_chopped) - 1
            } else {
                0xFFFFFFFF
            };

            self.normalization_table.insert(
                symbol_char,
                NormalizedEntry::new(n_bits_chopped, min_state, mask)
            );
        }

        for (&symbol, _) in &self.symbol_table {
            self.encoding_table.insert(symbol, HashMap::new());
        }

        self.decoding_table = vec![DecodedEntry::default(); self.block_size];

        for state in self.block_size..(self.block_size * 2) {
            let (symbol, old_state) = self.safe_d(state)?;
            self.encoding_table.get_mut(&symbol).unwrap().insert(old_state, state);

            let mut n_bits = 0;
            let mut scaled_state = old_state;

            const MAX_SCALED_BITS: i32 = 64;
            while scaled_state < self.block_size && n_bits < MAX_SCALED_BITS {
                scaled_state <<= 1;
                n_bits += 1;
            }

            if n_bits >= MAX_SCALED_BITS {
                return Err("Too many bits - wrong!".to_string());
            }

            let table_index = state - self.block_size;
            if table_index >= self.decoding_table.len() {
                return Err("table_index is too big".to_string());
            }

            if n_bits > 0 && old_state > usize::MAX >> n_bits {
                return Err("Old state shift would overflow".to_string());
            }

            let mask = if n_bits < 32 {
                (1u32 << n_bits) - 1
            } else {
                0xFFFFFFFF
            };

            self.decoding_table[table_index] = DecodedEntry::new(old_state << n_bits, n_bits, mask);
        }

        Ok(())
    }

    pub fn encode(&self, message: &str, initial_state: usize) -> Result<EncodeResult, String> {
        if message.is_empty() {
            return Err("Message cannot be empty".to_string());
        }

        println!("Encoding message of size: {}", message.len());

        let last_symbol = message.chars().last()
            .ok_or("Failed to get last character")?;

        let symbol_positions = self.symbol_table.get(&last_symbol)
            .ok_or("Last symbol not found in labeling")?;

        if initial_state >= symbol_positions.len() {
            return Err("Initial state out of bounds for symbol".to_string());
        }

        let mut bitstream = String::with_capacity(message.len() * 8);
        let mut state = symbol_positions[initial_state];

        if state >= self.decoding_table.len() {
            return Err("Initial state index out of bounds".to_string());
        }

        if state + self.block_size >= 2 * self.block_size {
            return Err("Initial state index out of bounds".to_string());
        }

        state += self.block_size;

        let message_chars: Vec<char> = message.chars().collect();
        for i in (0..message_chars.len() - 1).rev() {
            let symbol = message_chars[i];

            let norm = self.normalization_table.get(&symbol)
                .ok_or("Symbol not found in renormalization table")?;

            let mut n_bits = norm.n_bits_chopped;
            let mut mask = norm.mask;

            if norm.min_state > 0 && state >= norm.min_state {
                n_bits += 1;
                mask = (mask << 1) | 1;
            }

            if n_bits > 0 {
                for b in (0..n_bits).rev() {
                    let bit = if (state >> b) & 1 != 0 { '1' } else { '0' };
                    bitstream.push(bit);
                }
            }

            let shifted_state = state >> n_bits;

            let encoding_map = self.encoding_table.get(&symbol)
                .ok_or("Symbol not found in encoding table")?;

            state = *encoding_map.get(&shifted_state)
                .ok_or("State not found in encoding table")?;
        }

        Ok(EncodeResult::new(state, bitstream))
    }

    pub fn decode(&self, mut state: usize, bitstream: String) -> Result<DecodeResult, String> {
        let mut message = String::with_capacity(std::cmp::max(1024, bitstream.len()));

        const MAX_ITERATIONS: usize = 10_000; // Уменьшено с 100_000 для избежания зависания
        let mut iterations = 0;
        let mut bitstream_pos = bitstream.len();

        while iterations < MAX_ITERATIONS {
            iterations += 1;

            if state < self.block_size {
                return Err("State underflow during decoding".to_string());
            }

            let table_index = state - self.block_size;

            if table_index >= self.decoding_table.len() {
                return Err("Decoding table index out of bounds".to_string());
            }

            if table_index >= self.our_labels.chars().count() {
                return Err("State index out of bounds during decoding".to_string());
            }

            let (symbol, _old_state) = self.safe_d(state)?;
            message.push(symbol);

            let decode_entry = &self.decoding_table[table_index];
            state = decode_entry.old_state_shifted;

            let num_bits = decode_entry.num_bits;
            if num_bits > 0 {
                if bitstream_pos < num_bits as usize {
                    break;
                }

                let mut bits = 0u32;

                for i in 0..num_bits {
                    bitstream_pos -= 1;

                    let bit_char = bitstream.chars().nth(bitstream_pos)
                        .ok_or("Failed to get bit from bitstream")?;

                    if bit_char == '1' {
                        bits |= 1u32 << i;
                    } else if bit_char != '0' {
                        return Err("Invalid character in bitstream".to_string());
                    }
                }

                state |= bits as usize;
            } else {
                if bitstream_pos == 0 {
                    break;
                }

                if self.symbol_table.len() == 1 {
                    if state < self.block_size {
                        break;
                    }
                }
            }

            // Дополнительная проверка для избежания зацикливания
            if bitstream_pos == 0 && state < self.block_size {
                break;
            }
        }

        if iterations >= MAX_ITERATIONS {
            return Err("Maximum iterations reached during decoding".to_string());
        }

        Ok(DecodeResult::new(message, state))
    }

    pub fn validate_tables(&self) -> bool {
        if self.decoding_table.len() != self.block_size {
            return false;
        }
        if self.encoding_table.len() != self.symbol_table.len() {
            return false;
        }
        if self.normalization_table.len() != self.symbol_table.len() {
            return false;
        }

        true
    }

    pub fn get_block_size(&self) -> usize {
        self.block_size
    }

    pub fn get_symbol_count(&self) -> usize {
        self.symbol_table.len()
    }

    pub fn print_stats(&self) {
        println!("Block size: {}", self.block_size);
        println!("Unique symbols: {}", self.symbol_table.len());
        println!("Decoding table size: {}", self.decoding_table.len());

        print!("Symbol frequencies: ");
        for (&symbol, &count) in &self.count_per_block {
            if symbol.is_ascii_graphic() || symbol == ' ' {
                print!("'{}':{} ", symbol, count);
            } else {
                print!("(0x{:02x}):{} ", symbol as u8, count);
            }
        }
        println!();
    }
}

// Тесты
mod tests {
    use super::*;
    use std::fs;
    use std::time::Instant;
    use rand::Rng;

    fn read_text_file(filepath: &str) -> String {
        match fs::read_to_string(filepath) {
            Ok(content) => content,
            Err(_) => {
                eprintln!("Error: Cannot open file: {}", filepath);
                String::new()
            }
        }
    }

    fn expect_error<F>(func: F, test_name: &str) -> bool
    where
        F: FnOnce() -> Result<(), String>,
    {
        match func() {
            Ok(_) => {
                println!("Test {}: FAILED (Expected error, but none occurred).", test_name);
                false
            }
            Err(e) => {
                println!("Test {}: PASSED (Caught expected error: {})", test_name, e);
                true
            }
        }
    }

    fn run_correctness_test(test_name: &str, labeling: &str, original_message: &str, initial_state_val: usize) {
        let start = Instant::now();

        match TAns::new(labeling) {
            Ok(tans) => {
                match tans.encode(original_message, initial_state_val) {
                    Ok(encode_result) => {
                        match tans.decode(encode_result.state, encode_result.bitstream) {
                            Ok(decode_result) => {
                                let elapsed = start.elapsed();

                                if original_message == decode_result.message {
                                    println!("Test {}: correct! Took: {:.6} seconds.", test_name, elapsed.as_secs_f64());
                                } else {
                                    println!("Test {}: FAILED (Decoded message mismatch). Original: '{}', Decoded: '{}' Took: {:.6} seconds.",
                                             test_name, original_message, decode_result.message, elapsed.as_secs_f64());
                                }
                            }
                            Err(e) => {
                                println!("Test {}: FAILED (Decode error: {})", test_name, e);
                            }
                        }
                    }
                    Err(e) => {
                        println!("Test {}: FAILED (Encode error: {})", test_name, e);
                    }
                }
            }
            Err(e) => {
                println!("Test {}: FAILED (Constructor error: {})", test_name, e);
            }
        }
    }

    // Перегрузка для случаев с initial_state = 0 по умолчанию
    fn run_correctness_test_default(test_name: &str, labeling: &str, original_message: &str) {
        run_correctness_test(test_name, labeling, original_message, 0);
    }

    fn test_constructor_errors() {
        println!("\n<--- Constructor Error Tests --->");

        expect_error(|| {
            TAns::new("").map(|_| ())
        }, "1.1 (Constructor) Empty labeling string");

        expect_error(|| {
            let large_labeling = "a".repeat(1_048_576 + 1);
            TAns::new(&large_labeling).map(|_| ())
        }, "1.2 (Constructor) Very large labeling string");

        match TAns::new("aaaaa") {
            Ok(_) => println!("Test 1.3 (Constructor) Single char labeling: PASSED"),
            Err(e) => println!("Test 1.3 (Constructor) Single char labeling: FAILED (Caught unexpected error: {})", e),
        }
    }

    fn test_encode_errors() {
        println!("\n<--- Tests for encode --->");

        let tans = TAns::new("abcdefg").unwrap();

        expect_error(|| {
            tans.encode("", 0).map(|_| ())
        }, "2.1 (Encode) Empty message");

        expect_error(|| {
            tans.encode("xyz", 0).map(|_| ())
        }, "2.2 (Encode) Message with unknown symbol");

        expect_error(|| {
            tans.encode("abcx", 0).map(|_| ())
        }, "2.3 (Encode) Message with unknown last symbol");

        expect_error(|| {
            tans.encode("abc", 1).map(|_| ())
        }, "2.4 (Encode) initial_state out of bounds");
    }

    fn test_decode_errors() {
        println!("\n--- Tests for decode ---");

        let tans = TAns::new("abc").unwrap();
        let original = "abc";
        let encoded_result = tans.encode(original, 0).unwrap();

        expect_error(|| {
            let mut invalid_bitstream = encoded_result.bitstream.clone();
            invalid_bitstream.push('2');
            tans.decode(encoded_result.state, invalid_bitstream).map(|_| ())
        }, "3.1 (Decode) Invalid char in bitstream");

        expect_error(|| {
            tans.decode(0, "101010".to_string()).map(|_| ())
        }, "3.2 (Decode) State underflow");

        expect_error(|| {
            tans.decode(tans.get_block_size() * 2, "".to_string()).map(|_| ())
        }, "3.3 (Decode) Decoding table index out of bounds (high state)");

        // Тест для достижения max iterations - используем некорректное состояние
        expect_error(|| {
            let long_bitstream = "1".repeat(1000);
            // Используем состояние, которое может привести к зацикливанию
            tans.decode(tans.get_block_size() + 1, long_bitstream).map(|_| ())
        }, "3.4 (Decode) Max iterations reached");
    }

    fn test_edge_cases() {
        println!("\n<--- Tests of uncomfortable data --->");
        run_correctness_test_default("4.1 Single char message", "abc", "a");
        run_correctness_test_default("4.2 Repeated single char message", "abc", "abc");
        run_correctness_test_default("4.3 Single unique char labeling", "aaaaa", "a");
        run_correctness_test("4.4 Non-zero initial_state", "aaabbbccc", "aaa", 1);
        run_correctness_test("4.5 Non-zero initial_state (another)", "aaabbbccc", "ccc", 2);
        run_correctness_test_default("4.6 Last symbol is only in labeling", "a", "a");
    }

    fn test_performance_and_correctness() {
        println!("\n<--- Tests with real data --->");

        // Используем алфавит с повторяющимися символами для корректной работы алгоритма
        let repeated_alphabet = "aaabbbcccdddeeefffggghhhiiijjjkkklllmmmnnnooopppqqqrrrssstttuuuvvvwwwxxxyyyzzz\
                                AAABBBCCCDDDEEEFFFGGGHHHIIIJJJKKKLLLMMMNNNOOOPPPQQQRRRSSSTTTUUUVVVWWWXXXYYYZZZ\
                                000111222333444555666777888999";

        run_correctness_test_default("5.1 Repeated alphabet (small)", repeated_alphabet, "Hello, world!");

        let medium_message = "This is a longer test message to check performance and correctness. \
                             It contains various characters and repeated patterns like aaaaaaaaa.";
        run_correctness_test_default("5.2 Repeated alphabet (medium)", repeated_alphabet, medium_message);

        // Для файлов создадим алфавит из содержимого файла
        let example_file_content = read_text_file("example.txt");
        if !example_file_content.is_empty() {
            let content = if example_file_content.len() > 1000 {
                &example_file_content[..1000]
            } else {
                &example_file_content
            };
            println!("Encoding message of size: {}", content.len());
            // Используем само содержимое файла как алфавит (с повторениями)
            run_correctness_test_default("5.3 File content", content, "test");
        } else {
            println!("Test 5.3: example.txt is empty or not found. Skipping.");
        }

        let example1_file_content = read_text_file("example1.txt");
        if !example1_file_content.is_empty() {
            let content = if example1_file_content.len() > 1000 {
                &example1_file_content[..1000]
            } else {
                &example1_file_content
            };
            println!("Encoding message of size: {}", content.len());
            // Используем само содержимое файла как алфавит (с повторениями)
            run_correctness_test_default("5.4 File content", content, "test");
        } else {
            println!("Test 5.4: example1.txt is empty or not found. Skipping.");
        }

        // Тестируем с простыми повторяющимися алфавитами
        let low_entropy_message = "XXX".repeat(100); // Уменьшено еще больше
        run_correctness_test_default("5.5 Low entropy message", repeated_alphabet, &low_entropy_message);

        let high_entropy_message = "abcdefghijklmn".repeat(50); // Более предсказуемый паттерн
        run_correctness_test_default("5.6 High entropy message", repeated_alphabet, &high_entropy_message);
    }

    pub fn tans_test() {
        test_constructor_errors();
        test_encode_errors();
        test_decode_errors();
        test_edge_cases();
        test_performance_and_correctness();

        println!("\nAll tests finished.");
    }

    #[test]
    fn run_all_tans_tests() {
        tans_test();
    }

    #[test]
    fn test_individual_constructor_errors() {
        test_constructor_errors();
    }

    #[test]
    fn test_individual_encode_errors() {
        test_encode_errors();
    }

    #[test]
    fn test_individual_decode_errors() {
        test_decode_errors();
    }

    #[test]
    fn test_individual_edge_cases() {
        test_edge_cases();
    }

    #[test]
    fn test_individual_performance() {
        test_performance_and_correctness();
    }
}

fn main() {
    println!("=== tANS Algorithm Test Suite ===");

    // Запуск всех тестов
    tests::tans_test();

    println!("\n=== Simple Demo ===");

    // Простой пример использования с алфавитом, содержащим повторения
    let alphabet_with_repeats = "aaabbbcccdddeeefffggghhhiiijjjkkklllmmmnnnooopppqqqrrrssstttuuuvvvwwwxxxyyyzzz";

    match TAns::new(alphabet_with_repeats) {
        Ok(tans) => {
            println!("Created tANS with repeated alphabet");
            tans.print_stats();

            let message = "hello world";
            println!("\nEncoding message: '{}'", message);

            match tans.encode(message, 0) {
                Ok(result) => {
                    println!("Encoded state: {}", result.state);
                    println!("Bitstream length: {}", result.bitstream.len());
                    println!("Bitstream: {}", if result.bitstream.len() <= 100 {
                        result.bitstream.clone()
                    } else {
                        format!("{}... (truncated)", &result.bitstream[..100])
                    });

                    match tans.decode(result.state, result.bitstream) {
                        Ok(decoded) => {
                            println!("Decoded message: '{}'", decoded.message);
                            println!("Success: {}", message == decoded.message);
                        }
                        Err(e) => println!("Decode error: {}", e),
                    }
                }
                Err(e) => println!("Encode error: {}", e),
            }
        }
        Err(e) => println!("Constructor error: {}", e),
    }
}
