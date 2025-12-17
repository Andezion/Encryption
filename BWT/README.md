# Burrows–Wheeler Transform (BWT)

*Short description:*

The Burrows–Wheeler Transform (BWT) is a permutation of a string that groups identical characters together. BWT is commonly used in data compression (for example in bzip2 and FM-indexes) because the transformed output is more amenable to entropy encoding.

**Implementation:** 
- Implementations and tests exist in Rust and C++ (see the `Rust/` and `C++/` directories).
- `Rust/BWT.rs`, `Rust/BWT_tests.rs` — Rust implementation and tests.
- `C++/BWT.h` — C++ header (implementation files may be present alongside tests).

**Algorithm (brief):**

1. Construct all cyclic rotations of the input string s (all shifts s[i..] + s[..i]).
2. Sort these rotations lexicographically.
3. The BWT(s) is the sequence of characters that precede each sorted rotation (commonly the last column of the rotation matrix).

**Example:**

Input: "banana$" (a unique terminator `$` that is lexicographically smaller than other characters)
Sorted cyclic rotations:

- $banana
- a$banan
- ana$ban
- anana$b
- banana$
- na$bana
- nana$ba

Last column (BWT): `annb$aa`