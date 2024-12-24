#include <GL/glut.h>
#include <math.h>

static int angle = 0;     // Rotation angle for the solar system
static float zoom = 10.0; // Zoom level
static int rotateX = 0;   // Rotation around X-axis
static int rotateY = 0;   // Rotation around Y-axis
static int prevX = 0, prevY = 0; // Previous mouse coordinates for dragging

// Initialize the environment
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);  // Background color (black)
    glEnable(GL_DEPTH_TEST);           // Enable depth testing
    glEnable(GL_LIGHTING);             // Enable lighting
    glEnable(GL_LIGHT0);               // Enable a light source
    GLfloat light_pos[] = { 1.0, 1.0, 1.0, 0.0 }; // Light position
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}

// Set material color for the planet
void setMaterialColor(float r, float g, float b) {
    GLfloat mat_ambient[] = { r * 0.2, g * 0.2, b * 0.2, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // White specular highlights
    GLfloat mat_shininess[] = { 50.0 }; // Shininess level

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

// Render text (planet name) at a given position
void renderText(float x, float y, float z, const char* text) {
    glRasterPos3f(x, y, z);  // Position the text
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *text);  // Render each character
        text++;
    }
}

// Draw a planet (as a sphere) with material color, size, and name
void drawPlanet(float distance, float size, float r, float g, float b, const char* name) {
    glPushMatrix();
    setMaterialColor(r, g, b);  // Set planet color
    glTranslatef(distance, 0.0, 0.0);  // Move planet to its orbit distance
    glutSolidSphere(size, 20, 16);     // Draw the planet as a sphere
    renderText(0.0, -size - 0.2, 0.0, name);  // Render planet name below it
    glPopMatrix();
}

// Draw orbits for the planets
void drawOrbit(float radius) {
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);  // Orbit color (white)
    glBegin(GL_LINE_LOOP);     // Draw a circular orbit
    for (int i = 0; i < 100; i++) {
        float theta = 2.0f * 3.1415926f * ((float)i / 100.0f);  // Angle in radians
        float x = radius * cosf(theta);  // X coordinate
        float z = radius * sinf(theta);  // Z coordinate
        glVertex3f(x, 0.0, z);           // Specify the vertex
    }
    glEnd();
    glPopMatrix();
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    // Apply zoom and rotation
    glTranslatef(0.0, 0.0, -zoom);       // Zoom in/out
    glRotatef(rotateX, 1.0, 0.0, 0.0);   // Rotate around X-axis
    glRotatef(rotateY, 0.0, 1.0, 0.0);   // Rotate around Y-axis

    // Draw the Sun (yellow)
    setMaterialColor(1.0, 1.0, 0.0);   // Sun color (yellow)
    glutSolidSphere(1.0, 20, 16);      // Sun at the center
    renderText(0.0, -1.2, 0.0, "Sun"); // Render "Sun" name

    // Define planets with their respective colors, sizes, and distances
    float planetDistances[] = { 1.5, 2.2, 3.0, 4.0, 5.5, 7.0, 8.5, 10.0, 11.5 };
    float planetSizes[] = { 0.1, 0.15, 0.18, 0.12, 0.4, 0.35, 0.25, 0.24, 0.1 };
    float planetColors[][3] = {
        {0.5, 0.5, 0.5}, // Mercury (gray)
        {0.9, 0.8, 0.4}, // Venus (pale yellow)
        {0.0, 0.5, 1.0}, // Earth (blue-green)
        {1.0, 0.0, 0.0}, // Mars (red)
        {1.0, 0.6, 0.4}, // Jupiter (orange-brown)
        {1.0, 1.0, 0.6}, // Saturn (pale yellow)
        {0.4, 0.7, 1.0}, // Uranus (light blue)
        {0.0, 0.0, 1.0}, // Neptune (deep blue)
        {0.7, 0.7, 0.7}  // Pluto (grayish-white)
    };

    const char* planetNames[] = { "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto" };

    // Loop to draw planets and their orbits
    for (int i = 0; i < 9; i++) {
        drawOrbit(planetDistances[i]);  // Draw orbit for the planet
        drawPlanet(planetDistances[i], planetSizes[i], planetColors[i][0], planetColors[i][1], planetColors[i][2], planetNames[i]);
    }

    glPopMatrix();
    glutSwapBuffers();
}

// Update the rotation angle
void update(int value) {
    angle = (angle + 1) % 360;   // Rotate the system (slower rotation)
    glutPostRedisplay();         // Request a redraw
    glutTimerFunc(50, update, 0); // Set timer for the next update
}

// Handle window resizing
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w/h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0); // Top-down view
}

// Keyboard interaction for rotation and reset
void keyboard(int key, int x, int y) {
    if (key == GLUT_KEY_UP) rotateX -= 5;   // Rotate up
    if (key == GLUT_KEY_DOWN) rotateX += 5; // Rotate down
    if (key == GLUT_KEY_LEFT) rotateY -= 5; // Rotate left
    if (key == GLUT_KEY_RIGHT) rotateY += 5; // Rotate right
    glutPostRedisplay();
}

// Reset view with 'r' key
void normalKeys(unsigned char key, int x, int y) {
    if (key == 'r') {
        rotateX = 0;
        rotateY = 0;
        zoom = 10.0;
    }
    glutPostRedisplay();
}

// Mouse interaction for zooming
void mouseWheel(int button, int state, int x, int y) {
    if (button == 3) zoom -= 1.0; // Scroll up (zoom in)
    if (button == 4) zoom += 1.0; // Scroll down (zoom out)
    glutPostRedisplay();
}

// Mouse dragging for rotation
void mouseMotion(int x, int y) {
    rotateY += (x - prevX);
    rotateX += (y - prevY);
    prevX = x;
    prevY = y;
    glutPostRedisplay();
}

// Mouse click and scroll callback
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        prevX = x;
        prevY = y;
    }

    // Detect mouse scroll up (button 3) or down (button 4)
    if (button == 3 && state == GLUT_DOWN) {  // Scroll up
        zoom -= 1.0;
        if (zoom < 1.0) zoom = 1.0;  // Prevent zooming too close
    }
    if (button == 4 && state == GLUT_DOWN) {  // Scroll down
        zoom += 1.0;
    }

    glutPostRedisplay();  // Redraw after zooming
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);   // Set window size
    glutCreateWindow("Interactive Solar System");

    init();                             // Initialize OpenGL settings
    glutDisplayFunc(display);            // Set display callback
    glutReshapeFunc(reshape);            // Set reshape callback
    glutSpecialFunc(keyboard);           // Special keys for rotation
    glutKeyboardFunc(normalKeys);        // Normal keys for reset
    glutMouseFunc(mouseClick);           // Handle mouse click and scroll (no glutMouseWheelFunc)
    glutMotionFunc(mouseMotion);         // Handle mouse motion for dragging
    glutTimerFunc(50, update, 0);        // Set timer for animation

    glutMainLoop();                      // Start the main loop
    return 0;
}

