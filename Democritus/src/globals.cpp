#include "globals.hpp"

// Global simulation constants
int    NUM_PARTICLES    = 600;
float  APPLE_RADIUS     = 0.5f;
float  PARTICLE_RADIUS  = 0.02f;
float  DT               = 0.01f;
float  DAMPING          = 0.99f;

// Gravity, table
Vec3   GRAVITY          = {0.0f, -0.05f, 0.0f};
float  TABLE_Y          = -0.3f;

// Global containers
std::vector<Vec3> particles;
std::vector<Vec3> velocities;

// Blade / cutting variables
bool   cutting          = false;
bool   bladeCutFinished = false;
float  bladeThickness   = 0.02f;
float  bladeSpeed       = 0.1f;
float  BLADE_REPULSION_K= 200.0f;
Vec3   bladePos         = {0.0f, 0.0f, 0.0f};
Vec3   targetBladePos   = {0.0f, 0.0f, 0.0f};
Vec3 INITIAL_OFFSET = {0.0f, 0.0f, 0.3f};

/**
 * Utility implementations
 */
float rand01()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float lengthVec(const Vec3 &v)
{
    return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vec3 add(const Vec3 &a, const Vec3 &b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 sub(const Vec3 &a, const Vec3 &b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 mul(const Vec3 &a, float s)
{
    return {a.x*s, a.y*s, a.z*s};
}

Vec3 normalize(const Vec3 &v)
{
    float len = lengthVec(v);
    if (len < 1e-6f) return {0,0,0};
    return {v.x/len, v.y/len, v.z/len};
}

float dot(const Vec3 &a, const Vec3 &b)
{
    return (a.x*b.x + a.y*b.y + a.z*b.z);
}

void generateParticles() {
    // Clear existing particles and velocities
    particles.clear();
    velocities.clear();

    // Maximum number of attempts to generate NUM_PARTICLES particles
    const int maxAttempts = NUM_PARTICLES * 100;
    int attempts = 0;

    // Loop until we have generated the required number of particles
    while ((int)particles.size() < NUM_PARTICLES && attempts < maxAttempts) {
        attempts++;

        // Generate a random point inside a unit sphere with uniform distribution:
        // Use spherical coordinates with r = R * cube_root(u) so that the volume is uniformly sampled.
        float u = rand01();                   // uniform random in [0,1]
        float r = APPLE_RADIUS * cbrt(u);       // cube root ensures uniform density in the sphere
        float theta = 2.0f * M_PI * rand01();   // azimuthal angle in [0, 2π]
        float cosPhi = 1.0f - 2.0f * rand01();   // cos(phi) uniformly in [-1, 1]
        float phi = acosf(cosPhi);              // polar angle in [0, π]

        // Convert spherical coordinates to Cartesian coordinates
        float x = r * sinf(phi) * cosf(theta);
        float y = r * sinf(phi) * sinf(theta);
        float z = r * cosPhi;                   // or r * cosf(phi)

        Vec3 candidate = { x, y, z };

        // Add the initial offset so that the apple is positioned correctly (e.g., on a table)
        candidate = add(candidate, INITIAL_OFFSET);

        // Check for overlap with already generated particles
        bool overlap = false;
        for (const auto &p : particles) {
            if (lengthVec(sub(candidate, p)) < 2.0f * PARTICLE_RADIUS) {
                overlap = true;
                break;
            }
        }

        // If no overlap, accept the candidate particle
        if (!overlap) {
            particles.push_back(candidate);
            velocities.push_back({0, 0, 0});  // initialize velocity to zero
        }
    }

    // Output the result
    std::cout << "Generated " << particles.size() << " particles after " << attempts << " attempts.\n";
}
