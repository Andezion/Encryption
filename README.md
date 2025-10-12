# Encryption
Experiments with encryption, information compression and algorithms

## Content
* [ANS](#ans) - ANS-based compression and coding algorithms.
  * [C++ tANS](https://github.com/Andezion/Encryption/blob/main/ANS/C%2B%2B/tANS.h) - Add simulation!
  * [C++ uANS](https://github.com/Andezion/Encryption/blob/main/ANS/C%2B%2B/uANS.h) - Add simulation and test!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/ANS/Rust/tANS.rs) - Add simulation!
  * [Rust](#rust_uans) - Add implementation and tests and simulation!
  * [C tANS](https://github.com/Andezion/Encryption/blob/main/ANS/%D0%A1/tans_embedded.c) - Add tests!
  * [C uANS](#c_uans) - Add implementation and tests and simulation!
  * [Zig tANS](#zig_tans) - Add implementation and tests and simulation!
  * [Zig uANS](#zig_uans) - Add implementation and tests and simulation!
* [Affine](#affine) - is a special case of the more general monoalphabetic substitution cipher.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Affine/C%2B%2B/Affine.h) - Add tests and simulation!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Affine/Rust/affine.rs) - Add simulation!
  * [C](#c_affine) - Add implementation and tests and simulation!
  * [Zig](zig_affine) - Add implementation and tests and simulation!
* [Atbash](#atbash) - is a monoalphabetic substitution cipher originally used to encrypt the Hebrew alphabet.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Atbash/C%2B%2B/Atbash.h) - Add tests and simulation!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Atbash/Rust/atbash.rs) - Add tests and simulation!
  * [C](#c_atbash) - Add implementation and tests and simulation!
  * [Zig](#zig_atbash) - Add implementation and tests and simulation!
* [BWT](#bwt)
* [Caesar](#caesar) - is one of the simplest and most widely known encryption techniques.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Caesar/C%2B%2B/Caesar.h) - Add tests and simulation!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Caesar/Rust/main.rs) - Add implementation and tests and simulation!
  * [C](#c_caesar) - Add implementation and tests and simulation!
  * [Zig](#zig_caesar) - Add implementation and tests and simulation!
* [ECC](#ecc)
* [Hill](#hill) - in classical cryptography, the Hill cipher is a polygraphic substitution cipher based on linear algebra.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Hill/C%2B%2B/Hill.h) - Add tests and simulation!
  * [Rust](#rust_hill) - Add implementation and tests and simulation!
  * [C](#c_hill) - Add implementation and tests and simulation!
  * [Zig](#zig_hill) - Add implementation and tests and simulation!
* [Huffman](#huffman)
* [Morse](#morse) - is a telecommunications method which encodes text characters as standardized sequences of two different signal durations.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Morse/C%2B%2B/Morse.h) - Add test and simulation!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Morse/Rust/morse.rs) - Add test and simulation!
  * [C](#c_morse) - Add implementation and tests and simulation!
  * [Zig](#zig_morse) - Add implementation and tests and simulation!
* [Playfair](#playfair) - is a manual symmetric encryption technique and was the first literal digram substitution cipher.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Playfair/C%2B%2B/Playfair.h) - Add test and simulation!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Playfair/Rust/playfair.rs) - Add test and simulation!
  * [C](#c_playfair) - Add implementation and tests and simulation!
  * [Zig](#zig_playfair) - Add implementation and tests and simulation!
* [Polybius](#polybius) - is used for fractionating plaintext characters so that they can be represented by a smaller set of symbols.
  * [C++ Polybius type 1](https://github.com/Andezion/Encryption/blob/main/Polybius/C%2B%2B/Polybius_method1.h) - Add test and simulation!
  * [C++ Polybius type 2](https://github.com/Andezion/Encryption/blob/main/Polybius/C%2B%2B/Polybius_method2.h) - Add test and simulation!
  * [C++ Polybius type 3](https://github.com/Andezion/Encryption/blob/main/Polybius/C%2B%2B/Polybius_method3.h) - Add test and simulation!
  * [C++ Polybius type 4](https://github.com/Andezion/Encryption/blob/main/Polybius/C%2B%2B/Polybuis_method4.h) - Add test and simulation!
  * [Rust Polybius type 1](https://github.com/Andezion/Encryption/blob/main/Polybius/Rust/polybius_method1.rs) - Add test and simulation!
  * [Rust Polybius type 2](#rust_polybius_type_2) - Add implementation and tests and simulation!
  * [Rust Polybius type 3](#rust_polybius_type_3) - Add implementation and tests and simulation!
  * [Rust Polybius type 4](#rust_polybius_type_4) - Add implementation and tests and simulation!
  * [C Polybius type 1](#c_polybius_type_1) - Add implementation and tests and simulation!
  * [C Polybius type 2](#c_polybius_type_2) - Add implementation and tests and simulation!
  * [C Polybius type 3](#c_polybius_type_3) - Add implementation and tests and simulation!
  * [C Polybius type 4](#c_polybius_type_4) - Add implementation and tests and simulation!
  * [Zig Polybius type 1](#zig_polybius_type_1) - Add implementation and tests and simulation!
  * [Zig Polybius type 2](#zig_polybius_type_2) - Add implementation and tests and simulation!
  * [Zig Polybius type 3](#zig_polybius_type_3) - Add implementation and tests and simulation!
  * [Zig Polybius type 4](#zig_polybius_type_4) - Add implementation and tests and simulation!
* [ROT13](#rot_13) - is a simple letter substitution cipher that replaces a letter with the 13th letter after it in the Latin alphabet.
  * [C++](https://github.com/Andezion/Encryption/blob/main/ROT13/C%2B%2B/ROT13.h) - Add tests and simulation!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/ROT13/Rust/rot13.rs) - Add tests and simulation!
  * [C](#c_rot_13) - Add implementation and tests and simulation!
  * [Zig](https://github.com/Andezion/Encryption/blob/main/ROT13/Zig/rot13.zig) - Add tests and simulation!
* [ROT47](#rot_47) - ROT47 is a derivative of ROT13 which, in addition to scrambling the basic letters, treats numbers and common symbols.
  * [C++](https://github.com/Andezion/Encryption/blob/main/ROT47/C%2B%2B/ROT47.h) - Add tests and simulation! 
  * [Rust](https://github.com/Andezion/Encryption/blob/main/ROT47/Rust/rot47.rs) - Add tests and simulation!
  * [C](#c_rot_47) - Add implementation and tests and simulation!
  * [Zig](https://github.com/Andezion/Encryption/blob/main/ROT47/Zig/main.zig) - Add tests and simulation!
* [Scytale](#scytale) - is a tool used to perform a transposition cipher.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Scytale/C%2B%2B/Scytale.h) - Add tests and simulation! 
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Scytale/Rust/scytale.rs) - Add tests and simulation!
  * [C](#c_scytale) - Add implementation and tests and simulation!
  * [Zig](#zig_scytale) - Add implementation and tests and simulation!
* [Substitution](#substitution) - is a method of encrypting that creates the ciphertext by replacing units of the plaintext in a defined manner
  * [C++](https://github.com/Andezion/Encryption/blob/main/Substitution/C%2B%2B/Substitution.h) - Add tests and simulation! 
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Substitution/Rust/substitution.rs) - Add tests and simulation!
  * [C](#c_substitution) - Add implementation and tests and simulation!
  * [Zig](#zig_substitution) - Add implementation and tests and simulation!
* [Transposition](#transposition) - is a method of encryption which scrambles the positions of characters without changing the characters themselves.
  * [C++ double transposition](https://github.com/Andezion/Encryption/blob/main/Transposition/C%2B%2B/Double_Transposition.h) - Add tests and simulation!
  * [C++ transposition](https://github.com/Andezion/Encryption/blob/main/Transposition/C%2B%2B/Transposition.h) - Add tests and simulation!
  * [C++ vertical transposition](https://github.com/Andezion/Encryption/blob/main/Transposition/C%2B%2B/Vertical_Transposition.h) - Add tests and simulation!
  * [Rust double transposition](#rust_doub_tran) - Add implementation and tests and simulation!
  * [Rust transposition](https://github.com/Andezion/Encryption/blob/main/Transposition/Rust/transposition.rs) - Add tests and simulation!
  * [Rust vertical transposition](https://github.com/Andezion/Encryption/blob/main/Transposition/Rust/vertical_transposition) - Add tests and simulation!
  * [C double transposition](#c_doub_tran) - Add implementation and tests and simulation!
  * [C transposition](#c_tran) - Add implementation and tests and simulation!
  * [C vertical transposition](#c_ver_tran) - Add implementation and tests and simulation!
  * [Zig double transposition](#zig_doub_tran) - Add implementation and tests and simulation!
  * [Zig transposition](#zig_tran) - Add implementation and tests and simulation!
  * [Zig vertical transposition](#zig_ver_tran) - Add implementation and tests and simulation!
* [Vigenere](#vigenere) - Each letter is encrypted with a Caesar cipher shifted by the corresponding key letter.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Vigenere/C%2B%2B/Vigenere.h)  - Add tests and simulation!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Vigenere/Rust/vigenere.rs) - Add tests and simulation!
  * [C](#c_vigenere) - Add implementation and tests and simulation!
  * [Zig](#zig_vigenere) - Add implementation and tests and simulation!
 
---

## ANS
Asymmetric Numeral Systems (ANS) is an entropy coding method that combines high compression like arithmetic coding with performance close to bitwise algorithms (e.g., Huffman).
In ANS, a single number stores the entire state, and each symbol is encoded by transitioning from one state to another.
- tANS (table-based ANS) - uses a pre-computed table of transitions for fast encoding and decoding.
- uANS (uniform ANS) - uses mathematical formulae without tables, providing compactness and flexibility.

I will now show you the results of my implementation of the tANS algorithm. If you would like to see the tests and test materials, you can do so [here](https://github.com/Andezion/Encryption/blob/main/ANS/C%2B%2B/tANS_tests.cpp). You can read the code [here](https://github.com/Andezion/Encryption/blob/main/ANS/C%2B%2B/tANS.h), and here are the test results:
```
<--- Constructor Error Tests --->
Test 1.1 (Constructor) Empty labeling string: PASSED (Caught expected exception: Labeling cannot be empty)
Test 1.2 (Constructor) Very large labeling string: PASSED (Caught expected exception: Labeling too large)
Test 1.3 (Constructor) Single char labeling: PASSED

<--- Tests for encode --->
Test 2.1 (Encode) Empty message: PASSED (Caught expected exception: Message cannot be empty)
Encoding message of size: 3
Test 2.2 (Encode) Message with unknown symbol: PASSED (Caught expected exception: Last symbol not found in labeling)
Encoding message of size: 4
Test 2.3 (Encode) Message with unknown last symbol: PASSED (Caught expected exception: Last symbol not found in labeling
)
Encoding message of size: 3
Test 2.4 (Encode) initial_state out of bounds: PASSED (Caught expected exception: Initial state out of bounds for symbol
)

--- Tests for decode ---
Encoding message of size: 3
Test 3.1 (Decode) Invalid char in bitstream: PASSED (Caught expected exception: Invalid character in bitstream)
Test 3.2 (Decode) State underflow: PASSED (Caught expected exception: State underflow during decoding)
Test 3.3 (Decode) Decoding table index out of bounds (high state): PASSED (Caught expected exception: Decoding table ind
ex out of bounds)
Test 3.4 (Decode) Max iterations reached: PASSED (Caught expected exception: Maximum iterations reached during decoding)


<--- Tests of uncomfortable data --->
Encoding message of size: 1
Test 4.1 Single char message: correct! Took: 0.0001859 seconds.
Encoding message of size: 3
Test 4.2 Repeated single char message: correct! Took: 0.0001457 seconds.
Encoding message of size: 1
Test 4.3 Single unique char labeling: correct! Took: 0.0001478 seconds.
Encoding message of size: 3
Test 4.4 Non-zero initial_state: correct! Took: 0.0001428 seconds.
Encoding message of size: 3
Test 4.5 Non-zero initial_state (another): correct! Took: 0.0001574 seconds.
Encoding message of size: 1
Test 4.6 Last symbol is only in labeling: correct! Took: 0.0001418 seconds.

<--- Tests with real data --->
Encoding message of size: 13
Test 5.1 Full byte alphabet (small): correct! Took: 0.0552398 seconds.
Encoding message of size: 238
Test 5.2 Full byte alphabet (medium): correct! Took: 0.0531922 seconds.
Encoding message of size: 3406
Encoding message of size: 3406
Test 5.3 Large file content (example.txt): correct! Took: 0.0539561 seconds.
Encoding message of size: 10583
Encoding message of size: 10583
Test 5.4 Large file content (example1.txt): correct! Took: 0.0583809 seconds.
Encoding message of size: 10000
Test 5.5 Low entropy message: correct! Took: 0.0582758 seconds.
Encoding message of size: 10000
Test 5.6 High entropy message: correct! Took: 0.0640273 seconds.

All tests finished.
```

---

## Affine 

---

## Atbash

---

## Caesar

---

## Hill

---

## Morse

---

## Playfair

---

## Polybius

---

## ROT13

---

## ROT47

---

## Scytale

---

## Substitution

---

## Transposition

---

## Vigenere

---

## HUFFMAN
