#include "foret.hpp"

using namespace cgp;

void foret_structure::initialize() {
	int N_terrain_samples = 100;
	int number_of_trees = 20;
	mesh const terrain_mesh = create_terrain_mesh(N_terrain_samples, PARCELLE_LENGTH);
	terrain.initialize(terrain_mesh, "terrain");
	terrain.shading.color = { 0.6f,0.85f,0.5f };
	terrain.shading.phong.specular = 0.0f; // non-specular terrain material

	tree_positions = generate_positions_on_terrain(number_of_trees, PARCELLE_LENGTH);
	for (int i = 0; i < number_of_trees; i++) {
		tree_positions[i].z = tree_positions[i].z - 0.1f;
	}
	// création d'un arbre en deux parties
	float h = 0.7f; // trunk height
	float r = 0.1f; // trunk radius
	mesh trunk_mesh = create_cylinder_mesh(r, h);
	trunk.initialize(trunk_mesh, "trunk");
	GLuint const texture_trunk = opengl_load_texture_image("assets/Tronc.jpg",
		GL_REPEAT,
		GL_REPEAT);
	trunk.texture = texture_trunk;
	mesh foliage1_mesh = mesh_primitive_cone(4 * r, 6 * r, { 0.0f,0.0f,h }, { 0,0,1 }, true);
	mesh foliage2_mesh = mesh_primitive_cone(4 * r, 6 * r, { 0.0f,0.0f,2 * r + h }, { 0,0,1 }, true);
	mesh foliage3_mesh = mesh_primitive_cone(4 * r, 6 * r, { 0.0f,0.0f,4 * r + h }, { 0,0,1 }, true);
	foliage1.initialize(foliage1_mesh, "foliage");
	foliage2.initialize(foliage2_mesh, "foliage");
	foliage3.initialize(foliage3_mesh, "foliage");
	GLuint const texture_foliage = opengl_load_texture_image("assets/feuillage.jpg",
		GL_REPEAT,
		GL_REPEAT);
	foliage1.texture = texture_foliage;
	foliage2.texture = texture_foliage;
	foliage3.texture = texture_foliage;

	GLuint const texture_image_id = opengl_load_texture_image("assets/texture_grass.jpg",
		GL_REPEAT,
		GL_REPEAT);

	terrain.texture = texture_image_id;

	
	// création des billboards
	grass_position = generate_positions_on_terrain(number_of_trees, PARCELLE_LENGTH);
	fleur_position = generate_positions_on_terrain(number_of_trees, PARCELLE_LENGTH);
	fleur2_position = generate_positions_on_terrain(number_of_trees, PARCELLE_LENGTH);

	mesh quad_mesh = mesh_primitive_quadrangle({ -0.2f, 0, 0 }, { 0.2f,0,0 }, { 0.2f,0,0.4f }, { -0.2f,0,0.4f });
	quad.initialize(quad_mesh, "Quad 1");
	quad.texture = opengl_load_texture_image("assets/grass.png");
	
	mesh quad_mesh1 = mesh_primitive_quadrangle({ -0.15f,0,0 }, { 0.15f,0,0 }, { 0.15f,0,0.3f }, { -0.15f,0,0.3f });
	quad_1.initialize(quad_mesh1, "Quad_1");
	quad_1.texture = opengl_load_texture_image("assets/fleur.png");
	
	mesh quad_mesh2 = mesh_primitive_quadrangle({ -0.1f,0,0 }, { 0.1f,0,0 }, { 0.1f,0,0.2f }, { -0.1f,0,0.2f });
	quad_2.initialize(quad_mesh2, "Quad_1");
	quad_2.texture = opengl_load_texture_image("assets/R.png");

	//rocher tout fait
	/*rocher.initialize(mesh_load_file_obj("assets/palmier.obj"));
	rocher_position = generate_positions_on_terrain(1, PARCELLE_LENGTH);
	rocher.transform.scaling = 0.1f;
	rocher.transform.rotation= rotation_transform::from_axis_angle({ 1,0,0 },3.14/2);
	rocher.texture = opengl_load_texture_image("assets/textpalmier.jpg");*/
}

void foret_structure::draw(scene_environment_camera_head& environment, vec3 translation, bool overflown) {
	terrain.transform.translation = translation;
	cgp::draw(terrain, environment);

	
	for (int i = 0; i < tree_positions.size(); i++) {
		trunk.transform.translation = tree_positions[i]+translation;
		cgp::draw(trunk, environment);
		foliage1.transform.translation = tree_positions[i] + translation;
		cgp::draw(foliage1, environment);
		foliage2.transform.translation = tree_positions[i] + translation;
		cgp::draw(foliage2, environment);
		foliage3.transform.translation = tree_positions[i] + translation;
		cgp::draw(foliage3, environment);
		/*rocher.transform.translation = rocher_position[i] + translation;
		cgp::draw(rocher, environment);*/
	}
	if (overflown) {
		display_semiTransparent(environment, translation);
	}
	
	
}

foret_structure::~foret_structure() {};

void foret_structure::display_semiTransparent(scene_environment_camera_head& environment,vec3 translation)
{
	// Enable use of alpha component as color blend ing for transparent elements
	//  alpha = current_color.alpha
	//  new color = previous_color * alpha + current_color * (1-alpha)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable depth buffer writing
	//  - Transparent elements cannot use depth buffer
	//  - They are supposed to be display from furest to nearest elements
	glDepthMask(false);

	// Re-orient the grass shape to always face the camera direction
	vec3 const front = normalize(environment.camera.front()); // j'ai laissé la possibilité de tourner meme vers le haut: moins réaliste mais ne crash pas
	vec3 const right = environment.camera.right();
	// Rotation such that R*{1,0,0} = right-direction, R*{0,1,0} = front-direction
	rotation_transform R = rotation_transform::between_vector({ 1,0,0 }, { 0,1,0 }, right, front);
	quad.transform.rotation = R;
	quad_1.transform.rotation = R;
	quad_2.transform.rotation = R;
	for (int i = 0; i < grass_position.size(); i++) {
		quad.transform.translation = grass_position[i]+translation;
		cgp::draw(quad, environment);
		quad_1.transform.translation = fleur_position[i]+translation;
		cgp::draw(quad_1, environment);
		quad_2.transform.translation = fleur2_position[i]+translation;
		cgp:: draw(quad_2, environment);
	}
	


	// Don't forget to re-activate the depth-buffer write
	glDepthMask(true);
	glDisable(GL_BLEND);
}
