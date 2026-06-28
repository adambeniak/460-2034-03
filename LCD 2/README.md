### Homework Description
The goal is to create a 2D catch-em-all arcade game (e.g., a "Catch the Falling Object" clone) for an MCU platform with a touch-responsive LCD display. The assignment focuses on asynchronous touch coordinate tracking, fundamental collision detection, game loop architecture, string formatting, and efficient graphical rendering techniques.

#### Core Mechanics & Game Loop
1. **Player Paddle:** Positioned at the bottom boundary of the screen, restricted to lateral horizontal movement ($X$-axis). It updates its tracking position dynamically based on user touch coordinates while preventing clipping past the screen borders.
2. **Falling Object:** Spawns at a random $X$-coordinate along the top edge of the LCD screen and steps downwards along the $Y$-axis by a fixed pixel velocity increments each frame. It can be rendered as a circle, square, or text character.
3. **Collision & Bound Logic:** * **Catch:** A hit is recorded if the object's bottom boundary matches the paddle's vertical boundary and falls within its horizontal $X$-span. This increments the score counter, resets the object to the top row, and randomizes its next $X$-start.
   * **Miss:** If the object drops past the paddle, it seamlessly resets back to the top of the screen without penalizing or resetting the current score.
4. **UI Elements:** Continually renders a dynamic text field tracking the user's score (e.g., `Score: 5`). The text field must overwrite cleanly without flickering.

#### Critical Rendering Optimization Constraint
* **Incremental Overwriting:** To circumvent performance bottlenecks, **full-screen redraw commands are strictly forbidden**. 
* The system must implement explicit transactional rendering pairs: erase the object/paddle at its previous coordinates by drawing a matching bounding box tinted to the background color (`erase_object()`, `erase_player()`), modify the data coordinate matrices, and then draw the shape on its brand new coordinate offsets (`draw_object()`, `draw_player()`).