#include "eau.hpp"

using namespace cgp;

void eau_structure::initialize()
{
	int N_terrain_samples = 100;
	mesh const terrain_mesh = create_roche_mesh(N_terrain_samples, PARCELLE_LENGTH);
	terrain.initialize(terrain_mesh, "terrain");
	terrain.shading.phong.specular = 0.0f; // non-specular terrain material

	GLuint const texture_image_id = opengl_load_texture_image("assets/roche.jpg",
		GL_REPEAT,
		GL_REPEAT);

	terrain.texture = texture_image_id;

	// Specific Shader (*)
	// ***************************************** //
	// Load a new custom shader that computes the deformation
	GLuint const shader_deformation = opengl_load_shader("shaders/deformation/vert.glsl", "shaders/deformation/frag.glsl");


	// Create the Shapes
	// ***************************************** //

	// The standard frame
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	// Create the grid to be deformed (*)
	grid.initialize(mesh_primitive_grid({ 0,0,0 }, { PARCELLE_LENGTH,0,0 }, { PARCELLE_LENGTH,PARCELLE_LENGTH,0 }, { 0,PARCELLE_LENGTH,0 }, 200, 200), "Grid");
	grid.shader = shader_deformation;
	grid.shading.color = { 0.5f, 0.95f, 1.0f };
	cascade.initialize(mesh_primitive_cylinder(0.2f, { PARCELLE_LENGTH / 2.f,PARCELLE_LENGTH / 2.f,-1.0f }, { PARCELLE_LENGTH / 2.f,PARCELLE_LENGTH / 2.f,10 }, 200, 200, true), "Cascade");
	GLuint const texture_cascade = opengl_load_texture_image("assets/cascade.jpg",
		GL_REPEAT,
		GL_REPEAT);
	cascade.texture = texture_cascade;

	
}

void eau_structure::draw(scene_environment_camera_head& environment, vec3 translation, bool overflown)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Displaying the shape deformed by the shader
	cascade.transform.translation = translation;
	cgp::draw(cascade, environment);
	terrain.transform.translation = translation;
	cgp::draw(terrain, environment);
	grid.transform.translation = translation + vec3{ 0,0,-0.5f };
	cgp::draw(grid, environment); // Display the grid deformed by the shader (*)
	glDisable(GL_BLEND);
}
