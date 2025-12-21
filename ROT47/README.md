# ROT47

*Short description:*

ROT47 is a simple substitution cipher that rotates the 94 printable ASCII characters (from '!' to '~') by 47 positions. Like ROT13, it is symmetric but operates over a larger character set.

**Implementation:**
- Implementations and tests may be present in C and C++ subdirectories.

**Algorithm (brief):**

1. For each printable ASCII character between '!' and '~', add 47 modulo 94 to its codepoint.
2. Characters outside the printable range are left unchanged.

**Example:**

Input: "Hello, World!"
Output: transformed text by rotating printable ASCII characters by 47 positions (implementation-dependent exact output).


