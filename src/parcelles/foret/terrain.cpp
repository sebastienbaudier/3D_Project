
#include "terrain.hpp"

using namespace cgp;

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
float evaluate_terrain_height(float x, float y)
{
    vec2 p[4] = { {5, 5}, {20,20}, {12,19}, {21,23} };
    float h[4] = { 3.0f, -1.5f, 1.0f, 2.0f };
    float sigma[4] = { 2.0f, 3.0f, 4.0f, 4.0f };
    float z = 0;
    for (int i = 0; i < 4; i++) {
        float d = norm(vec2(x, y) - p[i]) / sigma[i];
        z += h[i] * std::exp(-d * d);
    }

    return z;
}

mesh create_terrain_mesh(int N, float terrain_length)
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


            // Compute the Perlin noise
            float const noise = noise_perlin({ u, v }, octave, persistency, frequency_gain);

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_terrain_height(x,y) * noise;

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

std::vector<vec3> generate_positions_on_terrain(int N, float terrain_length) {
    std::vector<vec3> positions = {};
    for (int i = 0; i < N; i++) {
        float x = rand_interval(0, terrain_length);
        float y = rand_interval(0, terrain_length);
        float const noise = noise_perlin({ x / terrain_length + 0.5f, y / terrain_length + 0.5f }, octave, persistency, frequency_gain);
        positions.push_back(vec3{ x, y, evaluate_terrain_height(x, y) * noise });
    }
    return positions;
}
