#pragma once

#include "cgp/cgp.hpp"

extern const int PARCELLE_LENGTH;

float evaluate_montagne_height(float x, float y);

/** Compute a terrain mesh 
	The (x,y) coordinates of the terrain are set in [0, length].
	The z coordinates of the vertices are computed using evaluate_terrain_height(x,y).
	The vertices are sampled along a regular grid structure in (x,y) directions. 
	The total number of vertices is N*N (N along each direction x/y) 	*/
cgp::mesh create_montagne_mesh(int N, float length);
cgp::mesh create_neige_mesh(int N, float length);

std::vector<cgp::vec3> generate_positions_on_montagne(int N, float terrain_length);
