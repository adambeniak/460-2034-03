### Homework Description
The goal is to create a heterogeneous application that utilizes the OpenCL framework to offload image processing algorithms to a Graphics Processing Unit (GPU). The program will take an input image (in standard RGB or RGBA format) and perform four distinct image manipulation operations in parallel.

#### Core Constraints & Requirements
* **Parallel Architecture:** Each mathematical image task must be implemented as a completely standalone OpenCL compute kernel (`.cl`). 
* **Granularity:** The Work-Item architecture must map precisely one pixel calculation onto one compute thread. 
* **Edge Handling:** Kernels must handle matrix boundary boundary checks (width/height bounds).
* **Saturation Arithmetic:** Resulting color components ($R$, $G$, $B$) must be clamped/saturated within the valid 8-bit integer range of $[0, 255]$ during any floating-point transformation to prevent overflow or underflow artifacts.

#### OpenCL Kernel Specifications

1. **`grayscale.cl` (Luminance Conversion):**
   * **Goal:** Convert a full-color input pixel into its equivalent grayscale intensity.
   * **Formula:** Apply the standard standard perceptual luminance weighting formula:
     $$\text{gray} = 0.299 \times R + 0.587 \times G + 0.114 \times B$$
   * **Output:** Overwrite the target pixel's Red, Green, and Blue channels identically with the calculated floating-point `gray` value.

2. **`negative.cl` (Color Inversion):**
   * **Goal:** Create a visual negative of the input image.
   * **Formula:** Subtract each individual color component of a pixel from the maximum value:
     $$R_{\text{new}} = 255 - R_{\text{old}}$$
     $$G_{\text{new}} = 255 - G_{\text{old}}$$
     $$B_{\text{new}} = 255 - B_{\text{old}}$$

3. **`contrast.cl` (Contrast Adjustment):**
   * **Goal:** Enhance or reduce image contrast based on a global floating-point constant scalar (`contrast`, e.g., $1.5$).
   * **Formula:** Shift each color component relative to the intensity midpoint ($128$):
     $$\text{new\_value} = 128 + \text{contrast} \times (\text{old\_value} - 128)$$
   * **Constraint:** Apply mathematical clamp operations to guarantee the result falls in $[0, 255]$.

4. **`mirror_horizontal.cl` (Vertical Axis Reflection):**
   * **Goal:** Reflect the input image horizontally across the vertical midline (left-to-right swap).
   * **Logic:** Calculate a mirroring mapping for each input coordinate $(x, y)$ onto the output coordinate matrix:
     $$\text{new\_x} = (\text{ImageWidth} - 1) - \text{old\_x}$$
     $$\text{new\_y} = \text{old\_y}$$