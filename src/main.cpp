#include <engine/game/registry/Entity.hpp>
#include "engine/game/registry/Registry.hpp"
#include "engine/game/registry/Registry.cpp"
#include <iostream>
#include <map>
struct Size {
	int i;
};


int main(int argc, char *argv[]) {
	game::Registry registry;
	Entity new_entity = registry.create();
	registry.addComponent<Size, int>(new_entity, 3);
	registry.erase(new_entity);
	return EXIT_SUCCESS;
}
