#pragma once

#include "cgp/cgp.hpp"
#include "environment_camera_head/environment_camera_head.hpp"

struct parcelle {
	virtual void initialize() = 0;
	virtual void draw(scene_environment_camera_head& environment, cgp::vec3 translation, bool overflown = false) = 0;
	virtual ~parcelle() {};
};
