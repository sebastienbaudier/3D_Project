#include "desert.hpp"

using namespace cgp;

void desert_structure::initialize() {
	int N_terrain_samples = 100;
	int number_of_cactus = 5;
	mesh const terrain_mesh = create_desert_mesh(N_terrain_samples, PARCELLE_LENGTH);
	terrain.initialize(terrain_mesh, "terrain");
	terrain.shading.color = { 1.f,1.f,0.2f };
	terrain.shading.phong.specular = 0.0f; // non-specular terrain material

	GLuint const texture_image_id = opengl_load_texture_image("assets/texture_sand.jpg",
		GL_REPEAT,
		GL_REPEAT);

	terrain.texture = texture_image_id;

	// création des billboards
	cactus_position = generate_positions_on_desert(number_of_cactus, PARCELLE_LENGTH);

	cactus.initialize(mesh_load_file_obj("assets/cactus.obj"), "Cactus");
	cactus.shading.color = { 0.f, 0.3f, 0.f };
	cactus.transform.scaling = 0.05;
	cactus.transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 2.f);
	

}

void desert_structure::draw(scene_environment_camera_head& environment, vec3 translation, bool overflown) {
	terrain.transform.translation = translation;
	cgp::draw(terrain, environment);
	for (int i = 0; i < cactus_position.size(); i++) {
		cactus.transform.translation = cactus_position[i] + translation;
		cgp::draw(cactus, environment);
	}
}

desert_structure::~desert_structure() {};
