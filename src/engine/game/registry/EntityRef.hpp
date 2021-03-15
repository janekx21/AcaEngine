#pragma once
#include "Entity.hpp"

struct EntityRef {
	Entity ent;
	uint32_t generation;
};
