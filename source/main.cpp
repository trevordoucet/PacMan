#include "Game.h"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

GLFWwindow* window;
Game *game;

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides, GLfloat r, GLfloat g, GLfloat b);
void drawLives(int lives, float iconRadius);
void drawPacManIcon(float centerX, float centerY, float radius);


struct Pellet {
   float x, y;
   bool isSuperPellet;
   bool isActive;

   Pellet(float x, float y, bool isSuperPellet, bool isActive)
       : x(x), y(y), isSuperPellet(isSuperPellet), isActive(isActive) {}
};



float pacman_x = 320.0f;
float pacman_y = 240.0f;
float initial_pacman_x = SCREEN_WIDTH / 2.0f;
float initial_pacman_y = SCREEN_HEIGHT / 2.0f;

float ghost_x = 320.0f;
float ghost_y = 240.0f;
float ghost_speed_x = 1.0f;
float ghost_speed_y = 1.0f;
float initial_ghost_x = SCREEN_WIDTH / 2.0f;
float initial_ghost_y = SCREEN_HEIGHT / 2.0f;
int pacmanLives = 3;

bool checkCollision(float x1, float y1, float r1, float x2, float y2, float r2) {
   float dx = x2 - x1;
   float dy = y2 - y1;
   float distance = sqrt(dx * dx + dy * dy);
   return distance < (r1 + r2);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
   const float speed = 20.0f;
   if (action == GLFW_PRESS || action == GLFW_REPEAT) {
       switch (key) {
           case GLFW_KEY_UP:
               pacman_y += speed;
               break;
           case GLFW_KEY_DOWN:
               pacman_y -= speed;
               break;
           case GLFW_KEY_LEFT:
               pacman_x -= speed;
               break;
           case GLFW_KEY_RIGHT:
               pacman_x += speed;
               break;
           default:
               break;
       }
   }
}

