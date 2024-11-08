#pragma once

#include "parcelles/parcelle.hpp"
#include "cgp/cgp.hpp"
#include "environment_camera_head/environment_camera_head.hpp"
#include "eau_terrain.hpp"
extern const int PARCELLE_LENGTH;

struct eau_structure: parcelle {
	scene_environment_camera_head environment; // The specific scene environment associated to the deforming shader (*)
	
	cgp::mesh_drawable global_frame;    // The standard global frame
	cgp::mesh_drawable grid;            // The grid that is deformed by the shader
	cgp::mesh_drawable cascade;

	cgp::mesh_drawable terrain;
	// We keep two elements in memory:
	//  - The mesh structure whose buffers can be modified at every frame
	//  - The mesh_drawable structure (as usual) that is displayed and dynamically updated from the mesh.
	cgp::mesh mesh_shape;


	cgp::timer_basic timer; // A basic timer for the procedural deformation of the shape
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	void initialize();
	void draw(scene_environment_camera_head& environment, cgp::vec3 translation, bool overflown = false) override;
	
};
