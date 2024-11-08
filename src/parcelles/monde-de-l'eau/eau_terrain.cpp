
#include "eau_terrain.hpp"

using namespace cgp;

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
/*float evaluate_roche_height(float x, float y)
{
    if ((y <= l) || (y >= PARCELLE_LENGTH - l) || (x <= l) || (x >= PARCELLE_LENGTH - l) ){
        float r = norm(vec2(x, y) - vec2(PARCELLE_LENGTH / 2.f, PARCELLE_LENGTH / 2.f));
    }
    float r = norm(vec2(x, y) - vec2(PARCELLE_LENGTH / 2.f, PARCELLE_LENGTH / 2.f));
    float r0 = PARCELLE_LENGTH / 2.f - 3.f;
    float seuil = r < r0 ? -4.f : 2.f * (std::tanh((r - r0) * 5) - 1);
    float d = norm(vec2(x, y) - vec2(PARCELLE_LENGTH / 2.f, PARCELLE_LENGTH / 2.f)) / (PARCELLE_LENGTH / 4.f);
    float z = seuil + 5.f * std::exp(-d * d);
    

    return z;
}*/
float evaluate_roche_height(float x, float y)
{
    float z = 0;
    float r = norm(vec2(x, y) - vec2(PARCELLE_LENGTH / 2.f, PARCELLE_LENGTH / 2.f));
    float r0 = PARCELLE_LENGTH / 2.f - 3.f;
    // float seuil = r < r0 ? -4.f : 2.f * (std::tanh((r - r0) * 5) - 1);
    if (r < 15) {
    float d = norm(vec2(x, y) - vec2(PARCELLE_LENGTH / 2.f, PARCELLE_LENGTH / 2.f)) / (PARCELLE_LENGTH / 4.f);
    z = -2.f * (std::tanh((3.0f) * 5) - 1) + 6.f * std::exp(-4.0f) + 2.f * (std::tanh((r - r0) * 5) - 1) - 6.f * std::exp(-(16*16+1)/8);
}
    else {
    z = 0;
}

    return z;
}

mesh create_roche_mesh(int N, float terrain_length)
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
            const float persistency = 0.35f;
            const float frequency_gain = 8.0f;
            const int octave = 6;
            float const noise = noise_perlin({ u, v }, octave, persistency, frequency_gain);

            // Compute the surface height function at the given sampled coordinate
           float z = evaluate_roche_height(x, y) * noise;
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
