#pragma once
#include <cstdint>

struct Entity {
	uint32_t id;

	inline bool operator==(const Entity& other) const{ return id == other.id; }
};
