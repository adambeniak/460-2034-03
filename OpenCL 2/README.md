### Homework Description
The goal is to create a heterogeneous application centered on high-performance graphical processing, utilizing the OpenCL framework to offload image composition tasks from the CPU to a Graphics Processing Unit (GPU). The program will algorithmically generate a complex background matrix, manage smaller transparent objects (RGBA), apply multiple simultaneous geometric and color transformations to different instances of these objects in parallel, and composite the final RGB image.

#### Core Mechanics & Performance Requirements
* **GPU Parallelization via OpenCL:** The assignment focuses on modularizing complex image math into distinct OpenCL compute kernels (`.cl`) to maximize throughput. Operations such as resizing, rotation, mirroring, color modulation, and blending are exceptionally suitable for mapping each output pixel calculation to a standalone compute thread.
* **Incremental Modularization:** The recommended design breaks down the problem into atomic functional subroutines (e.g., separate functions/kernels for sampling, resizing, rotation, mirroring, and drawing) rather than one monolithic rendering routine. This structure facilitates porting the solution to OpenCL kernels.
* **Transformations & Fidelity:** To ensure high visual fidelity, **bilinear interpolation** must be used during all resizing and rotation transformations.

#### Composition Rules
1. **Background Matrix Generation:** The program must algorithmically generate a unique, non-solid RGB background matrix (e.g., gradient, grid, checkerboard, or noise) within a large standard memory allocation (e.g., $1024 \times 768$ pixels).
2. **RGBA Object Handling:** A smaller transparent asset (e.g., $128 \times 128$ pixels) must be managed as an RGBA image. The asset can either be procedurally generated (Variant A: e.g., a glow sphere) or loaded from a PNG with an alpha channel (Variant B).
3. **Multi-Instance Rendering:** At least 5 unique, parameterized instances of the transparent object must be composited into the background scene.
4. **Transformations Stack:** Each of the 5+ instances must apply a unique combination of five base transformations:
   * **Position:** $X, Y$ coordinates.
   * **Scale:** Magnification scalar.
   * **Rotation:** Angle offset.
   * **Color Modulation:** Individual RGB channel gain/attenuation multipliers.
   * **Global Alpha:** Overall transparency scalar.
5. **Variant B Extra Constraint:** If utilizing an external PNG (Variant B), at least one additional transformation must be applied to the stack (e.g., mirroring or color channel swapping).
6. **Alpha Blending:** When inserting an RGBA pixel into the RGB background canvas, proper standard linear alpha blending must be applied to compute the final combined pixel color.