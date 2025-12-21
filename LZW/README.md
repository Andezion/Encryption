# LZW (Lempel–Ziv–Welch)

*Short description:*

LZW is a lossless data compression algorithm that builds a dictionary of substrings encountered in the input and emits compact codes that reference dictionary entries. It is widely used for simple, fast compression in formats like GIF.

**Implementation:**
- Implementations and tests may be present in C++ subdirectory.

**Algorithm (brief):**

1. Initialize the dictionary with all single-symbol strings.
2. Find the longest sequence present in the dictionary that matches the current input prefix.
3. Output the code for that sequence and add the sequence plus the next symbol to the dictionary.
4. Repeat until the entire input has been encoded.

**Example:**

Input: "TOBEORNOTTOBE$"
Output: a sequence of dictionary codes representing repeated substrings (encoding details depend on implementation).

