### Homework Description
The goal is to create a microcontroller (MCU) programming application that handles hardware peripheral control, specifically managing LED state animations via Pulse Width Modulation (PWM) and processing asynchronous hardware button inputs.

#### Homework Tasks Breakdown

1. **`Postupné rozsvěcení LED` (Progressive LED Fade-In/Out Array):**
   * **Goal:** Program a progressive illumination and dimming animation sequence across an external module containing 8 red LEDs (`LED0` through `LED7`).
   * **Animation Behavior:** * **Fade-In Phase:** LEDs ignite sequentially from `LED0` to `LED7`. Each individual LED must transition from minimum to maximum intensity using a smooth PWM duty-cycle ramp. The subsequent LED only begins its fade-in routine once the preceding LED has fully reached maximum brightness, accumulating lit elements (e.g., `LED0` $\rightarrow$ `LED0`+`LED1` $\rightarrow$ `LED0`+`LED1`+`LED2`).
     * **Fade-Out Phase:** Once all 8 LEDs are fully lit, the entire array reverses the process, sequentially dimming from maximum to minimum brightness in the same orderly fashion using a PWM ramp.
   * **Button Control Mapping:**
     * `P3_21` – Start/Resume the animation.
     * `P3_20` – Stop/Pause the animation at its current frame.
     * `P3_18` – Reset the animation timeline back to the initial state (`LED0` off).

2. **`RGB semafor` (Smooth RGB Traffic Light Transition):**
   * **Goal:** Program a single RGB LED module to simulate a classic traffic light junction sequence utilizing multi-channel PWM duty-cycle mixing to ensure fluid color interpolation.
   * **Animation Behavior:** * The single RGB LED must smoothly cycle through the following color states continuously: **Red $\rightarrow$ Yellow $\rightarrow$ Green $\rightarrow$ Yellow $\rightarrow$ Red**.
     * Color blending must be entirely fluid via PWM channels (e.g., transitioning from Red to Yellow requires holding the Red channel high while progressively scaling up the Green channel intensity).
   * **Button Control Mapping:**
     * `P3_21` – Enable/Start the color transitions loop.
     * `P3_20` – Stop/Freeze the color transition immediately at its current tint.
     * `P3_18` – Induce a temporary 1-second delay/pause on the active animation frame before resuming automatically.