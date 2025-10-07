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

For a FANTASTIC explanation of collision detection, with examples, see:  
[Jeffrey Thompson’s Collision Detection Guide](https://www.jeffreythompson.org/collision-detection/table_of_contents.php)

Simply, I reversed the equation for the collision detection of two circles.  
Normally, a collision is detected **when the distance between the two circle centers becomes less than the sum of their radii**.  
In my code, the opposite is necessary — so I simply reversed this logic.

