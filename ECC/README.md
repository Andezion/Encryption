
# Elliptic Curve Cryptography (ECC)

Overview
--------
This folder contains code related to Elliptic Curve Cryptography (ECC). The implementation is intended as an educational/example collection showing basic ECC concepts, point operations on elliptic curves, and higher-level primitives built on those operations.

Files
-----
- `C++/ecc.h` — primary C++ header exposing ECC routines (point arithmetic, curve parameters, helpers).

Build & Usage
-------------
There is no top-level build script in this folder. To compile an example that uses the provided header, include `C++/ecc.h` in your source and compile with a standard C++ compiler. Example:

```bash
g++ -std=c++17 -I. your_example.cpp -o ecc_example
./ecc_example
```

Notes
-----
- This code focuses on clarity and education rather than production security. It may not include constant-time operations, side-channel protections, or hardened big-integer routines required for secure deployments.
- For real-world ECC use, rely on vetted libraries such as OpenSSL, libsodium, or dedicated ECC libraries (e.g., libsecp256k1) and follow current cryptographic best practices.

References
----------
- Hankerson, D., Menezes, A., Vanstone, S. (2004). Guide to Elliptic Curve Cryptography.
- Standards and curve definitions: SEC 2, NIST curves, and modern alternatives like Curve25519/Ed25519.

License & Attribution
---------------------
See the repository root for the project license and contribution information.

