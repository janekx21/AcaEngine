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
#include <engine/game/states/Start.hpp>
#include <engine/game/states/Shooter.hpp>
#include <engine/game/states/ExampleScene.hpp>

int main()
{
	game::Game game;
	//game::HorizontalSpring state;
	game.run(std::make_unique<game::ExampleScene>());

	//game.run(std::make_unique<game::HorizontalSpring>(state));

	return EXIT_SUCCESS;
}
