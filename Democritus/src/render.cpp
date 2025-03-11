#include "render.hpp"
#include "globals.hpp"
#include "clusters.hpp"
#include "bonds.hpp"
#include <GL/freeglut.h>
#include <iostream>

void displayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // If there's at least one cluster, center the camera on cluster[0]
    if(!clusters.empty())
    {
        Vec3 c = clusters[0].center;
        glTranslatef(-c.x, -c.y, -3.0f);
    }
    else
    {
        // fallback
        glTranslatef(0.0f, 0.0f, -3.0f);
    }

    // draw table
    glDisable(GL_LIGHTING);
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, TABLE_Y, -1.0f);
    glVertex3f( 1.0f, TABLE_Y, -1.0f);
    glVertex3f( 1.0f, TABLE_Y,  1.0f);
    glVertex3f(-1.0f, TABLE_Y,  1.0f);
    glEnd();
    glEnable(GL_LIGHTING);

    // draw blade
    extern float bladeThickness;
    extern Vec3 bladePos;
    float angle = 135.0f; // or any other angle

    glPushMatrix();
    glTranslatef(bladePos.x, bladePos.y, bladePos.z);
    glRotatef(angle, 0, 0, 1);
    glDisable(GL_LIGHTING);
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_QUADS);
    glVertex3f(-bladeThickness/2, -0.5f, 0);
    glVertex3f( bladeThickness/2, -0.5f, 0);
    glVertex3f( bladeThickness/2,  0.5f, 0);
    glVertex3f(-bladeThickness/2,  0.5f, 0);
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // draw bonds (lines)
    glDisable(GL_LIGHTING);
    glLineWidth(1.0f);
    glColor3f(1,1,1);
    for(auto &b : bonds)
    {
        if(b.broken) 
            continue;
        glBegin(GL_LINES);
        glVertex3f(particles[b.i].x, particles[b.i].y, particles[b.i].z);
        glVertex3f(particles[b.j].x, particles[b.j].y, particles[b.j].z);
        glEnd();
    }
    glEnable(GL_LIGHTING);

    // draw particles (red spheres)
    for(size_t i=0; i<particles.size(); i++)
    {
        glPushMatrix();
        glTranslatef(particles[i].x, particles[i].y, particles[i].z);
        glColor3f(1.0f, 0.0f, 0.0f);
        glutSolidSphere(PARTICLE_RADIUS, 10, 10);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void reshapeCallback(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w/h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

