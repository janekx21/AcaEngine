#include <engine/game/registry/Entity.hpp>
#include "engine/game/registry/Registry.hpp"
#include "engine/game/registry/Registry.cpp"
#include <iostream>
#include <map>
struct Size {
	int i;
};
struct Area {
	int length;
	int width;

};


int main(int argc, char *argv[]) {
	game::Registry registry;
	Entity new_entity_1 = registry.create();
	Entity new_entity_2 = registry.create();
	Entity new_entity_3 = registry.create();
	registry.addComponent<Size>(new_entity_1, 3);
	registry.addComponent<Size>(new_entity_2, 3);
	registry.addComponent<Size>(new_entity_3, 3);
	registry.execute<Size>([](const Size& size) {std::cout << size.i << "\n"; });

	registry.erase(new_entity_2);
	registry.erase(new_entity_1);
	registry.erase(new_entity_3);
	registry.print_entities();
	return EXIT_SUCCESS;
}
