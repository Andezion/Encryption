# SHA-256

*Short description:*

SHA-256 is a cryptographic hash function that produces a 256-bit (32-byte) digest. It is widely used for integrity verification, digital signatures, and password hashing constructions (with salt and work factors).

**Implementation:**
- Reference implementations or wrappers may be present in C and Rust subdirectories.

**Algorithm (brief):**

1. Preprocess the input with padding and length encoding.
2. Process the input in 512-bit blocks through the SHA-256 compression function, updating an internal 256-bit state.
3. Output the final 256-bit state as a hex digest.

**Example:**

Input: "hello"
Output: 256-bit hex digest — for example SHA256("hello") = 2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824


