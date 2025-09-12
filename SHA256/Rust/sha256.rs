#![no_std]

use core::convert::TryInto;

const H0: [u32; 8] = [
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19,
];

const K: [u32; 64] = [
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4,
    0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe,
    0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f,
    0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc,
    0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116,
    0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7,
    0xc67178f2,
];

#[derive(Clone)]
pub struct Sha256 {
    h: [u32; 8],        
    buffer: [u8; 64],   
    buffer_len: usize,  
    bit_len: u64,       
}

impl Default for Sha256 {
    fn default() -> Self {
        Self::new()
    }
}

impl Sha256 {
    pub const fn new() -> Self {
        Self {
            h: H0,
            buffer: [0u8; 64],
            buffer_len: 0,
            bit_len: 0,
        }
    }

    pub fn with_state(state: [u32; 8]) -> Self {
        Self {
            h: state,
            buffer: [0u8; 64],
            buffer_len: 0,
            bit_len: 0,
        }
    }

    pub fn reset(&mut self) {
        self.h = H0;
        self.buffer = [0u8; 64];
        self.buffer_len = 0;
        self.bit_len = 0;
    }

    fn compress_block(&mut self, block: &[u8; 64]) {
        let mut w = [0u32; 64];

        for i in 0..16 {
            let off = i * 4;
            w[i] = u32::from_be_bytes([block[off], block[off + 1], block[off + 2], block[off + 3]]);
        }
        for i in 16..64 {
            let s0 = w[i - 15].rotate_right(7) ^ w[i - 15].rotate_right(18) ^ (w[i - 15] >> 3);
            let s1 = w[i - 2].rotate_right(17) ^ w[i - 2].rotate_right(19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16]
                .wrapping_add(s0)
                .wrapping_add(w[i - 7])
                .wrapping_add(s1);
        }

        let mut a = self.h[0];
        let mut b = self.h[1];
        let mut c = self.h[2];
        let mut d = self.h[3];
        let mut e = self.h[4];
        let mut f = self.h[5];
        let mut g = self.h[6];
        let mut h = self.h[7];

        for i in 0..64 {
            let S1 = e.rotate_right(6) ^ e.rotate_right(11) ^ e.rotate_right(25);
            let ch = (e & f) ^ ((!e) & g);
            let temp1 = h.wrapping_add(S1)
                .wrapping_add(ch)
                .wrapping_add(K[i])
                .wrapping_add(w[i]);
            let S0 = a.rotate_right(2) ^ a.rotate_right(13) ^ a.rotate_right(22);
            let maj = (a & b) ^ (a & c) ^ (b & c);
            let temp2 = S0.wrapping_add(maj);

            h = g;
            g = f;
            f = e;
            e = d.wrapping_add(temp1);
            d = c;
            c = b;
            b = a;
            a = temp1.wrapping_add(temp2);
        }

        self.h[0] = self.h[0].wrapping_add(a);
        self.h[1] = self.h[1].wrapping_add(b);
        self.h[2] = self.h[2].wrapping_add(c);
        self.h[3] = self.h[3].wrapping_add(d);
        self.h[4] = self.h[4].wrapping_add(e);
        self.h[5] = self.h[5].wrapping_add(f);
        self.h[6] = self.h[6].wrapping_add(g);
        self.h[7] = self.h[7].wrapping_add(h);
    }

    pub fn update(&mut self, mut input: &[u8]) {
        let added_bits = (input.len() as u64).wrapping_mul(8);
        self.bit_len = self.bit_len.wrapping_add(added_bits);

        if self.buffer_len > 0 {
            let need = 64 - self.buffer_len;
            if input.len() >= need {
                self.buffer[self.buffer_len..].copy_from_slice(&input[..need]);
                let block: &[u8; 64] = (&self.buffer).try_into().expect("slice with len 64");
                self.compress_block(block);
                self.buffer_len = 0;
                input = &input[need..];
            } else {
                self.buffer[self.buffer_len..self.buffer_len + input.len()].copy_from_slice(input);
                self.buffer_len += input.len();
                return;
            }
        }

        while input.len() >= 64 {
            let block: &[u8; 64] = (&input[..64]).try_into().expect("slice with len 64");
            self.compress_block(block);
            input = &input[64..];
        }

        if !input.is_empty() {
            self.buffer[..input.len()].copy_from_slice(input);
            self.buffer_len = input.len();
        }
    }

    pub fn finish(mut self) -> [u8; 32] {
        let mut pad = [0u8; 128]; 
        pad[0] = 0x80;

        let pad_len = if self.buffer_len < 56 {
            56 - self.buffer_len
        } else {
            64 + 56 - self.buffer_len
        };

        let total_pad = pad_len + 8;
        let mut to_update = &pad[..total_pad];

        let bit_len_be = self.bit_len.to_be_bytes();
        let off = pad_len;
       
        {
            let mut tail = [0u8; 8];
            tail.copy_from_slice(&bit_len_be);
            
            if pad_len > 0 {
                self.update(&pad[..pad_len]);
            }
            self.update(&tail);
            
            let mut out = [0u8; 32];
            for (i, &word) in self.h.iter().enumerate() {
                let bytes = word.to_be_bytes();
                out[i * 4..i * 4 + 4].copy_from_slice(&bytes);
            }
            return out;
        }
    }

    pub fn digest(data: &[u8]) -> [u8; 32] {
        let mut ctx = Self::new();
        ctx.update(data);
        ctx.finish()
    }

    pub fn state(&self) -> [u32; 8] {
        self.h
    }
}
