# Playfair Cipher

*Short description:*

The Playfair cipher is a digraph substitution cipher that encrypts pairs of letters using a 5x5 key table (usually combining I/J). It was historically used for manual encryption and provides better security than simple monoalphabetic substitution.

**Implementation:**
- Implementations and tests may be present in C++ and Rust subdirectories.

**Algorithm (brief):**

1. Build a 5x5 key table from a keyword, filling remaining letters.
2. Split the plaintext into digraphs (pairs), inserting filler characters if needed.
3. For each digraph, apply Playfair rules (same row, same column, rectangle swap) to produce the ciphertext digraph.

**Example:**

Input: "HELLO"
Output: ciphertext produced by the digraph substitutions using the chosen key (implementation-dependent).

