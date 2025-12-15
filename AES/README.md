**Algorithm:** AES (Advanced Encryption Standard)

*Short description:*
AES is a symmetric block cipher that operates on 128-bit blocks and supports key sizes of 128, 192, or 256 bits. It transforms state using the round functions: `SubBytes`, `ShiftRows`, `MixColumns` and `AddRoundKey`.

**Implementation:**
- Languages: C and C++ (see `C` and `C++` subfolders).
- Supported key sizes: 128 / 192 / 256 bits.
- Notes: ensure correct byte-order and padding when testing with external vectors.

**Usage / API:**
- Provide a 128-bit input block and key; functions support ECB/CBC modes (if implemented).
- Example (conceptual): `encrypt_block(state, round_keys)`

**Results and tests:**
_Placeholder for:_
- Performance benchmarks (throughput, latency)
- Test vectors (NIST vectors, custom tests)
- Memory and portability notes

**References:**
- FIPS 197 — Specification for the Advanced Encryption Standard

