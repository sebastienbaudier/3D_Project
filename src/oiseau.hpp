#pragma once

#include "cgp/cgp.hpp"
#include "environment_camera_head/environment_camera_head.hpp"

struct oiseau_struct {

	cgp::hierarchy_mesh_drawable hierarchy;

	void initialize();
	void draw(scene_environment_camera_head& environment, float time);
};