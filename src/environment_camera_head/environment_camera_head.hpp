#pragma once

#include "cgp/cgp.hpp"

extern const int PARCELLE_LENGTH;

// Standard scene environment but replacing the common rotating camera with a camera_head
//  camera_head is a camera model that rotates around its own position - this appropriate to model free-flight.
struct scene_environment_camera_head {

	// Standard parameters (same as basic environment)
	cgp::vec3 background_color;
	cgp::camera_projection projection;
	cgp::vec3 light;  // stores the light position
	float t;         // The time is sent to the shader to compute a procedural deformation


	// Change the standard rotating camera to a "head camera" more appropriate to model free-flight (*)
	cgp::camera_head camera;

	scene_environment_camera_head();
};

// Send the uniform parameters of the scene with light 
// Function automatically called when using draw([mesh_drawable], scene_environment_camera_head);
void opengl_uniform(GLuint shader, scene_environment_camera_head const& environment);
