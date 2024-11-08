#pragma once

#include "parcelles/parcelle.hpp"
#include "cgp/cgp.hpp"
#include "environment_camera_head/environment_camera_head.hpp"
#include "desert_terrain.hpp"

extern const int PARCELLE_LENGTH;

struct desert_structure: parcelle {
	cgp::mesh_drawable terrain;
	cgp::mesh_drawable cactus;
	std::vector<cgp::vec3> cactus_position;

	void initialize() override;
	void draw(scene_environment_camera_head& environment, cgp::vec3 translation, bool overflown = false) override;
	~desert_structure() override;
};
