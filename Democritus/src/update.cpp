#include "update.hpp"
#include "globals.hpp"
#include "clusters.hpp"
#include "bonds.hpp"
#include <GL/freeglut.h>
#include <iostream>

void updateCutting()
{
    // 1) Move blade from bladePos toward targetBladePos
    Vec3 diff = sub(targetBladePos, bladePos);
    bladePos = add(bladePos, mul(diff, bladeSpeed * DT));

    // 2) If we want a blade normal from cluster[0].center -> bladePos, for example
    if(!clusters.empty())
    {
        Vec3 bladeNormal = normalize(sub(clusters[0].center, bladePos));

        // 3) Repel particles within bladeThickness range
        for(size_t i=0; i<particles.size(); i++)
        {
            Vec3 toBlade = sub(particles[i], bladePos);
            float d = dot(toBlade, bladeNormal);
            if(std::fabs(d) < bladeThickness*0.5f)
            {
                float pen = (bladeThickness*0.5f) - std::fabs(d);
                Vec3 rep = mul(bladeNormal, BLADE_REPULSION_K * pen);
                velocities[i] = add(velocities[i], mul(rep, DT));
            }
        }

        // 4) Break bonds that cross the blade plane
        for(auto &b : bonds)
        {
            if(b.broken) 
                continue;
            float d1 = dot(sub(particles[b.i], bladePos), bladeNormal);
            float d2 = dot(sub(particles[b.j], bladePos), bladeNormal);
            if(d1 * d2 < 0.0f)
            {
                b.broken = true;
            }
        }
    }

    // 5) For demonstration, update each particle individually
    for(size_t i=0; i<particles.size(); i++)
    {
        velocities[i] = add(velocities[i], mul(GRAVITY, DT));
        velocities[i] = mul(velocities[i], DAMPING);
        particles[i] = add(particles[i], mul(velocities[i], DT));

        // table collision
        if(particles[i].y - PARTICLE_RADIUS < TABLE_Y)
        {
            particles[i].y = TABLE_Y + PARTICLE_RADIUS;
            velocities[i].y = 0.0f;
        }
    }

    // 6) Check if blade is near target
    if(lengthVec(sub(bladePos, targetBladePos)) < 0.05f)
    {
        cutting = false;
        bladeCutFinished = true;
        std::cout << "Cut finished.\n";

        // Optionally re-split clusters
        // updateClustersAfterCut();
    }
}

void idleCallback()
{
    if(cutting)
    {
        updateCutting();
    }
    else
    {
        updateAllClusters(); 
    }

    // Trigger a redraw
    glutPostRedisplay();
}

void mouseCallback(int button, int state, int x, int y)
{
    std::cout << "Mouse event: button=" << button << ", state=" << state << std::endl;
    if (state == GLUT_DOWN) {
        // Only trigger cutting if not already cutting
        if (!cutting) {
            cutting = true;
            bladeCutFinished = false;
            std::cout << "Cut initiated." << std::endl;
            std::flush(std::cout);
        }
    }
}

