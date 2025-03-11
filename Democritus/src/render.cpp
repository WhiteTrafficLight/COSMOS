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

    glPushMatrix();
    glTranslatef(bladePos.x, bladePos.y, bladePos.z);
    glRotatef(bladeAngle, 0, 0, 1);
    glDisable(GL_LIGHTING);
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_QUADS);
    glVertex3f(-bladeThickness/2, -0.1f, 0);
    glVertex3f( bladeThickness/2, -0.1f, 0);
    glVertex3f( bladeThickness/2,  0.1f, 0);
    glVertex3f(-bladeThickness/2,  0.1f, 0);
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

void initGL() {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    // Enable lighting and set up a light source
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    float ambientLight[]  = {0.2f, 0.2f, 0.2f, 1.0f};
    float diffuseLight[]  = {0.8f, 0.8f, 0.8f, 1.0f};
    float lightPos[]      = {5.0f, 5.0f, 5.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Enable color material so that glColor3f calls affect the material properties
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}