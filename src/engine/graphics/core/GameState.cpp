
#include "Game.hpp"
#include "GameState.hpp"
#include "memory"
#include <vector>

Game::Game()
{
	using clock = std::chrono::high_resolution_clock;
	using delta_t = std::chrono::duration<float>;
}

Game::~Game()
{
	//cleanup();

}

void Game::run(std::unique_ptr<GameState> _initialState)
{
	while (!states.empty()) {
		GameState& current = *states.back();

		// .. update, render state

		if (current.isFinished()) {
			states.pop_back();
		}

	}
}

