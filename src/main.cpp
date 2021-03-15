#include "engine/game/Game.hpp"
#include "engine/game/states/Start.hpp"

int main()
{
	game::Game game;
	game.run(std::make_unique<game::Start>());
	return EXIT_SUCCESS;
}
