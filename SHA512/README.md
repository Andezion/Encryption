# SHA-512

*Short description:*

SHA-512 is a member of the SHA-2 family that produces a 512-bit (64-byte) cryptographic digest. It is used where larger digests are desired for higher collision resistance.

**Implementation:**
- Implementations may be present in C++ or Rust subdirectories.

**Algorithm (brief):**

1. Preprocess the input with padding and length encoding appropriate for SHA-512.
2. Process the input in 1024-bit blocks through the SHA-512 compression function, updating a 512-bit internal state.
3. Output the final 512-bit state as a hex digest.

**Example:**

Input: "hello"
Output: 512-bit hex digest (implementation-dependent formatting). Example outputs are standard SHA-512 hex digests.


