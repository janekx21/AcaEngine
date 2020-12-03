#pragma once
#include "GameState.hpp"
#include <chrono>
#include <vector>
#include <memory>
#include <GLFW/glfw3.h>
namespace game {
	class Game
	{
	public:
		// initialize global resources


		Game();
		// finalize global resources
		~Game();
		// Start the game loop with _initialState on the stack.
		void run(std::unique_ptr<GameState> _initialState);

	private:
		std::vector<std::unique_ptr<GameState>> states;
		GLFWwindow* window;
	};
}// namespace game
