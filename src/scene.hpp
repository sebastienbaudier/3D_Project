#pragma once

#include "cgp/cgp.hpp"
#include "environment_camera_head/environment_camera_head.hpp"
#include "parcelles/parcelle.hpp"
#include "parcelles/foret/foret.hpp"
#include "parcelles/desert/desert.hpp"
#include "parcelles/ile/ile.hpp"
#include "parcelles/monde-de-l'eau/eau.hpp"
#include "parcelles/montagnes/montagne.hpp"
#include "oiseau.hpp"

extern const int PARCELLE_LENGTH;





// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	scene_environment_camera_head environment; // The specific scene environment that contains a "head camera" (*)
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	cgp::skybox_drawable skybox;

	cgp::timer_basic timer; // A basic timer for the camera animation

	std::vector<parcelle*> parcelles;
	cgp::vec3 translation;

	oiseau_struct oiseau;

	GLuint shader_eau_ile;
	GLuint shader_eau_eau;

	// Parcelles
	parcelle* foret;
	parcelle* ile;
	parcelle* desert;
	parcelle* eau;
	parcelle* montagne;


	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void update_camera(); // Function to call in the animation loop in main (*) suit l'oiseau
	parcelle* parcelle_random_pick(); // Select one parcelle randomly between availables
	int update_translation(const cgp::vec3& camera_position); // Update the vec3 translation which stores the origin coordinate of the parcelle under the camera
	void update_parcelles(int translation_result); // Update vector of parcelles
	void update_parcelles_helper(int nombres[9]);


};





