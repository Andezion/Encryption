# Scytale

*Short description:*

Scytale is an ancient transposition cipher that arranges text along a strip wound around a rod of a given circumference. Reading characters along the rod produces the ciphertext; unwinding yields the original text when the same circumference is used.

**Implementation:**
- Implementations and tests may be present in C, C++ and Rust subdirectories.

**Algorithm (brief):**

1. Choose a circumference (number of rows or columns depending on convention).
2. Write the plaintext along the rod (filling rows or columns) and read off in the orthogonal direction to produce the ciphertext.
3. Decoding uses the same circumference to reconstruct the original ordering.

**Example:**

Input: "ATTACKATDAWN", circumference = 3
Output: ciphertext produced by reading the transposed matrix (format depends on implementation).


