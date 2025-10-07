This rendering portion of this program is adapted from my code within other GitHub repositories. The adaptations mainly involve passing struct variables into rendering functions instead of using magic numbers.

Gravity is calculated toward the bottom of the window using a discretized form of Newton’s Second Law of Motion, explained below:

### Gravity Update Equation

    ball->vy += GRAVITY * 100 * deltaTime;

This line updates the vertical velocity (vy) of the object using a simplified form of Newton’s Second Law of Motion.

Formula origin:
    
    v_y(t) = v_{y0} + a * t

In a game or simulation, we approximate this per frame using:
    
    v_y += a * Δt

**Variable Breakdown**

| Variable     | Meaning                   | Notes                                      |
|---------------|---------------------------|--------------------------------------------|
| `vy`          | Vertical velocity          | Pixels per second (in screen space)        |
| `GRAVITY`     | Acceleration due to gravity | Typically 9.8 m/s²                         |
| `deltaTime`   | Time since the last frame  | Measured in seconds                        |
| `* 100`       | Scale factor               | Converts meters → pixels for visual scaling |

Each frame, gravity increases the object’s downward velocity by an amount proportional to the elapsed time (`deltaTime`). The `* 100` scaling makes the effect visible on screen, since real-world gravity is too subtle at pixel scale.

### Collision Detection

For a great resource on high-level collision detection, see:  
https://www.jeffreythompson.org/collision-detection/table_of_contents.php
