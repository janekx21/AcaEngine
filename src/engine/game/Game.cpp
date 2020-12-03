#include "Game.hpp"
#include "engine/graphics/core/device.hpp"
#include <GLFW\glfw3.h>
#include <chrono>
#include <memory>
#include "engine/input/inputmanager.hpp"
#include <thread>

game::Game::Game() {
	graphics::Device::initialize(1366, 768, false);
	GLFWwindow *window = graphics::Device::getWindow();
	input::InputManager::initialize(window);
}

game::Game::~Game() {
	while (!states.empty()) {
		GameState &current = *states.back();
		states.pop_back();
	}
	states.clear();
}

void game::Game::run(std::unique_ptr<GameState> _initialState) {
	using TimePoint = std::chrono::steady_clock::time_point;
	using Clock = std::chrono::high_resolution_clock;
	using Duration = std::chrono::duration<float>;

	states.push_back(std::move(_initialState));
	TimePoint beginTimePoint = Clock::now();
	TimePoint previousTimePoint = beginTimePoint;
	Duration targetFrameTime = Duration(1.0 / 60.0);
	while (!states.empty()) {
		auto now = Clock::now();
		Duration time = beginTimePoint - now;
		Duration dt = now - previousTimePoint;
		if (targetFrameTime > dt) {
			std::this_thread::sleep_for(targetFrameTime - dt);
		}

		// do { now = Clock::now(); } while (now - previousTimePoint < targetFrameTime);
		// dt = now - previousTimePoint;
		// previousTimePoint = now;
		// time += dt.count();

		GameState &current = *states.back();
		current.update(time.count(), dt.count());
		current.draw(time.count(), dt.count());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(graphics::Device::getWindow());
		if (current.getIsFinished()) {
			states.pop_back();
			states.back()->onResume();
		}
	}
}
