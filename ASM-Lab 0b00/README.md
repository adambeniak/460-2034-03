### Homework Description
The goal is to create a mixed C and 64-bit Assembly (x86-64) application focusing on low-level data movement and type casting using global variables. The assignment explores register sizes, signed/unsigned bit extensions, and memory addressing without using loops or bit-shifting operations. All tasks must adhere to a strict instruction whitelist: `mov`, `movzx`, and `movsx`.

#### Assembly Tasks Breakdown

1. **`task1_extend_temp` (Sign Extension):**
   * **Goal:** Convert 4 temperature samples from signed 8-bit integers (`int8_t` / bytes) to signed 32-bit integers (`int32_t` / double-words) within global arrays `g_temp8` and `g_temp32`.
   * **Constraint:** Must use `movsx` (Move with Sign-Extend) to correctly preserve negative values (e.g., `-128` remains `-128`). Hardcode all 4 operations sequentially without a loop.

2. **`task2_extend_batt` (Zero Extension):**
   * **Goal:** Convert 4 battery percentage values from unsigned 8-bit integers (`uint8_t` / bytes) to unsigned 32-bit integers (`uint32_t` / double-words) within global arrays `g_batt8` and `g_batt32`.
   * **Constraint:** Must use `movzx` (Move with Zero-Extend) to prevent incorrect sign interpretation of unsigned values. Process all 4 values sequentially without loops.

3. **`task3_reverse_timestamp` (Byte-Order Reversal):**
   * **Goal:** Reverse the byte order (Endianness conversion) of a 32-bit timestamp from `g_timestamp` into `g_timestamp_r` (e.g., `0x0A141E28` $\rightarrow$ `0x281E140A`).
   * **Constraint:** Bit-shifts are forbidden. The conversion must be executed strictly by loading individual bytes using `mov` into 8-bit register components and storing them in reversed physical memory positions:
     $$\text{src}[0] \rightarrow \text{dst}[3], \quad \text{src}[1] \rightarrow \text{dst}[2], \quad \text{src}[2] \rightarrow \text{dst}[1], \quad \text{src}[3] \rightarrow \text{dst}[0]$$

4. **`task4_split_device_id` (64-bit Splitting):**
   * **Goal:** Split a 64-bit device ID (`g_device_id`) into two separate 32-bit variables: `g_vendor_id` (upper 32 bits) and `g_serial_id` (lower 32 bits).
   * **Constraint:** Bit-shifts are forbidden. Must use direct memory addressing offset calculations and standard 32-bit `mov` operations.