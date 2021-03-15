#pragma once
#include <engine\game\registry\ComponentType.hpp>
#include <vector>
struct Archetype {

	std::vector<size_t> types;
	std::vector<ComponentType> components;
	std::vector<uint32_t> entities;
};