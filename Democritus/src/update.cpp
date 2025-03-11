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
        // Convert degrees to radians:
        float rad = bladeAngle * (M_PI / 180.0f);

        // Calculate the normal vector in the XY plane (assuming Z remains 0)
        Vec3 bladeNormal = { cosf(rad), sinf(rad), 0.0f };
        //Vec3 bladeNormal = normalize(sub(clusters[0].center, bladePos));

        // 3) Repel particles within bladeThickness range
        for (size_t i = 0; i < particles.size(); i++)
        {
            // Compute the vector from the blade to the particle
            Vec3 toBlade = sub(particles[i], bladePos);
            // Ignore the Z component for 2D distance calculations:
            toBlade.z = 0.0f;

            // Calculate the signed distance from the particle to the blade's plane in 2D
            float d = dot(toBlade, bladeNormal);
            
            if (std::fabs(d) < bladeThickness * 0.1f)
            {
                float pen = (bladeThickness * 0.1f) - std::fabs(d);
                Vec3 rep = mul(bladeNormal, BLADE_REPULSION_K * pen);
                // Update velocity accordingly in 2D (Z remains 0)
                velocities[i] = add(velocities[i], mul(rep, DT));
            }
        }

        // 4) Break bonds that cross the blade plane
        const float BLADE_CUT_DISTANCE_THRESHOLD = 0.1f;
        for(auto &b : bonds)
        {
            if(b.broken) 
                continue;
            // Compute distances along the blade's normal direction
            float d1 = dot(sub(particles[b.i], bladePos), bladeNormal);
            float d2 = dot(sub(particles[b.j], bladePos), bladeNormal);

            // Compute 2D distance from each particle to the blade position
            Vec3 diff1 = sub(particles[b.i], bladePos);
            Vec3 diff2 = sub(particles[b.j], bladePos);
            diff1.z = 0.0f; // Ignore Z for 2D distance check
            diff2.z = 0.0f;
            float len1 = lengthVec(diff1);
            float len2 = lengthVec(diff2);

            // Ensure both conditions: opposite sides + close enough to the blade
            if (d1 * d2 < 0.0f && (len1 < BLADE_CUT_DISTANCE_THRESHOLD || len2 < BLADE_CUT_DISTANCE_THRESHOLD)) 
            {
                b.broken = true;
            }
        }
    }
  
    
    // 5) For demonstration, update each particle individually
    for(size_t i=0; i<particles.size(); i++)
    {
        //velocities[i] = add(velocities[i], mul(GRAVITY, DT));
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
        updateClustersAfterCut();
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
    //std::cout << "Mouse event: button=" << button << ", state=" << state << std::endl;
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

