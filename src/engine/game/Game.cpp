#include "Game.hpp"
#include "Flyer.hpp"
#include "engine/graphics/core/device.hpp"
#include "engine/input/inputmanager.hpp"
#include <GLFW/glfw3.h>
#include <chrono>
#include "engine/utils/meshloader.hpp"
#include <memory>
#include <thread>
#include <engine/game/states/HorizontalSpring.hpp>

game::Game::Game() {
	graphics::Device::initialize(1366, 768, false);
	GLFWwindow *window = graphics::Device::getWindow();
	input::InputManager::initialize(window);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.f, 0.f, 0.01f, 1);
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

	auto flyer = game::Flyer::Flyer();
	game::HorizontalSpring next;
	states.push_back(std::move(_initialState));
	TimePoint beginTimePoint = Clock::now();
	TimePoint previousTimePoint = beginTimePoint;
	Duration targetFrameTime = Duration(1.0 / 60.0);
	int count_states = 1;
	while (!states.empty()|| !glfwWindowShouldClose(graphics::Device::getWindow())) {
		auto now = Clock::now();
		Duration time = now - beginTimePoint;
		Duration dt = now - previousTimePoint;
		if (targetFrameTime > dt) {
			std::this_thread::sleep_for(targetFrameTime - dt);
		}

		// do { now = Clock::now(); } while (now - previousTimePoint < targetFrameTime);
		// dt = now - previousTimePoint;
		previousTimePoint = now;
		GameState* current = states.back().get();
		
		
		glfwPollEvents();
		current->update(time.count(), dt.count());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		current->draw(time.count(), dt.count(), flyer);
		glfwSwapBuffers(graphics::Device::getWindow());

		input::InputManager::update();
		if (current->pushNext()) {
			current->onPause();
			states.push_back(std::make_unique<game::HorizontalSpring>(next));
			current = states.back().get();
			count_states++;
		}

		while (current->getIsFinished() || glfwWindowShouldClose(graphics::Device::getWindow())) {
			states.pop_back();
			count_states--;
			if (count_states!=0) {
				current = states.back().get();
				current->onResume();
			}
			if (count_states == 0) {
				break;
			}
		}
		if (count_states == 0) {
			break;
		}

		
	}
	
}
