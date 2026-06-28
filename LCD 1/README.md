### Homework Description
The goal is to create a microcontroller (MCU) application focused on graphical display control (LCD), font rendering, precise geometric shape drawing algorithms, touch coordinate processing, and area-filling logic. The solution should be implemented within the `graph_class.hpp` file.

#### UI Layout & Touch Controls
* **Top Border (Color Palette):** Displays the colored character triggers **R** (Red), **G** (Green), **B** (Blue), and **W** (White) spaced evenly. Touching a letter sets that color as active and visually highlights it (e.g., via a bounding box, inversion, or underlining).
* **Bottom Border (Shape Selection):** Displays two touch-responsive control characters using a large, touch-optimized font size ($16 \times 26$ or $22 \times 36$ pixels):
  * **T**: Draws an equilateral triangle centered on the display screen (suggested side length of 50 pixels). 
  * **O**: Draws an ellipse centered on the display screen (suggested semi-axes of $40 \times 25$ pixels).
* **Canvas Behavior:** Only one active shape needs to be present at a time; drawing a new shape clears the previous one by overwriting it with the background color.

#### Drawing & Interaction Algorithms
1. **Triangle Rendering:** Constructed by connecting three lines using the integer-based **Bresenham's line algorithm**.
2. **Ellipse Rendering:** Formed using an efficient integer midpoint ellipse algorithm or circular coordinate approximations.
3. **Interactive Flood Fill:** If a user touches the screen inside the boundary of the active rendered shape, the program triggers an area-filling routine (such as a recursive **Flood-Fill** or a **Scanline** algorithm) to paint the inner region with the currently selected active color. The shape can be refilled repeatedly with different colors.