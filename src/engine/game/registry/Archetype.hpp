#pragma once
#include <vector>
#include <engine\game\registry\ComponentType.hpp>
struct Archetype {
	std::vector < size_t > types;
	std::vector < ComponentType > components;
	std::vector < uint32_t > entities;
};