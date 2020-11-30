#pragma once
#include "GameState.hpp"
#include <chrono>
#include <vector>
#include <memory>
class Game
{
public:
	// initialize global resources
	float clock;
	float delta_t;

	Game();
	// finalize global resources
	~Game();
	// Start the game loop with _initialState on the stack.
	void run(std::unique_ptr<GameState> _initialState);

private:
	std::vector<GameState*> states;
};

