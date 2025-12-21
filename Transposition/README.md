# Transposition Ciphers

*Short description:*

Transposition ciphers reorder the characters of the plaintext according to some fixed scheme (e.g., columnar transposition, rail fence). They preserve the original symbols but change their positions.

**Implementation:**
- Implementations and tests may be present in C, C++ and Rust subdirectories.

**Algorithm (brief):**

1. Choose a transposition scheme (number of columns, rail pattern, key order).
2. Arrange the plaintext into a matrix or rails according to the scheme.
3. Read the characters out in the prescribed order to form the ciphertext.

**Example:**

Input: "WEAREDISCOVERED"
Output: ciphertext produced by the chosen transposition method (implementation-dependent).


