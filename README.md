# Encryption
Experiments with encryption, information compression and algorithms

## Content
* [ANS](#ans) - ANS-based compression and coding algorithms.
  * [C++ tANS](https://github.com/Andezion/Encryption/blob/main/ANS/C%2B%2B/tANS.h) - Add simulation!
  * [C++ uANS](https://github.com/Andezion/Encryption/blob/main/ANS/C%2B%2B/uANS.h) - Add test!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/ANS/Rust/tANS.rs) - Add tests!
  * [Rust](#rust_uans) - Add implementation and tests!
  * [C tANS](https://github.com/Andezion/Encryption/blob/main/ANS/%D0%A1/tans_embedded.c) - Add tests!
* [Affine](#affine) - is a special case of the more general monoalphabetic substitution cipher.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Affine/C%2B%2B/Affine.h) - Add tests!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Affine/Rust/affine.rs) - Add tests!
* [Atbash](#atbash) - is a monoalphabetic substitution cipher originally used to encrypt the Hebrew alphabet.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Atbash/C%2B%2B/Atbash.h) - Add tests!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Atbash/Rust/atbash.rs) - Add tests!
* [Caesar](#caesar) - is one of the simplest and most widely known encryption techniques.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Caesar/C%2B%2B/Caesar.h) - Add tests!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Caesar/Rust/main.rs) - Add implementation and tests!
* [ROT13](#rot_13) - is a simple letter substitution cipher that replaces a letter with the 13th letter after it in the Latin alphabet.
  * [C++](https://github.com/Andezion/Encryption/blob/main/ROT13/C%2B%2B/ROT13.h) - Add tests!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/ROT13/Rust/rot13.rs) - Add tests!
  * [Zig](https://github.com/Andezion/Encryption/blob/main/ROT13/Zig/rot13.zig) - Add tests!
* [ROT47](#rot_47) - ROT47 is a derivative of ROT13 which, in addition to scrambling the basic letters, treats numbers and common symbols.
  * [C++](https://github.com/Andezion/Encryption/blob/main/ROT47/C%2B%2B/ROT47.h) - Add tests! 
  * [Rust](https://github.com/Andezion/Encryption/blob/main/ROT47/Rust/rot47.rs) - Add tests!
  * [Zig](https://github.com/Andezion/Encryption/blob/main/ROT47/Zig/main.zig) - Add tests!
* [Vigenere](#vigenere) - Each letter is encrypted with a Caesar cipher shifted by the corresponding key letter.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Vigenere/C%2B%2B/Vigenere.h)  - Add tests!
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Vigenere/Rust/vigenere.rs) - Add tests!
 
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

## ROT13

---

## ROT47

---

## Vigenere

---
