#include "Game.h"

GLFWwindow* window;
Game *game;

void drawCircle( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberofSides, GLfloat r, GLfloat g, GLfloat b);

float pacman_x = 320.0f;
float pacman_y = 240.0f;

float ghost_x = 320.0f;
float ghost_y = 240.0f;
float ghost_speed_x = 2.0f;
float ghost_speed_y = 2.0f;

bool checkCollision(float x1, float y1, float r1, float x2, float y2, float r2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (r1 + r2);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const float speed = 10.0f;
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
    
    glfwSetKeyCallback(window, key_callback);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float triangle_angle = 60;

    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
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
        
        bool collided = checkCollision(pacman_x, pacman_y, 75, ghost_x, ghost_y, 75);
        

        
        glColor3ub(1, 1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // Draw Pac-Man (Circle) at the updated position
        if (collided) {
            drawCircle(pacman_x, pacman_y, 0, 80, 360, 1.0, 0.0, 0.0); // Pac-Man in red if collided
        } else {
            drawCircle(pacman_x, pacman_y, 0, 80, 360, 1.0, 1.0, 0.0); // Pac-Man in original color if not collided
        }
        // Pac-Man Mouth
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(pacman_x, pacman_y, 0);
        glRotatef(180.0, 0.0f, 0.0f, 1.0f);
        glTranslatef(50, 120, 0.0);
        drawCircle(-100, -118, 0, 60, 3, 0.0, 0.0, 0.0);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(ghost_x, ghost_y, 0);
        // Body, Eye
        drawCircle(0, 0, 0, 80, 360, 1.0, 0.0, 0.0);

        drawCircle(0, 0, 0, 40, 360, 1.0, 1.0, 1.0);

        drawCircle(0, 0, 0, 20, 360, 0.0, 0.0, 0.0);

        // Legs
        glPushMatrix();
        glRotatef(31.0f, 0.0f, 0.0f, 1.0f);
        drawCircle(-55, -48, 0, 30, 3, 1.0, 0.0, 0.0);
        glPopMatrix();

        glPushMatrix();
        glRotatef(31.0f, 0.0f, 0.0f, 1.0f);
        drawCircle(-15, -73, 0, 30, 3, 1.0, 0.0, 0.0);
        glPopMatrix();

        glfwSwapBuffers(window);

 
        glfwPollEvents();
    }
    
 
    glfwTerminate();
 
    return 0;
}


void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides, GLfloat r, GLfloat g, GLfloat b)
{
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
 


