
# Chessboard Cipher

Overview
--------
This folder contains an implementation of the classical Chessboard/Straddling Checkerboard-style substitution used for compact numeric/text encoding. It demonstrates the algorithmic idea and provides header/source files to integrate into C/C++ projects.

Files
-----
- `C++/Chessboard.h` — primary C++ header for the cipher implementation.

Build & Usage
-------------
There are no build scripts in this folder. To compile and run tests or example code that uses the header, include `C++/Chessboard.h` in your project and compile with a standard C++ compiler:

```bash
g++ -std=c++17 -I. your_test.cpp -o chessboard_test
./chessboard_test
```

Notes
-----
- The header is written to be integrated into larger programs — adapt the example above to your project's build system.
- See referenced articles on the straddling checkerboard for background and use cases.

References
----------
- Kirsch, historical references on straddling checkerboard and classical ciphers.

