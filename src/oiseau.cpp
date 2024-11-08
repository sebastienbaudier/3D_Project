#include "oiseau.hpp"


using namespace cgp;





void oiseau_struct::initialize() {
	// oiseau
	mesh_drawable corps;
	mesh_drawable aile11;
	mesh_drawable aile21;
	mesh_drawable aile12;
	mesh_drawable aile22;
	mesh_drawable head;
	mesh_drawable oeil1;
	mesh_drawable oeil2;
	mesh_drawable bec;
	corps.initialize(mesh_primitive_ellipsoid({ 1.0f,2.0f,1.0f }, { 0,0,10.0f }), "Corps"); corps.transform.scaling = 0.2f;
	head.initialize(mesh_primitive_sphere(0.15f), "Head");
	bec.initialize(mesh_primitive_cone(0.08f, 0.15f, { 0.0f,0.1f,0.02f }, { 0,1.0f,0 }), "Bec");
	oeil1.initialize(mesh_primitive_sphere(0.02f, { 0.05f,0.09f,0.11f }), "Oeil1");
	oeil2.initialize(mesh_primitive_sphere(0.02f, { -0.05f,0.09f,0.11f }), "Oeil2");
	aile11.initialize(mesh_primitive_quadrangle({ 0,0,0 }, { 0,2,0 }, { 1.5,2,0 }, { 1.5,0,0 }), "Aile11"); aile11.transform.scaling = 0.25f;
	aile12.initialize(mesh_primitive_quadrangle({ 0,0,0 }, { 0,2,0 }, { 2,1.2f,0 }, { 2,0.8f,0 }), "Aile12"); aile12.transform.scaling = 0.25f;
	aile21.initialize(mesh_primitive_quadrangle({ 0,0,0 }, { 0,2,0 }, { -1.5,2,0 }, { -1.5,0,0 }), "Aile21"); aile21.transform.scaling = 0.25f;
	aile22.initialize(mesh_primitive_quadrangle({ 0,0,0 }, { 0,2,0 }, { -2,1.2f,0 }, { -2,0.8f,0 }), "Aile22"); aile22.transform.scaling = 0.25f;
	vec3 color1 = { 1.0f, 1.0f, 1.0f };
	vec3 color2 = { 1.0f,0.5f,0.0f };
	vec3 color3 = { 0,0,0 };
	GLuint const texture_corps = opengl_load_texture_image("assets/bleu.jpg",
		GL_REPEAT,
		GL_REPEAT);
	GLuint const texture_ailes = opengl_load_texture_image("assets/bleu.jpg",
		GL_REPEAT,
		GL_REPEAT);
	GLuint const texture_head = opengl_load_texture_image("assets/bleu.jpg",
		GL_REPEAT,
		GL_REPEAT);
	corps.texture = texture_corps;
	head.texture = texture_head;
	bec.shading.color = color1;
	oeil1.shading.color = color3;
	oeil2.shading.color = color3;
	aile11.texture = texture_ailes;
	aile12.texture = texture_ailes;
	aile21.texture = texture_ailes;
	aile22.texture = texture_ailes;
	hierarchy.add(corps);
	hierarchy.add(head, "Corps", { 0.0f,0.4f,2.2f });
	hierarchy.add(aile11, "Corps", { 0.1f,-0.2f,2.0f });
	hierarchy.add(aile12, "Aile11", { 0.25f * 1.5f,0.0f,0.0f });
	hierarchy.add(aile21, "Corps", { -0.1f,-0.2f,2.0f });
	hierarchy.add(aile22, "Aile21", { -0.25f * 1.5f,0.0f,0.0f });
	hierarchy.add(bec, "Head");
	hierarchy.add(oeil1, "Head");
	hierarchy.add(oeil2, "Head");
}





void oiseau_struct::draw(scene_environment_camera_head& environment, float time) {
	/*
	// Pour faire bouger l'oiseau avec la cam�ra deux solutions :
	vec3 direction = normalize(environment.camera.front());
	vec3 position = { 0.0f,0.1f,4.0f };
	vec3 oui = normalize(position);
	rotation_transform a = rotation_transform::between_vector(oui, direction);
	position = a * position;
	hierarchy["Corps"].transform.translation = environment.camera.position() + position + vec3(0, 0, -5.f);
	vec3 const front = normalize(environment.camera.front()); // j'ai laiss� la possibilit� de tourner m�me vers le haut : moins r�aliste mais ne crash pas
	vec3 const right = environment.camera.right();
	// Rotation such that R*{1,0,0} = right-direction, R*{0,1,0} = front-direction
	rotation_transform R = rotation_transform::between_vector({ 1,0,0 }, { 0,1,0 }, right, front);
	hierarchy["Corps"].transform.rotation = R;
	hierarchy["Corps"].transform.scaling = 2.0f;
	*/

	vec3 const front = normalize(environment.camera.front()); // j'ai laiss� la possibilit� de tourner m�me vers le haut : moins r�aliste mais ne crash pas
	vec3 const right = environment.camera.right();
	// Rotation such that R*{1,0,0} = right-direction, R*{0,1,0} = front-direction
	rotation_transform R = rotation_transform::between_vector({ 1,0,0 }, { 0,1,0 }, right, front);

	hierarchy["Corps"].transform.translation = vec3(0, 0, 0);
	hierarchy["Corps"].transform.rotation = R;
	hierarchy["Corps"].transform.translation = environment.camera.position() + 2.0f * environment.camera.front() + vec3(0, 0, -2.5);


	hierarchy["Head"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, cos(time * 2) / 5);
	hierarchy["Aile11"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, cos(time * 4) / 3);
	hierarchy["Aile12"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, cos(time * 4));
	hierarchy["Aile21"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, -cos(time * 4) / 3);
	hierarchy["Aile22"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, -cos(time * 4));

	// This function must be called before the drawing in order to propagate the deformations through the hierarchy
	hierarchy.update_local_to_global_coordinates();

	// Draw the hierarchy as a single mesh
	cgp::draw(hierarchy, environment);
}
