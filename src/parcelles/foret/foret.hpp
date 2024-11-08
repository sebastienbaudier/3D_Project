#pragma once

#include "parcelles/parcelle.hpp"
#include "cgp/cgp.hpp"
#include "environment_camera_head/environment_camera_head.hpp"
#include "terrain.hpp"
#include "tree.hpp"

extern const int PARCELLE_LENGTH;

struct foret_structure: parcelle {
	std::vector<cgp::vec3> tree_positions;
	std::vector<cgp::vec3> grass_position;
	std::vector<cgp::vec3> fleur_position;
	std::vector<cgp::vec3> fleur2_position;
	std::vector<cgp::vec3> rocher_position;
	cgp::mesh_drawable terrain;
	cgp::mesh_drawable tree;
	cgp::mesh_drawable quad;
	cgp::mesh_drawable quad_1;
	cgp::mesh_drawable quad_2;
	cgp::mesh_drawable trunk;
	cgp::mesh_drawable foliage1;
	cgp::mesh_drawable foliage2;
	cgp::mesh_drawable foliage3;
	cgp::mesh_drawable rocher;

	void initialize() override;
	void draw(scene_environment_camera_head& environment, cgp::vec3 translation, bool overflown = false) override;
	void display_semiTransparent(scene_environment_camera_head& environment, cgp::vec3 translation = cgp::vec3{ 0,0,0 });
	~foret_structure() override;
};
