Cardan Grille (Cardan Cipher)
==============================

Overview
--------
The Cardan grille is a transposition cipher that uses a perforated mask (the grille) placed over a blank grid. Plaintext is written into the visible cells of the grid; rotating or moving the grille fills the remaining cells. The result is read in a fixed order to produce the ciphertext.

Contents
--------
This folder contains a C++ header with an implementation for a one-table Cardan grille:

- C++: CardanOneTable.h

Usage
-----
Include the header in your C++ project and use the provided class or functions to create a grille, place plaintext, and produce ciphertext (or reverse the process for decryption). The header implements the core operations for a single-table (one-mask) Cardan grille.

Notes
-----
- The Cardan grille is a classical, pedagogical cipher and is not secure for modern use.
- Check the header for API details and example usage patterns.

