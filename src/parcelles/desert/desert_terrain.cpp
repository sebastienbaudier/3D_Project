
#include "desert_terrain.hpp"

using namespace cgp;

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
float evaluate_desert_height(float x, float y)
{
    float d = norm(vec2(x, y) - vec2(PARCELLE_LENGTH/2.f, PARCELLE_LENGTH/2.f)) / (PARCELLE_LENGTH/5.f);
    float d2 = norm(vec2(x, y) - vec2(PARCELLE_LENGTH / 2.f, PARCELLE_LENGTH / 2.f)) / (PARCELLE_LENGTH / 10.f);
    float z = 4.f * std::exp(-d * d) - 2.f * std::exp(-d2 * d2);
    

    return z;
}

mesh create_desert_mesh(int N, float terrain_length)
{

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N * N);
    terrain.uv.resize(N * N);

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // Compute the real coordinates (x,y) of the terrain 
            float x = u * terrain_length;
            float y = v * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_desert_height(x,y);

            // Store vertex coordinates
            terrain.position[kv + N * ku] = { x,y,z };
            terrain.uv[kv + N * ku] = { ku/10.0f,kv/10.0f };
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(int ku=0; ku<N-1; ++ku)
    {
        for(int kv=0; kv<N-1; ++kv)
        {
            unsigned int idx = kv + N*ku; // current vertex offset

            uint3 triangle_1 = {idx, idx+1+N, idx+1};
            uint3 triangle_2 = {idx, idx+N, idx+1+N};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
	terrain.fill_empty_field(); 

    return terrain;
}

std::vector<vec3> generate_positions_on_desert(int N, float terrain_length) {
    std::vector<vec3> positions = {};
    for (int i = 0; i < N; i++) {
        float x = rand_interval(0, terrain_length);
        float y = rand_interval(0, terrain_length);
        positions.push_back(vec3{ x, y, evaluate_desert_height(x, y) - 0.2f });
    }
    return positions;
}