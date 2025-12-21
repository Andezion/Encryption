# Polybius Square

*Short description:*

The Polybius square is a substitution cipher that maps letters to coordinates in a square (typically 5x5). Each letter is represented by a pair of row/column numbers, making it useful for simple manual encoding and historical ciphers.

**Implementation:**
- Implementations and tests may be present in C++ and Rust subdirectories.

**Algorithm (brief):**

1. Construct a square filled with the alphabet (often merging I/J into one cell).
2. For each plaintext letter, output the corresponding row and column numbers.
3. Decoding maps coordinate pairs back to letters.

**Example:**

Input: "HELLO"
Output: a sequence of coordinates (row/column pairs) representing each letter (format depends on implementation).

