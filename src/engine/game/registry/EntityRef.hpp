#pragma once
#include "Entity.hpp"
#include <cstdint>

struct EntityRef {
	Entity entity;
	uint32_t generation;
};
