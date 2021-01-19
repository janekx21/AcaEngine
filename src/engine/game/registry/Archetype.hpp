#pragma once

#include "ComponentArray.hpp"
#include "Entity.hpp"
#include <typeindex>
#include <vector>

template<int SIZE>
struct Archetype {
	std::vector<size_t> types;
	std::vector<ComponentArray<SIZE>> components;
	std::vector<Entity> entities;

	static size_t hashCode(const std::vector<size_t> &types) {
		size_t hash = 0;
		for (auto &value : types) {
			hash += value;
		}
		return hash;
	}
};
