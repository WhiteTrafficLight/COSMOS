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
    
    initGL();
    generateParticles(); 
    generateBonds();
    initSingleCluster();
    // 3) Callback registration
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutIdleFunc(idleCallback);
    // For mouse cutting
    glutMouseFunc(mouseCallback);

    // 6) Enter main loop
    glutMainLoop();
    return 0;
}

