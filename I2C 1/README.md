### Homework Description
The goal is to create an MCU application utilizing the **I2C serial communication bus** to interface with two external peripherals: a **PCF8574 I/O expander** (driving an 8-LED bar graph) and an **Si4735 FM radio receiver module**. The program implements driver abstraction layer patterns (classes/structs) and async hardware button routines for state controls and system telemetry reporting.

#### Peripherals & Hardware Logic

1. **`I2C Expandér PCF8574` (LED Bar Graph Driver):**
   * **Device Addressing:** The I2C slave address pin configurations ($A_0, A_1, A_2$) must evaluate to the student's personal `login % 8` formula result.
   * **Bar Graph Rendering:** Implement an initialization class containing a specialized display method that maps a bounded integer scope ($0$ to $8$) to a contiguous block of illuminated LEDs (e.g., a parameter value of `3` drives the three lowest output pins low/high simultaneously to illuminate three consecutive LEDs).
   * **Verification Loop:** In the primary execution path, cycle infinitely by filling the bar graph from $0 \rightarrow 8$ progressively, followed by dimming it back down from $8 \rightarrow 0$.

2. **`Rádio SI4735` (FM Receiver Architecture):**
   * **Driver Abstraction:** Encapsulate the radio parameters into a dedicated object-oriented class or structured layout (`struct`).
   * **Core Subroutines:** Implement functional methods for frequency channel tuning (frequency steps) and audio amplitude attenuation adjustment (volume level bounds).

#### Controls & Interactive UI Logic
* **Volume Loop Trigger (1 Button):** A single input button cycles through a loop of volume attenuation levels (e.g., minimum to maximum, resetting back to minimum).
* **Manual Tuning Triggers (2 Buttons):** Two individual buttons increment or decrement the active receiving radio frequency (tuning upwards or downwards through the standard FM band spectrum).
* **Automated Seek Scan (Double-Click):** Double-clicking either tuning button initializes an automated signal seek search algorithm in the respective direction (up or down the band) until a stable broadcast signal lock is established.

#### System Telemetry Reporting
* **Serial Link Monitoring:** Stream system operational status structures over a standard hardware Serial Link UART interface. Every channel adjust or button interaction logs updating terminal lines tracing data fields including: **Frequency**, **RSSI** (Received Signal Strength Indicator), and **SNR** (Signal-to-Noise Ratio).