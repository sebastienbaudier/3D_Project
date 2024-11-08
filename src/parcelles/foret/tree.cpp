#define M_PI 3.14159265358979323846f

#include "tree.hpp"

using namespace cgp;


mesh create_cylinder_mesh(float r, float h)
{

    // Number of samples of the terrain is N x N
    int N = 20;

    mesh cylinder; // temporary terrain storage (CPU only)
    cylinder.position.resize(N * N);
    cylinder.uv.resize(N * N);

    // Fill terrain geometry
    for (int ku = 0; ku < N; ++ku)
    {
        for (int kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku / (N - 1.0f);
            float v = kv / (N - 1.0f);

            // Compute the local surface function
            vec3 p = { r * std::cos(2 * Pi * u), r * std::sin(2 * Pi * u), h * v };
            vec2 uv = { u,v };

            // Store vertex coordinates
            cylinder.position[kv + N * ku] = p;
            cylinder.uv[kv + N * ku] = { u,v };
        }
    }

    // Generate triangle organization
    for (int ku = 0; ku < N - 1; ++ku)
    {
        for (int kv = 0; kv < N - 1; ++kv)
        {
            int idx = kv + N * ku;

            uint3 triangle_1 = { idx, idx + 1 + N, idx + 1 };
            uint3 triangle_2 = { idx, idx + N, idx + 1 + N };

            cylinder.connectivity.push_back(triangle_1);
            cylinder.connectivity.push_back(triangle_2);
        }
    }

    cylinder.fill_empty_field();
    return cylinder;
}

mesh create_cone_mesh(float radius, float height, float z_offset)
{
	int N = 20;
	float theta = 2 * M_PI / float(N);
	mesh m;
	m.position.push_back(vec3{ 0.0f, 0.0f, height + z_offset });
	m.position.push_back(vec3{ 0.0f, 0.0f, z_offset });
	for (int i = 0; i < N; i++) {
		m.position.push_back(vec3{ radius * std::cos(theta * i), radius * std::sin(theta * i), z_offset });
	}
	for (int i = 2; i < N + 1; i++) {
		m.connectivity.push_back(uint3{ i, i + 1, 0 });
		m.connectivity.push_back(uint3{ i + 1, i, 1 });
	}
	m.connectivity.push_back(uint3{ N + 1, 2, 0 });
	m.connectivity.push_back(uint3{ 2, N + 1, 1 });

	m.fill_empty_field();
	return m;
}

mesh create_tree()
{
	float h = 0.7f; // trunk height
	float r = 0.1f; // trunk radius

	// Create a brown trunk
	mesh trunk = create_cylinder_mesh(r, h);
	trunk.color.fill({ 0.4f, 0.3f, 0.3f });


	// Create a green foliage from 3 cones
	mesh foliage = create_cone_mesh(4 * r, 6 * r, 0.0f);      // base-cone
	foliage.push_back(create_cone_mesh(4 * r, 6 * r, 2 * r));   // middle-cone
	foliage.push_back(create_cone_mesh(4 * r, 6 * r, 4 * r));   // top-cone
	foliage.position += vec3(0, 0, h);                 // place foliage at the top of the trunk
	foliage.color.fill({ 0.4f, 0.6f, 0.3f });

	// The tree is composted of the trunk and the foliage
	mesh tree = trunk;
	tree.push_back(foliage);

	return tree;
}
