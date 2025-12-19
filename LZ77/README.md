
# LZ77 Compression

Overview
--------
This folder contains a simple implementation of the LZ77 lossless compression algorithm. LZ77 works by replacing repeated occurrences of data with references to a single copy existing earlier in the data stream.

Files
-----
- `C++/LZ77.h` — header with LZ77 encoder/decoder logic.
- `C++/LZ77_Test.cpp` — example/test harness to demonstrate compression and decompression.

Build & Usage
-------------
Compile the example or include the header in your project. Example:

```bash
g++ -std=c++17 C++/LZ77_Test.cpp -I. -o lz77_test
./lz77_test
```

Notes
-----
- This implementation is intended for educational purposes; production-quality LZ77 implementations use optimized sliding windows and efficient bit/byte packing.

References
----------
- Ziv, J., & Lempel, A. (1977). A universal algorithm for sequential data compression.

