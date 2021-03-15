#include "Game.hpp"
#include "engine/game/states/ExampleScene.hpp"
#include "engine/game/states/HorizontalSpring.hpp"
#include "engine/game/states/Shooter.hpp"
#include "engine/graphics/core/device.hpp"
#include "engine/graphics/core/opengl.hpp"
#include "engine/input/inputmanager.hpp"
#include "engine/utils/meshloader.hpp"
#include <GLFW/glfw3.h>
#include <chrono>

game::Game::Game() {
	graphics::Device::initialize(1366, 768, false);
	GLFWwindow *window = graphics::Device::getWindow();
	input::InputManager::initialize(window);
	graphics::glCall(glEnable, GL_DEPTH_TEST);
	graphics::glCall(glClearColor, 0.8f, 0.8f, 0.8f, 1);
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
	game::HorizontalSpring horizontalSpring;
	game::Shooter shooter;
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

		previousTimePoint = now;

		glfwPollEvents();
		input::InputManager::update();
		states.back()->update(time.count(), dt.count());

		if (states.back()->goToState() == 1 && states.back()->getIsMenue()) {
			states.back()->onPause();
			states.push_back(std::move(std::make_unique<game::HorizontalSpring>(horizontalSpring)));
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		if (states.back()->goToState() == 3 && states.back()->getIsMenue()) {
			states.back()->onPause();
			states.push_back(std::move(std::make_unique<game::Shooter>(shooter)));
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		if (states.back()->goToState() == 2 && states.back()->getIsMenue()) {
			states.back()->onPause();
			states.push_back(std::move(std::make_unique<game::ExampleScene>()));
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

		graphics::glCall(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		states.back()->draw(time.count(), dt.count());

		glfwSwapBuffers(graphics::Device::getWindow());

		while (states.back()->getIsFinished() || glfwWindowShouldClose(graphics::Device::getWindow())) {
			states.pop_back();

			if (states.empty()) {
				break;
			}
			states.back()->onResume();
		}
	}
}
