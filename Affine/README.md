**Algorithm:** Affine Cipher

*Short description:*
The Affine cipher is a simple monoalphabetic substitution cipher defined by the function E(x) = (a*x + b) mod m, where `m` is the alphabet size and `a` and `b` are keys (with `a` coprime to `m`). Decryption uses the modular inverse of `a`.

**Implementation:**
- Language: C / C++ (see `C` and `C++` folders)
- Key constraints: `gcd(a, m) = 1`

**Usage / API:**
- Provide plaintext and keys `a`, `b`.
- Example: `E('A') = (5*0 + 8) mod 26 = 8 -> 'I'`

**Results and tests:**
_Place for performance numbers, test vectors and correctness notes._

