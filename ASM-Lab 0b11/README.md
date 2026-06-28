### Homework Description
The goal is to create a 64-bit Assembly (x86-64) application implementing nested loops, signed multiplication (`imul`), conditional moves (`cmovxx`), bitwise manipulations, and function call conventions (including nested function calls tracking the stack frame/registers).

#### Assembly Tasks Breakdown

1. **`faktorialy` (Array Factorials with Overflow Check):**
   * **Signature:** `void faktorialy(long *pole, int N);`
   * **Goal:** Iterate through an array of $N$ 64-bit signed integers (`long`), compute the factorial of each element using an iterative loop, and overwrite the original element with its factorial value.
   * **Constraint:** Must handle overflows using `imul`. If a multiplication triggers an overflow flag ($OF$), catch it and store `0` as the result for that element.

2. **`bubble` (Bubble Sort with Conditional Moves):**
   * **Signature:** `void bubble(int *pole, int N, int asc_desc);`
   * **Goal:** Sort an array of $N$ 32-bit integers using the Bubble Sort algorithm. The sorting direction depends on `asc_desc` (ascending if $>0$, descending if $\le 0$).
   * **Constraint:** Requires two nested loops. The actual element swapping logic must be performed **without conditional jump instructions ($Jcc$)**, utilizing `cmovxx` (Conditional Move) to conditionally select and swap the values in registers before storing them back to memory.

3. **Missing Alphabet Characters (Bitwise Mask Tracking):**
   * **Goal:** Determine how many lowercase letters (`'a'`..`'z'`) are missing from a given string. The logic must be split into two distinct assembly functions:
     * **Function 1 (Mask Creation):** Start with an initial 32-bit bitmask set to `0x03FFFFFF` (representing 26 active bits for the alphabet). Loop through the string, and for each character, clear its corresponding bit using bitwise operations equivalent to: $\text{mask} = \text{mask} \ \& \ \sim(1 \ll (\text{char} - \text{'a'}))$.
     * **Function 2 (Bit Tally):** Take the final bitmask from Function 1 and count the remaining `1`s. The resulting total represents the number of missing letters.

4. **`sum_abs` (Sum of Absolute Values via Function Calling):**
   * **Signatures:** `long abs_val(long x);` and `long sum_abs(long *pole, int N);`
   * **Goal:** Compute the sum of the absolute values of all elements in an array of $N$ 64-bit integers.
   * **Constraint:** The parent function `sum_abs` **must explicitly invoke** the child function `abs_val` for every element. Proper preservation of volatile/non-volatile registers across function boundaries must be implemented.