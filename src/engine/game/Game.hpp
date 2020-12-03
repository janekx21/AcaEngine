#pragma once
#include "GameState.hpp"
#include <memory>
#include <vector>
namespace game {
	class Game
	{
	public:
		// Initialize global resources
		Game();

		// Finalize global resources
		~Game();

		// Start the game loop with _initialState on the stack.
		void run(std::unique_ptr<GameState> _initialState);

	private:
		std::vector<std::unique_ptr<GameState>> states;
	};
}
