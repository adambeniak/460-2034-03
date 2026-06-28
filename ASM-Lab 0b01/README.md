### Homework Description
The goal is to create a 64-bit Assembly (x86-64) application implementing conditional jumps ($Jcc$), mathematical operations, bitwise logic, and indirect memory addressing with a single register. The project manipulates global variables, maps structural logic (loops and conditional branches), and applies a specific assembly instruction subset (`add`, `sub`, `cmp`, `inc`, `dec`, `neg`, `and`, `test`, `or`, `xor`, `not`, `shl`, `shr`).

#### Assembly Tasks Breakdown

1. **`count_ones` (Bit Population Count):**
   * **Goal:** Count the total number of binary `1`s (set bits) within a 32-bit global integer variable `data_x` and save the resulting tally into a global integer `count`.
   * **Logic:** Use bit-shifting (`shr`/`shl`) or mask-testing (`test`/`and`) along with loops and conditional jumps to iterate through the 32 bits of the integer.

2. **`fibb` (Fibonacci Iterative Sequence):**
   * **Goal:** Calculate the $X$-th member of the Fibonacci sequence based on a global variable `iterations`, storing the final value inside a global integer `result_num`.
   * **Logic:** Replicate a `do-while` loop pattern in Assembly using registers to hold transient `prev` and `next` values. Decrement the loop counter using `dec` and branch using conditional jumps (`jnz`/`jcc`) until the count hits zero.

3. **`replace_char` (String Element Replacement):**
   * **Goal:** Iterate through a global null-terminated character array `text` and swap every occurrence of character `from` with character `replace_to`.
   * **Logic:** Apply **indirect pointer addressing** using a single register (e.g., `[rbx]`) to read characters step-by-step. Implement a loop that checks for the string's null terminator (`\0`), performs comparisons (`cmp`), and conditionally overwrites target characters in memory.