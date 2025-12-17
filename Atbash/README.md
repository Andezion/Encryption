# Atbash

*Short description:*

Atbash is a simple substitution cipher that maps each letter to its "mirror" in the alphabet (for example A↔Z, B↔Y, etc.). It is primarily used for illustrative purposes and simple obfuscation.

**Implementation:**

- Implementations are provided in C and C++ (see `C/` and `C++/`).
- Tests and example usage files are included to verify correctness.
- `C/Atbash.c`, `C/AtbashTest.cpp` — C implementation and tests.
- `C++/Atbash.h`, `C++/AtbashTest.cpp` — C++ header and tests.

**Algorithm (brief):**

1. For each input letter in the Latin alphabet determine its position (A=0..Z=25).
2. Compute the mirrored position: mirrored = 25 - pos.
3. Replace the letter with the letter at the mirrored position, preserving case.
4. Non-letter characters are preserved unchanged.

**Example:**

Input: "Hello, World!"
Output: "Svool, Dliow!"

