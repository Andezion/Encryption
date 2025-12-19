# Elliptic Curve Cryptography (ECC)

*Short description:*

This folder contains code related to Elliptic Curve Cryptography (ECC). The implementation is intended as an educational/example collection showing basic ECC concepts, point operations on elliptic curves, and higher-level primitives built on those operations.

**Implementation:** 

- `C++/ecc.h` — primary C++ header exposing ECC routines (point arithmetic, curve parameters, helpers).

**Notes:**

- This code focuses on clarity and education rather than production security. It may not include constant-time operations, side-channel protections, or hardened big-integer routines required for secure deployments.
- For real-world ECC use, rely on vetted libraries such as OpenSSL, libsodium, or dedicated ECC libraries (e.g., libsecp256k1) and follow current cryptographic best practices.


