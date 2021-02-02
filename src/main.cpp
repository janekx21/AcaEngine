#include <engine/game/registry/Entity.hpp>
#include "engine/game/registry/Registry.hpp"
#include "engine/game/registry/Registry.cpp"
#include <iostream>
#include <map>
#include <engine/game/registry/Registry.hpp>
#include <vector>
#include <engine/game/tests/testutils.hpp>
#include <engine/game/Game.hpp>
#include <engine/game/states/HorizontalSpring.hpp>
#include <engine/game/states/MovingPlanets.hpp>

int main()
{
	game::Game game;
	game::HorizontalSpring state;
	//game::MovingPlanets state;
	game.run(std::make_unique<game::HorizontalSpring>(state));
	//game.run(std::make_unique<game::MovingPlanets>(state));
	game.~Game();

	return EXIT_SUCCESS;
}
