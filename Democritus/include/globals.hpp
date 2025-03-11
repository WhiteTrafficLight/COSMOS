#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>

/**
 * Basic 3D vector struct.
 */
struct Vec3
{
    float x, y, z;
};

// Global simulation constants
extern int    NUM_PARTICLES;
extern float  APPLE_RADIUS;
extern float  PARTICLE_RADIUS;
extern float  DT;
extern float  DAMPING;

/**
 * Gravity vector, table height, etc.
 */
extern Vec3   GRAVITY;
extern float  TABLE_Y;

/**
 * Global containers for particles (atoms) and velocities
 */
extern std::vector<Vec3> particles;
extern std::vector<Vec3> velocities;

/**
 * Variables for blade-based cutting
 */
extern bool   cutting;              ///< If true, we are in cutting mode
extern bool   bladeCutFinished;     ///< True once the blade movement is done
extern float  bladeThickness;       ///< Blade thickness
extern float  bladeSpeed;           ///< Blade movement speed
extern float  BLADE_REPULSION_K;    ///< Strength of repulsion
extern struct Vec3 bladePos;        ///< Current blade position
extern struct Vec3 targetBladePos;  ///< Target position for blade
extern Vec3 INITIAL_OFFSET;        ///OFFSET of the apple from the table

/**
 * Utility functions
 */
float rand01();
float lengthVec(const Vec3 &v);
Vec3  add(const Vec3 &a, const Vec3 &b);
Vec3  sub(const Vec3 &a, const Vec3 &b);
Vec3  mul(const Vec3 &a, float s);
Vec3  normalize(const Vec3 &v);
float dot(const Vec3 &a, const Vec3 &b);
void generateParticles();

#endif // GLOBALS_HPP

