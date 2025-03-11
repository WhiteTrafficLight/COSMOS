#ifndef CLUSTERS_HPP
#define CLUSTERS_HPP

#include "globals.hpp"
#include "bonds.hpp"
#include <vector>

/**
 * A single cluster or fragment of the apple
 * - indices: which particle indices belong to this cluster
 * - center : center of mass
 * - velocity : velocity of this cluster if we treat it as a rigid body
 * - relOffsets : each particle's position relative to center
 */
struct Cluster
{
    std::vector<int> indices;
    Vec3 center;
    Vec3 velocity;
    std::vector<Vec3> relOffsets;
};

/**
 * Global list of all clusters in the scene
 */
extern std::vector<Cluster> clusters;

/**
 * @brief Initialize a single cluster containing all particles
 */
void initSingleCluster();

/**
 * @brief Update the cluster's position (center) under gravity, collisions,
 *        then update each particle = center + relOffset
 */
void updateCluster(Cluster &cl);

/**
 * @brief Update all clusters. Typically called if we are not in cutting mode.
 */
void updateAllClusters();

/**
 * @brief After bond breakage, do a DFS or BFS to re-split the apple into
 *        multiple clusters. This function re-fills the 'clusters' vector.
 */
void updateClustersAfterCut();

#endif // CLUSTERS_HPP

