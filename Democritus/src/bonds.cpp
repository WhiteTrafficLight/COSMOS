#include "bonds.hpp"

std::vector<Bond> bonds;

void generateBonds()
{
    bonds.clear();

    int n = static_cast<int>(particles.size());
    for(int i = 0; i < n; i++)
    {
        for(int j = i+1; j < n; j++)
        {
            Vec3 diff = sub(particles[j], particles[i]);
            float dist = lengthVec(diff);

            // Use a global or local threshold. We'll assume
            // a global BOND_CONNECT_DIST from somewhere, or just a fixed value
            float threshold = 0.06f; // or BOND_CONNECT_DIST if you prefer
            if(dist < threshold)
            {
                Bond b;
                b.i          = i;
                b.j          = j;
                b.restLength = dist;
                b.broken     = false;
                bonds.push_back(b);
            }
        }
    }

    std::cout << "Bonds created: " << bonds.size() << "\n";
}

