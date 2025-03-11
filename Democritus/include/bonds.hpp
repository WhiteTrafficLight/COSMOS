#ifndef BONDS_HPP
#define BONDS_HPP

#include "globals.hpp"

/**
 * Represents a spring-like connection between two particles
 */
struct Bond
{
    int i;           ///< index of first particle
    int j;           ///< index of second particle
    float restLength;///< creation distance
    bool broken;     ///< if true, this bond is considered broken
};

/**
 * Global vector of all bonds
 */
extern std::vector<Bond> bonds;

/**
 * @brief Creates bonds between nearby particles.
 *        This checks each pair of particles within a threshold
 *        (BOND_CONNECT_DIST) and adds a Bond with restLength = distance.
 */
void generateBonds();

#endif // BONDS_HPP

