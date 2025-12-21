# Vigenère Cipher

*Short description:*

The Vigenère cipher is a polyalphabetic substitution cipher that uses a repeating key to perform shifts on the plaintext letters. It is stronger than a simple Caesar cipher but can be broken with frequency and key-length analysis.

**Implementation:**
- Implementations and tests may be present in C++ and Rust subdirectories.

**Algorithm (brief):**

1. Repeat or truncate the key to match the plaintext length.
2. For each plaintext letter, shift it by the corresponding key letter (e.g., A=0 shift, B=1 shift).
3. Decoding subtracts the key shifts to recover the plaintext.

**Example:**

Input: "ATTACKATDAWN", key: "LEMON"
Output: "LXFOPVEFRNHR" (classic Vigenère example)


