## README

### Introduction

This project is a simple 2D side-scroller game developed using OpenGL and GLUT. The player controls a cart that can jump to avoid obstacles and collect items. The game environment includes elements such as clouds, a mall, a road strip, and various obstacles like potholes and slabs. The objective is to avoid obstacles and collect oranges to increase the score.

### Getting Started

#### Prerequisites

- OpenGL
- GLUT library
- A C++ compiler (e.g., GCC, MSVC)

#### Installation

1. **Install OpenGL and GLUT:** Follow the instructions specific to your operating system to install OpenGL and GLUT.
2. **Compile the Code:** Use your preferred C++ compiler to compile the source code. For example, using GCC:

   ```bash
   g++ -o game main.cpp -lGL -lGLU -lglut
   ```

3. **Run the Game:** Execute the compiled binary.

   ```bash
   ./game
   ```

### Code Overview

The code is structured as follows:

#### Includes and Namespace

```cpp
#define _USE_MATH_DEFINES
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;
```

#### Struct Definitions

- **Rectangle:** Represents road strips.
- **Mall:** Represents a mall structure.
- **Clouds:** Represents clouds in the sky.
- **Pothole:** Represents obstacles.
- **Orange:** Represents collectible items.
- **Slab:** Represents platforms the cart can jump onto.

#### Global Variables

- **Game Objects:** `vector<Rectangle> rectangles`, `vector<Clouds> cloud`, `Pothole pothole`, `vector<orange> org`, `Slab slab`, `Mall mall`
- **Game State:** `float cartY`, `bool gameOver`, `bool isJumping`, `bool isOnSlab`, `float jumpHeight`, `float speed`, `int score`

#### Initialization and Reset Functions

- **myinit():** Sets up the initial OpenGL environment.
- **resetGame():** Resets the game state to initial conditions.

#### Drawing Functions

- **setPixel(), drawLine(), thickLine():** Low-level drawing functions.
- **mpCircle(), drawCircle(), drawGradientCircle(), drawEllipse(), mpEllipse():** Functions to draw various shapes.
- **drawMall(), drawClouds(), drawquad(), drawSlab(), drawCart(), draworange():** Functions to draw specific game elements.

#### Keyboard Input

- **keyboard():** Handles keyboard input for jumping and restarting the game.

#### Game Logic

- **performJumpAnimation():** Controls the jump animation of the cart.
- **update*Position():** Functions to update positions of game elements (e.g., `updateroadstripPosition()`, `updatecloudsPosition()`, `updatePotholePosition()`, `updateMallPosition()`, `updateSlabPosition()`, `updateorgposition()`).
- **checkCollision(), checkfruitcoll():** Collision detection functions.

#### Text Drawing

- **drawStrokeText(), drawStrokeTextandscore():** Functions to draw text on the screen.

#### Main Display Function

```cpp
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Drawing and updating functions
    drawClouds();
    drawMall(mall.x);
    drawSlab(slab.x, slab.y, slab.width, slab.length);
    drawCart(100, cartY);
    drawPothole(pothole.x, pothole.y, pothole.horizontalAxis, pothole.verticalAxis);
    drawOranges();

    // Check for collisions
    if (checkCollision(100)) {
        gameOver = true;
        drawStrokeText(850, 500, 0.3, "Game Over");
    }

    // Check for fruit collisions
    if (checkfruitcoll(100)) {
        score += 10;
    }

    // Update positions
    updateroadstripPosition();
    updatecloudsPosition();
    updatePotholePosition();
    updateMallPosition();
    updateSlabPosition();
    updateorgposition();

    // Handle jump animation
    performJumpAnimation();

    // Draw score
    drawStrokeTextandscore(1500, 900, 0.3, "Score: ", score);

    // Swap buffers
    glutSwapBuffers();
    glutPostRedisplay();
}
```

### How to Play

1. **Start the Game:** Run the executable to start the game.
2. **Controls:**
   - Press the spacebar to make the cart jump.
   - Avoid potholes and other obstacles.
   - Collect oranges to increase your score.
   - Press the Enter key to restart the game after a game over.

### Customization

- **Modify Game Speed:** Adjust the `speed` variable to change the game's speed.
- **Change Jump Height:** Adjust the `jumpHeight` variable to change the jump height.
- **Add More Obstacles or Collectibles:** Modify the `vector` containers and update corresponding functions to add more game elements.

### Conclusion

This project demonstrates basic 2D game development using OpenGL and GLUT in C++. It covers various aspects such as drawing shapes, handling input, collision detection, and basic game physics. Feel free to enhance the game by adding new features or improving existing ones.

### Acknowledgments

Special thanks to the creators and maintainers of OpenGL and GLUT for providing the necessary libraries to develop this game.
