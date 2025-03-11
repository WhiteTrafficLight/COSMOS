#include "clusters.hpp"
#include <queue>

std::vector<Cluster> clusters;

void initSingleCluster()
{
    clusters.clear();
    if(particles.empty())
    {
        std::cout << "No particles to form a cluster.\n";
        return;
    }

    Cluster c;
    // compute center
    Vec3 sum = {0,0,0};
    for(auto &p : particles)
        sum = add(sum, p);

    c.center = mul(sum, 1.0f / particles.size());
    c.velocity = {0,0,0};

    // all particles in one cluster
    c.indices.reserve(particles.size());
    for(int i = 0; i < (int)particles.size(); i++)
        c.indices.push_back(i);

    c.relOffsets.resize(particles.size());
    for(int i = 0; i < (int)particles.size(); i++)
    {
        c.relOffsets[i] = sub(particles[i], c.center);
    }

    clusters.push_back(c);
    std::cout << "initSingleCluster: 1 cluster with " << particles.size() << " particles.\n";
}

void updateCluster(Cluster &cl)
{
    // Gravity on center
    cl.velocity = add(cl.velocity, mul(GRAVITY, DT));
    cl.velocity = mul(cl.velocity, DAMPING);
    cl.center   = add(cl.center, mul(cl.velocity, DT));

    // table collision
    float minOffY = 1e9f;
    for(size_t k=0;k<cl.indices.size();k++)
    {
        if(cl.relOffsets[k].y < minOffY) 
            minOffY = cl.relOffsets[k].y;
    }
    float bottom = cl.center.y + minOffY;
    if(bottom < TABLE_Y)
    {
        cl.center.y += (TABLE_Y - bottom);
        cl.velocity.y = 0;
    }

    // place each particle
    for(size_t k=0; k<cl.indices.size(); k++)
    {
        int idx = cl.indices[k];
        particles[idx] = add(cl.center, cl.relOffsets[k]);
    }
}

void updateAllClusters()
{
    for(auto &cl : clusters)
    {
        updateCluster(cl);
    }
}

void updateClustersAfterCut()
{
    // We'll do a BFS or DFS over the unbroken bonds to find connected components
    clusters.clear();

    int n = (int)particles.size();
    if(n == 0)
        return;

    // Build adjacency from unbroken bonds
    std::vector<std::vector<int>> adj(n);
    for(auto &b : bonds)
    {
        if(!b.broken)
        {
            adj[b.i].push_back(b.j);
            adj[b.j].push_back(b.i);
        }
    }

    // BFS
    std::vector<int> visited(n, -1);
    int clusterId = 0;
    for(int i=0;i<n;i++)
    {
        if(visited[i] != -1) 
            continue;

        std::queue<int>q;
        q.push(i);
        visited[i] = clusterId;

        std::vector<int> localIndices;
        localIndices.push_back(i);

        while(!q.empty())
        {
            int cur = q.front(); 
            q.pop();
            for(int nb : adj[cur])
            {
                if(visited[nb] == -1)
                {
                    visited[nb] = clusterId;
                    q.push(nb);
                    localIndices.push_back(nb);
                }
            }
        }

        // Now localIndices are the particles for this cluster
        Cluster c;
        c.velocity = {0,0,0};

        // compute center
        Vec3 sum = {0,0,0};
        for(auto idx : localIndices)
            sum = add(sum, particles[idx]);

        int count = (int)localIndices.size();
        if(count > 0)
            c.center = mul(sum, 1.0f / count);
        else
            c.center = {0,0,0};

        c.indices = localIndices;
        c.relOffsets.resize(count);
        for(int k=0; k<count; k++)
        {
            int idx = localIndices[k];
            c.relOffsets[k] = sub(particles[idx], c.center);
        }

        clusters.push_back(c);
        clusterId++;
    }

    std::cout << "updateClustersAfterCut: " << clusters.size() << " clusters formed.\n";
    for(size_t cIdx=0; cIdx<clusters.size(); cIdx++)
    {
        std::cout << " Cluster " << cIdx
                  << " has size=" << clusters[cIdx].indices.size() << "\n";
    }
}

