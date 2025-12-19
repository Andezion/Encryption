
# Hill Cipher

Overview
--------
This folder contains an implementation of the Hill cipher — a classical polygraphic substitution cipher based on linear algebra over modular arithmetic. The implementation includes helper functions for matrix operations, encryption and decryption using a square key matrix.

Files
-----
- `C++/Hill.h` — header with Hill cipher functions and matrix utilities.
- `C++/Hill_Test.cpp` — example/test harness demonstrating usage.

Build & Usage
-------------
Compile the test or integrate the header into your project. Example:

```bash
g++ -std=c++17 C++/Hill_Test.cpp -I. -o hill_test
./hill_test
```

Notes
-----
- Keys must be invertible modulo the alphabet size (typically 26) for decryption to be possible.
- The implementation assumes uppercase alphabetic input unless adapted.

References
----------
- Hill, L. S. (1929). Cryptography in an Algebraic Alphabet.

