#include "Game.hpp"
#include <GLFW\glfw3.h>
#include <chrono>
#include "engine/graphics/core/device.hpp"
#include "engine/input/inputmanager.hpp"
#include "engine/utils/meshloader.hpp"

game::Game::Game() {
	graphics::Device::initialize(1366, 768, false);
	GLFWwindow *window = graphics::Device::getWindow();
	input::InputManager::initialize(window);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.f, 1.f, 1.f, 1);
}

game::Game::~Game() {
	while (!states.empty()) {
		states.pop_back();
	}
	states.clear();
	utils::MeshLoader::clear();
	graphics::Device::close();
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
		Duration time = now - beginTimePoint;
		Duration dt = now - previousTimePoint;
		if (targetFrameTime > dt) {
			std::this_thread::sleep_for(targetFrameTime - dt);
		}

		// do { now = Clock::now(); } while (now - previousTimePoint < targetFrameTime);
		// dt = now - previousTimePoint;
		previousTimePoint = now;

		GameState &current = *states.back();
		glfwPollEvents();
		input::InputManager::update();
		current.update(time.count(), dt.count());
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		current.draw(time.count(), dt.count());
		
		glfwSwapBuffers(graphics::Device::getWindow());

		while (current.getIsFinished() || glfwWindowShouldClose(graphics::Device::getWindow())) {
			states.pop_back();
			if (states.empty()) break;
			current = *states.back();
			current.onResume();
		}
	}
}
