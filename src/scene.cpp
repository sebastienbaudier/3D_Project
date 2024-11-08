#include "scene.hpp"

using namespace cgp;

const int PARCELLE_LENGTH = 32;


void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	skybox.initialize("assets/skybox/");         // indicate a path where to load the skybox

	// Initial placement of the camera
	environment.camera.position_camera = { 0.5f, 0.5f, 3.0f };
	environment.camera.manipulator_rotate_roll_pitch_yaw(0, Pi/2.0f, 0);
	translation = { 0,0,0 };

	// Parcelles
	foret = new foret_structure();
	foret->initialize();
	ile = new ile_structure();
	ile->initialize();
	desert = new desert_structure();
	desert->initialize();
	eau = new eau_structure();
	eau->initialize();
	montagne = new montagne_structure();
	montagne->initialize();

	for (int i = 0; i < 9; i++)
	{
		parcelles.push_back(parcelle_random_pick());
		parcelles[i]->initialize();
	}

	oiseau.initialize();
}





void scene_structure::display()
{
	draw(skybox, environment);

	// Basic elements of the scene
	environment.light = environment.camera.position();
	environment.t = timer.t;

	update_parcelles(update_translation(environment.camera.position()));
	
	oiseau.draw(environment, timer.t);
	
	parcelles[1]->draw(environment, vec3{ -PARCELLE_LENGTH + 1,-1,0 } + translation);
	parcelles[2]->draw(environment, vec3{ -PARCELLE_LENGTH + 1,PARCELLE_LENGTH - 3,0 } + translation);
	parcelles[3]->draw(environment, vec3{ -1,PARCELLE_LENGTH - 3,0 } + translation);
	parcelles[4]->draw(environment, vec3{ PARCELLE_LENGTH - 3,PARCELLE_LENGTH - 3,0 } + translation);
	parcelles[5]->draw(environment, vec3{ PARCELLE_LENGTH - 3,-1,0 } + translation);
	parcelles[6]->draw(environment, vec3{ PARCELLE_LENGTH - 3,-PARCELLE_LENGTH + 1,0 } + translation);
	parcelles[7]->draw(environment, vec3{ -1,-PARCELLE_LENGTH + 1,0 } + translation);
	parcelles[8]->draw(environment, vec3{ -PARCELLE_LENGTH + 1,-PARCELLE_LENGTH + 1,0 } + translation);
	parcelles[0]->draw(environment, vec3{ -1,-1,0 } + translation, true);
}





// The main function implementing the Flying Mode
void scene_structure::update_camera()
{
	inputs_keyboard_parameters const& keyboard = inputs.keyboard;
	camera_head& camera = environment.camera;

	// The camera moves forward all the time
	// We consider in this example a constant velocity, so the displacement is: velocity * dt * front-camera-vector
	// Let's try to add an acceleration
	float velocity = 3.f;
	if (keyboard.shift)          
		velocity = 6.f;
	float const dt = timer.update();
	vec3 const forward_displacement = velocity * dt * camera.front();
	camera.position_camera += forward_displacement;

	// The camera rotates if we press on the arrow keys
	//  The rotation is only applied to the roll and pitch degrees of freedom.
	float const rotation = 0.5f; // speed of the rotation
	if (keyboard.left)
		camera.manipulator_rotate_roll_pitch_yaw(0, 0, rotation * dt);
	if (keyboard.right)
		camera.manipulator_rotate_roll_pitch_yaw(0, 0, -rotation * dt);
	if (keyboard.down)
		camera.manipulator_rotate_roll_pitch_yaw(0, -rotation * dt, 0);
	if (keyboard.up)
		camera.manipulator_rotate_roll_pitch_yaw(0, rotation * dt, 0);
}





parcelle* scene_structure::parcelle_random_pick()
{
	int number = std::rand() % 5;
	switch (number)
	{
	case 4:
		return montagne;
	case 3:
		return eau;
	case 2:
		return ile;
	case 1:
		return desert;
	default:
		return foret;
	}
}



/**
 * Fonction qui permet d'actualiser le vecteur translation qui indique l'origine de la parcelle actuellement survolée par la caméra.
 * Si on retourne 0, c'est que le vecteur translation n'a pas été modifié (on reste au-dessus de la même parcelle).
 * Sinon, on retourne un entier de 1 à 4 indiquant par quel côté on a quitté la parcelle :
 * ← 1, ↑ 2, → 3 et ↓ 4
*/
int scene_structure::update_translation(const vec3& camera_position) {
	int x = (int)camera_position.x / 30 * 30;
	if (camera_position.x < 0) { x -= 30; }
	int y = (int)camera_position.y / 30 * 30;
	if (camera_position.y < 0) { y -= 30; }

	if (translation.x == x && translation.y == y)
		return 0;

	int retour = 0;
	if (x < translation.x) {
		retour = 1;
	}
	else if (y > translation.y) {
		retour = 2;
	}
	else if (x > translation.x) {
		retour = 3;
	}
	else if (y < translation.y) {
		retour = 4;
	}
	translation = vec3{ x,y,0 };
	return retour;
}





void scene_structure::update_parcelles(int translation_result) {
	if (translation_result == 0) {
		return;
	} else if (translation_result == 1) {
		int argument[9] = { 4,5,6,3,0,7,2,1,8 };
		update_parcelles_helper(argument);
	} else if (translation_result == 2) {
		int argument[9] = { 8,7,6,1,0,5,2,3,4 };
		update_parcelles_helper(argument);
	} else if (translation_result == 3) {
		int argument[9] = { 2,1,8,3,0,7,4,5,6 };
		update_parcelles_helper(argument);
	}
	else if (translation_result == 4) {
		int argument[9] = { 2, 3, 4, 1, 0, 5, 8, 7, 6 };
		update_parcelles_helper(argument);
	}
}





void scene_structure::update_parcelles_helper(int nombres[9]) {
	parcelles[nombres[0]] = parcelles[nombres[3]];
	parcelles[nombres[1]] = parcelles[nombres[4]];
	parcelles[nombres[2]] = parcelles[nombres[5]];
	parcelles[nombres[3]] = parcelles[nombres[6]];
	parcelles[nombres[4]] = parcelles[nombres[7]];
	parcelles[nombres[5]] = parcelles[nombres[8]];
	parcelles[nombres[6]] = parcelle_random_pick();
	parcelles[nombres[7]] = parcelle_random_pick();
	parcelles[nombres[8]] = parcelle_random_pick();
}
