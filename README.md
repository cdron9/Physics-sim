This rendering portion of this program is adapted from code within my other GitHub repositories, specifically the one brightly named 'SDL mess about'. The adaptations mainly involve passing struct variables into rendering functions instead of using magic numbers.

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

A collision is detected when the ball’s outer edge reaches or exceeds the container’s radius:

```
if (distance + ball->radius >= container.radius)
```

Here, `distance` is the distance between the container’s center and the ball’s center.
If this sum equals or surpasses the container’s radius, the ball has contacted the wall.


### Reflection and Bouncing

When the ball hits the inner wall of the circular container, its velocity is reflected using basic vector math and physics principles. This ensures realistic bouncing behavior.


#### Formula

Once a collision is detected, the normal vector (pointing inward from the wall) is calculated and used to reflect the velocity:

```
ball->vx = ball->vx - 2 * dot * nx;
ball->vy = ball->vy - 2 * dot * ny;
```

This applies the reflection formula:

[
v' = v - 2(v \cdot n)n
]

where:

* `v` = incoming velocity vector
* `n` = normalized surface normal
* `v'` = reflected (outgoing) velocity vector
* `v · n` = dot product, representing how much of the velocity is directed into the wall

#### Energy Loss (Bounciness)

To make the bounce more realistic, some energy is lost using a **restitution constant**:

```
ball->vx *= RESTITUTION;
ball->vy *= RESTITUTION;
```

* `RESTITUTION = 1` → perfectly elastic bounce (no energy lost)
* `RESTITUTION < 1` → inelastic bounce (ball slows slightly each hit)

#### Position Correction

After the bounce, the ball may slightly overlap the wall due to numerical rounding.
To prevent this, the ball’s position is nudged back inside:

```
ball->centerX += nx * overlap;
ball->centerY += ny * overlap;
```

This ensures the ball doesn’t “stick” or jitter at the boundary.

#### Summary

| Concept             | Formula / Code                          | Description                       |
| ------------------- | --------------------------------------- | --------------------------------- |
| Collision Detection | `distance + radius >= container.radius` | Detects contact with wall         |
| Normal Calculation  | `nx = -x_dist / distance`               | Finds inward-facing normal        |
| Reflection          | `v' = v - 2(v · n)n`                    | Flips velocity across wall normal |
| Energy Loss         | `v *= RESTITUTION`                      | Simulates bounciness              |
| Overlap Fix         | `pos += n * overlap`                    | Prevents sticking at wall         |

This combination of detection, reflection, and correction results in smooth, realistic bouncing motion inside a circular container.


Thank you :)
--- README translated to markdown using ChatGPT ---


