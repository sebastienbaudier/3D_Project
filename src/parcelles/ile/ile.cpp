#include "ile.hpp"

using namespace cgp;

void ile_structure::initialize() {
	int N_terrain_samples = 100;
	mesh const terrain_mesh = create_ile_mesh(N_terrain_samples, PARCELLE_LENGTH);
	terrain.initialize(terrain_mesh, "terrain");
	terrain.shading.color = { 1.f,1.f,0.2f };
	terrain.shading.phong.specular = 0.0f; // non-specular terrain material

	GLuint const texture_image_id = opengl_load_texture_image("assets/texture_sand.jpg",
		GL_REPEAT,
		GL_REPEAT);

	terrain.texture = texture_image_id;

	GLuint const shader_eau_ile = opengl_load_shader("shaders/eau_ile/vert.glsl", "shaders/eau_ile/frag.glsl");

	grid.initialize(mesh_primitive_grid({ 0,0,0 }, { PARCELLE_LENGTH,0,0 }, { PARCELLE_LENGTH,PARCELLE_LENGTH,0 }, { 0,PARCELLE_LENGTH,0 }, 200, 200), "Eau");
	grid.shader = shader_eau_ile;
	grid.shading.color = { 0.f, 0.6f, 0.6f };

	// palmier
	palmier.initialize(mesh_load_file_obj("assets/palmier.obj"));
	palmier.transform.scaling = 0.04f;
	palmier.transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 3.14 / 2);
	palmier.texture = opengl_load_texture_image("assets/textpalmier.jpg");

}


void ile_structure::draw(scene_environment_camera_head& environment, vec3 translation, bool overflown) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	terrain.transform.translation = translation;
	cgp::draw(terrain, environment);
	grid.transform.translation = translation - vec3(0, 0, 0.1f);
	cgp::draw(grid, environment);
	palmier.transform.translation = vec3(PARCELLE_LENGTH / 2.f, PARCELLE_LENGTH / 2.f, evaluate_ile_height(PARCELLE_LENGTH / 2.f, PARCELLE_LENGTH / 2.f)) + translation;
	cgp::draw(palmier, environment);
	
	glDisable(GL_BLEND);
}

ile_structure::~ile_structure() {};
