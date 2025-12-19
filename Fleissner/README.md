
# Fleissner Grille Cipher

Overview
--------
This folder contains an implementation of the Fleissner (Fleissner grille) transposition cipher. The Fleissner grille uses a perforated mask (grille) rotated over a text grid to determine the order of writing or reading characters.

Files
-----
- `C++/FleissnerGrille.h` — C++ header with Fleissner grille routines and helpers.

Build & Usage
-------------
Include the header in your C++ project or compile a test program that uses it. Example compilation:

```bash
g++ -std=c++17 -I. your_test.cpp -o fleissner_test
./fleissner_test
```

Notes
-----
- The implementation expects a grille definition and plaintext length compatible with the grille rotations.
- Adapt the included API to your input format (strings, files, or byte buffers).

References
----------
- Historical descriptions of the Fleissner grille and examples of rotations and grille patterns.

