#include <GL/freeglut.h>
#include "globals.hpp"
#include "bonds.hpp"
#include "clusters.hpp"
#include "update.hpp"
#include "render.hpp"
#include <ctime>

int main(int argc, char** argv)
{
    srand((unsigned int)time(nullptr));

    // 1) GLUT init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Democritus Apple Demo");

    // 2) Basic GL states
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // 3) Callback registration
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutIdleFunc(idleCallback);
    // For mouse cutting
    glutMouseFunc(mouseCallback); // if you declared it somewhere

    // 4) Setup lighting, materials if needed
    // (Alternatively, do this in some initGL() function)

    // 5) Generate particles, bonds, single cluster
    generateParticles(); 
    generateBonds();
    initSingleCluster();

    // 6) Enter main loop
    glutMainLoop();
    return 0;
}

