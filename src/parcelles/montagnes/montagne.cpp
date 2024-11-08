

#include "montagne.hpp"

using namespace cgp;

void montagne_structure::initialize() {
	int N_terrain_samples = 100;
	mesh const terrain_mesh = create_montagne_mesh(N_terrain_samples, PARCELLE_LENGTH);
	terrain.initialize(terrain_mesh, "terrain");
	terrain.shading.phong.specular = 0.0f; // non-specular terrain material

	GLuint const texture_image_id = opengl_load_texture_image("assets/montagne.jpg",
		GL_REPEAT,
		GL_REPEAT);

	terrain.texture = texture_image_id;

	mesh const neige_mesh = create_neige_mesh(N_terrain_samples, PARCELLE_LENGTH);
	neige.initialize(neige_mesh, "neige");
	neige.shading.phong.specular = 0.0f; // non-specular terrain material

	GLuint const texture_neige = opengl_load_texture_image("assets/neige.jpg",
		GL_REPEAT,
		GL_REPEAT);

	neige.texture = texture_neige;


}

void montagne_structure::draw(scene_environment_camera_head& environment, vec3 translation, bool overflown) {
	terrain.transform.translation = translation;
	cgp::draw(terrain, environment);
	neige.transform.translation = translation;
	cgp::draw(neige, environment);

}

montagne_structure::~montagne_structure() {};
