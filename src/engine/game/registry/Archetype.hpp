#pragma once
#include <engine\game\registry\ComponentType.hpp>
#include <vector>
struct Archetype {

	std::vector<size_t> types; // TODO move to ComponentType
	std::vector<ComponentType> components;
	std::vector<uint32_t> entities;// hashmap better maybe
};