int main(void)
{
   GLFWwindow* window;
   
   if (!glfwInit())
   {
       return -1;
   }
   
   window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple Motion", NULL, NULL);
   
   if (!window)
   {
       glfwTerminate();
       return -1;
   }
   
   glfwMakeContextCurrent(window);
   
   if (!gladLoadGL(glfwGetProcAddress)) {
       printf("Failed to initialize GLAD\n");
       return -1;
   }
   double lastTime = glfwGetTime(); // Gets the number of seconds since GLFW started, as a double
   double currentTime;
   double deltaTime;
   
   bool superPelletActive = false;
   float superPelletEffectTimer = 0.0f;
   float ghostFlickerTimer = 0.0f;
   bool ghostEaten = false;
   float ghostRespawnTimer = 0.0f;
   bool pacmanEaten = false;
   float pacmanRespawnTimer = 0.0f;
   
   
   glfwSetKeyCallback(window, key_callback);
   glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   std::vector<Pellet> pellets;
   pellets.push_back(Pellet(100.0f, 100.0f, false, true));
   pellets.push_back(Pellet(150.0f, 100.0f, false, true));
   pellets.push_back(Pellet(200.0f, 200.0f, false, true));
   pellets.push_back(Pellet(300.0f, 300.0f, true, true));
   pellets.push_back(Pellet(400.0f, 400.0f, true, true));
   
   float triangle_angle = 60;
   
   bool gameOver = false;
   
   while (!glfwWindowShouldClose(window) && !gameOver)
   {
       currentTime = glfwGetTime();
       deltaTime = currentTime - lastTime;
       lastTime = currentTime;
       
       // Define these near your other game configuration variables
       const float pacman_radius = 40.0f; // The radius of Pac-Man for collision detection
       const float ghost_radius = 40.0f;  // The radius of the ghost for collision detection
       
       
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();
       
       
       
       // Iterate through and draw active pellets
       for (auto& pellet : pellets) {
           if (pellet.isActive) {
               float radius = pellet.isSuperPellet ? 30.0f : 5.0f; // Super pellets are larger
               drawCircle(pellet.x, pellet.y, 0, radius, 30, 1.0f, 1.0f, 1.0f); // Drawing pellets
           }
       }
       // Use deltaTime for time-dependent operations
       if (superPelletActive) {
           superPelletEffectTimer -= deltaTime;
           if (superPelletEffectTimer <= 0.0f) {
               superPelletActive = false;
               // ... Rest of the logic for when super pellet effect ends
           }
       }
       
       // Check for collisions with Pac-Man to deactivate pellets and activate super pellet effect
       for (auto& pellet : pellets) {
           if (pellet.isActive && checkCollision(pacman_x, pacman_y, pacman_radius, pellet.x, pellet.y, pellet.isSuperPellet ? 10.0f : 5.0f)) {
               pellet.isActive = false; // Pac-Man eats the pellet
               if (pellet.isSuperPellet) {
                   superPelletActive = true;
                   superPelletEffectTimer = 3.0f; // Super pellet effect lasts for 3 seconds
                   ghostFlickerTimer = 0.0f; // Reset the flicker timer
               }
           }
       }
       
       bool collidedWithGhost = checkCollision(pacman_x, pacman_y, pacman_radius, ghost_x, ghost_y, ghost_radius);
       if (collidedWithGhost) {
           if (!superPelletActive && !pacmanEaten) {
               pacmanLives--;
               // Pac-Man has collided with a ghost and is now "eaten"
               pacmanEaten = true;
               pacmanRespawnTimer = 3.0f; // Pac-Man will respawn after 3 seconds
               // Optionally, move Pac-Man off-screen or make him invisible
               pacman_x = -100.0f; // Example off-screen position
               pacman_y = -100.0f;
               if (pacmanLives <= 0) {
                   gameOver = true;
               }
               
           } else {
               // If the super pellet is active, handle the ghost-eaten logic
               ghostEaten = true;
               ghostRespawnTimer = 3.0f; // Set the respawn timer for the ghost
               // ... other logic for when the ghost is eaten ...
           }
       }
       // Handle Pac-Man respawn after being eaten
       if (pacmanEaten) {
           pacmanRespawnTimer -= deltaTime;
           if (pacmanRespawnTimer <= 0.0f) {
               pacmanEaten = false;
               pacmanRespawnTimer = 0.0f;
               
               if (pacmanLives > 0) {
                   // Reset Pac-Man's position to its starting point or respawn location
                   pacman_x = initial_pacman_x;
                   pacman_y = initial_pacman_y;
               }
           }
       }
       
       
       
       // Ghost Character
       ghost_x += ghost_speed_x;
       ghost_y += ghost_speed_y;
       
       // Adjusted boundary checks
       if (ghost_x > SCREEN_WIDTH - 80 || ghost_x < 80) {
           ghost_speed_x *= -1;
       }
       if (ghost_y > SCREEN_HEIGHT - 80 || ghost_y < 80) {
           ghost_speed_y *= -1;
       }
       
       // At the rendering section of your game loop
       for (int i = 0; i < pacmanLives; i++) {
           // Draw a small Pac-Man icon for each life
           drawCircle(pacman_x, pacman_y, 0, 80, 360, 1.0, 1.0, 0.0); // Replace with actual function and adjust positioning as needed
       }
       
       glColor3ub(1, 1, 1);
       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();
       // Draw Pac-Man (Circle) at the updated position
       // At the rendering section of your game loop
       // Inside your rendering loop, replace with your actual lives display coordinates
       // Draw at position (10 + i * 20
       
       // Pac-Man Mouth
       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();
       glTranslatef(pacman_x, pacman_y, 0);
       glRotatef(180.0, 0.0f, 0.0f, 1.0f);
       glTranslatef(45, 120, 0.0);
       drawCircle(-100, -118, 0, 58, 3, 0.0, 0.0, 0.0);
       
       // Ghost drawing logic
       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();
       glTranslatef(ghost_x, ghost_y, 0);
       
       if (!ghostEaten) { // Only draw the ghost if it hasn't been eaten
           glMatrixMode(GL_MODELVIEW);
           glLoadIdentity();
           glTranslatef(ghost_x, ghost_y, 0);
           
           // Flicker the ghost if the super pellet is active
           if (superPelletActive) {
               ghostFlickerTimer += deltaTime;
               bool drawInBlue = ((int)(ghostFlickerTimer * 10)) % 2 == 0;
               
               GLfloat r = drawInBlue ? 0.0f : 1.0f;
               GLfloat g = drawInBlue ? 0.0f : 1.0f;
               GLfloat b = drawInBlue ? 1.0f : 1.0f;
               
               // Body
               drawCircle(0, 0, 0, 80, 360, r, g, b);
           } else {
               // If the super pellet effect is not active, draw the ghost in its normal color
               drawCircle(0, 0, 0, 80, 360, 1.0, 0.0, 0.0); // Normal ghost color
           }
           
           // Eyes (always drawn the same way)
           drawCircle(0, 0, 0, 40, 360, 1.0, 1.0, 1.0); // White part of the eye
           drawCircle(0, 0, 0, 20, 360, 0.0, 0.0, 0.0); // Pupil
       }
       // Respawn logic for the ghost
       if (ghostEaten) {
           ghostRespawnTimer -= deltaTime;
           if (ghostRespawnTimer <= 0.0f) {
               ghostEaten = false;
               ghostRespawnTimer = 3.0f;
               // Reset the ghost's position to the starting point
               ghost_x = initial_ghost_x; // Use a specific starting X coordinate
               ghost_y = initial_ghost_y; // Use a specific starting Y coordinate
               // No need to reset ghost_speed_x and ghost_speed_y if they're not changed elsewhere
           }
       }
       bool allPelletsEaten = true;
       for (const auto& pellet : pellets) {
           if (pellet.isActive) {
               allPelletsEaten = false;
               break; // As soon as we find an active pellet, we can stop checking
           }
       }

           
           if (allPelletsEaten) {
               gameOver = true; // Game over because all pellets are eaten
           }
           
           drawLives(pacmanLives, 20.0f);
           
           glfwSwapBuffers(window);
           
           
           glfwPollEvents();
       }



   
       // After the game loop, you can handle game over state
       if (gameOver) {
           
           std::cout << "Game Over" << std::endl;
            // If you want to pause the console window
            std::cout << "Press Enter to exit..." << std::endl;
            std::cin.get(); // Wait for an Enter key press

       }
       
       glfwTerminate();
       
       return 0;
   }
   

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides, GLfloat r, GLfloat g, GLfloat b) {
   GLint numberOfVertices = numberOfSides + 2;
   GLfloat doublePi = 2.0f * M_PI;
   
   GLfloat* circleVertices = (GLfloat*)malloc(sizeof(GLfloat) * 3 * numberOfVertices);
   GLfloat* circleColors = (GLfloat*)malloc(sizeof(GLfloat) * 3 * numberOfVertices);
   
   circleVertices[0] = x;
   circleVertices[1] = y;
   circleVertices[2] = z;
   
   circleColors[0] = r;
   circleColors[1] = g;
   circleColors[2] = b;
   
   for (int i = 0; i < numberOfVertices; i++)
   {
       circleVertices[i * 3] = x + (radius * cos(i * doublePi / numberOfSides));
       circleVertices[i * 3 + 1] = y + (radius * sin(i * doublePi / numberOfSides));
       circleVertices[i * 3 + 2] = z;
       
       float angle = i * doublePi / numberOfSides;
       
       
       //GLfloat r = 1.0;
       // GLfloat g = 0.64;
       // GLfloat b = 0.0;
       
       circleColors[i * 3] = r;
       circleColors[i * 3 + 1] = g;
       circleColors[i * 3 + 2] = b;
   }
   
   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, circleVertices);
   
   glEnableClientState(GL_COLOR_ARRAY);
   glColorPointer(3, GL_FLOAT, 0, circleColors);
   
   glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
   
   glDisableClientState(GL_COLOR_ARRAY);
   glDisableClientState(GL_VERTEX_ARRAY);
   
   
   free(circleVertices);
   free(circleColors);
}

