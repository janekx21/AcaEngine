#include <engine/game/registry/Entity.hpp>
#include "engine/game/registry/Registry.hpp"
#include "engine/game/registry/Registry.cpp"
#include <iostream>
#include <map>
#include <engine/game/registry/Registry.hpp>
#include <vector>
#include <engine/game/tests/testutils.hpp>
#include <engine/game/Game.hpp>
#include <engine/game/states/MovingPlanets.hpp>
#include <engine/game/states/Start.hpp>
#include <engine/game/states/Shooter.hpp>
#include <engine/game/states/HorizontalSpring.hpp>

int main()
{
	game::Game game;
	
	game::Start state;
	//game::HorizontalSpring state;
	//game::Shooter state;
	//game.run(std::make_unique<game::Shooter>(state));
	//game.run(std::make_unique<game::HorizontalSpring>(state));
	game.run(std::make_unique<game::Start>(state));
	return EXIT_SUCCESS;
}
