#include "Game.hpp"
#include "GameState.hpp"
#include "engine/graphics/core/device.hpp"
#include "memory"
#include <GLFW\glfw3.h>
#include <chrono>
#include <engine/input/inputmanager.hpp>
#include <thread>
#include <vector>

game::Game::Game()
{
	graphics::Device::initialize(1366, 768, false);
	GLFWwindow* window = graphics::Device::getWindow();
	input::InputManager::initialize(window);
}

game::Game::~Game()
{
	while (!states.empty()) {
		GameState& current = *states.back();
		current.Cleanup();
		states.pop_back();
	}
	states.clear();
	//cleanup();

}

void game::Game::run(std::unique_ptr<GameState> _initialState)
{
	states.push_back(_initialState);
	using clock = std::chrono::high_resolution_clock;
	using delta_t = std::chrono::duration<float>;
	std::chrono::steady_clock::time_point t = clock::now();
	float time = 0;
	delta_t targetFT = std::chrono::milliseconds(17);
	while (!states.empty()) {
		GameState& current = *states.back();
		auto now = clock::now();
		delta_t dt = now - t;
		if (targetFT - dt > std::chrono::milliseconds(1)) {
			std::this_thread::sleep_for(--std::chrono::floor<std::chrono::milliseconds>(targetFT - dt));
		}
		do { now = clock::now(); } while (now - t < targetFT);
		dt = now - t;
		t = now;
		time += dt.count();


		current.update(time, dt.count());
		current.draw(time, dt.count());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
		if (current.isFinished()) {
			states.pop_back();
			GameState& current = *states.back();
			current.onResume();

		}

	}
}

