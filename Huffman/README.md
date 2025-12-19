
# Huffman Coding

Overview
--------
This folder contains an implementation of Huffman coding — an optimal prefix code algorithm used for lossless compression. The code implements frequency analysis, Huffman tree construction, and encode/decode routines.

Files
-----
- `C++/huffman.h` — header with Huffman encoder/decoder interfaces.
- `C++/Huffman_Test.cpp` — example/test demonstrating usage and basic tests.

Build & Usage
-------------
Compile the test or include the header and corresponding sources in your project. Example:

```bash
g++ -std=c++17 C++/Huffman_Test.cpp -I. -o huffman_test
./huffman_test
```

Notes
-----
- The implementation focuses on teaching and demonstration; for production use consider robust bitstream handling and error checking.

References
----------
- David A. Huffman, "A Method for the Construction of Minimum-Redundancy Codes", 1952.

