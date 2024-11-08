#pragma once

#include "parcelles/parcelle.hpp"
#include "cgp/cgp.hpp"
#include "environment_camera_head/environment_camera_head.hpp"
#include "ile_terrain.hpp"

extern const int PARCELLE_LENGTH;

struct ile_structure: parcelle {
	cgp::mesh_drawable terrain;
	cgp::mesh_drawable grid;
	cgp::mesh_drawable palmier;

	void initialize() override;
	void draw(scene_environment_camera_head& environment, cgp::vec3 translation, bool overflown = false) override;
	~ile_structure() override;
};
