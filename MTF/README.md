# Move-to-Front (MTF) Transform

*Short description:*

The move-to-front transform is a reversible sequence transform that maintains an ordered list of symbols and outputs the index of each symbol while moving that symbol to the front. MTF is often used as a preprocessing step before entropy coding to exploit locality.

**Implementation:**
- Implementations and tests may be present in C++ and Rust subdirectories.

**Algorithm (brief):**

1. Initialize an ordered list of possible symbols (e.g., the alphabet or byte values).
2. For each input symbol, output its index in the list and move the symbol to the front.
3. Decoding reverses the process by using the indices to look up and move symbols back to the front.

**Example:**

Input: "banana"
Output: a sequence of indices where repeated letters yield small numbers (implementation-dependent exact indices).