void drawLives(int lives, float iconRadius) {
   float startX = SCREEN_WIDTH / 4; // Start drawing from the right side
   float startY = SCREEN_HEIGHT / 4; // A fixed distance from the bottom

   for (int i = 0; i < lives; i++) {
       // Calculate the x position for each icon
       float x = startX - (i * (iconRadius * 2 + 20)); // Move left for each icon

       // Draw Pac-Man icon for each life
       drawPacManIcon(x, startY, iconRadius);
   }
}


void drawPacManIcon(float centerX, float centerY, float radius) {
   glColor3f(1.0f, 1.0f, 0.0f); // Yellow for Pac-Man

   // Draw Pac-Man Body
   glBegin(GL_TRIANGLE_FAN);
   glVertex2f(centerX, centerY); // Center of Pac-Man
   for(int angle = 0; angle <= 360; angle++) {
       if(angle > 30 && angle < 330) { // Create an opening for Pac-Man's mouth
           continue;
       }
       float rad = angle * M_PI / 180.0f; // Convert degrees to radians
       glVertex2f(centerX + cos(rad) * radius, centerY + sin(rad) * radius);
   }
   glEnd();

   // Optionally, draw an eye for Pac-Man
   glColor3f(0.0f, 0.0f, 0.0f); // Black for the eye
   glBegin(GL_POINTS);
   float eyeX = centerX + radius * 0.5f;
   float eyeY = centerY - radius * 0.7f;
   glVertex2f(eyeX, eyeY);
   glEnd();
}
