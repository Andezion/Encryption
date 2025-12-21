# ROT13

*Short description:*

ROT13 is a simple substitution cipher that rotates each letter by 13 positions in the Latin alphabet. It is symmetric (encoding and decoding are the same operation) and often used for obfuscation rather than security.

**Implementation:**
- Implementations and tests may be present in C++, C and Rust subdirectories.

**Algorithm (brief):**

1. For each alphabetic character, shift it 13 places forward (wrapping within A–Z or a–z).
2. Non-alphabetic characters are left unchanged.

**Example:**

Input: "hello"
Output: "uryyb"


