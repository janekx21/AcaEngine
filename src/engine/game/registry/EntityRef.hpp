#pragma once
#include "Entity.hpp"
#include <cstdint>

struct EntityRef {
	Entity ent;
	uint32_t generation;
};
