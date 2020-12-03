#include "Game.hpp"
#include "engine/graphics/camera.hpp"
#include "engine/graphics/core/device.hpp"
#include "engine/graphics/core/shader.hpp"
#include "engine/graphics/core/texture.hpp"
#include "engine/graphics/renderer/mesh.hpp"
#include "engine/graphics/renderer/meshrenderer.hpp"
#include "engine/input/inputmanager.hpp"
#include "engine/math/directions.h"
#include "engine/utils/meshloader.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <map>
#include <thread>

// CRT's memory leak detection
#ifndef NDEBUG
#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <engine/game/Game.hpp>
#endif
#include <engine\graphics\core\hSpring.h>
/*
using namespace std::chrono_literals;

class Flyer {
public:
	void update(float dt) {
		applyRotation();
		applyFriction(dt);
		auto move = getMove();
		move = glm::inverse(rotation) * move;
		applyAcceleration(move, dt);
	}

	glm::vec3 getPosition() { return position; }
	glm::quat getRotation() { return rotation; }

private:
	glm::vec3 position = glm::vec3{-5,1,0};
	glm::quat rotation = glm::identity<glm::quat>();
	glm::vec3 velocity = math::zero;
	const float friction = 10.f;
	const float acceleration = 100.f;
	const std::map<input::Key, glm::vec3> keyToDirection = {
					{input::Key::W, math::forward},
					{input::Key::S, math::backwards},
					{input::Key::A, math::left},
					{input::Key::D, math::right},
	};

	glm::vec3 getMove() {
		auto move = math::zero;
		for (auto&[key, direction] : keyToDirection) {
			if (input::InputManager::isKeyPressed(key)) {
				move += direction;
			}
		}
		if (glm::length(move) > 1) {
			move = glm::normalize(move);
		}
		return move;
	}

	void applyFriction(float dt) {
		if (glm::length(velocity) > 0) {
			velocity += -glm::normalize(velocity) * glm::length(velocity) * friction * dt;
		}
	}

	void applyAcceleration(glm::vec3 move, float dt) {
		velocity += move * acceleration * dt;
		position += velocity * dt;
	}

	void applyRotation() {
		auto deltaMouse = input::InputManager::getDeltaCursorPos();
		auto deltaRot = glm::angleAxis(deltaMouse.x * .002f, math::up); // * glm::angleAxis(-deltaMouse.y * .002f, rotation * math::right);
		rotation *= deltaRot;
	}
};

class TimeManager {
	typedef std::chrono::high_resolution_clock clock;
	typedef std::chrono::duration<float> duration;

public:
	TimeManager() : startMoment(clock::now()), lastResetMoment(startMoment) {
	}

	float getTime() {
		duration elapsed = clock::now() - startMoment;
		return elapsed.count();
	}

	float getDeltaTime() { return deltaTime; }

	void resetDeltaTime() {
		auto now = clock::now();
		duration elapsed = now - lastResetMoment;
		deltaTime = elapsed.count();
		lastResetMoment = now;
	}

private:
	clock::time_point startMoment;
	clock::time_point lastResetMoment;
	float deltaTime = 0;
};
*/

int main(int argc, char *argv[]) {
#ifndef NDEBUG
#if defined(_MSC_VER)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//	_CrtSetBreakAlloc(2760);
#endif
#endif
	game::Game game;
	hSpring test;
	game.run(std::make_unique<hSpring>(test));
/*
	graphics::Device::initialize(1366, 768, false);
	GLFWwindow *window = graphics::Device::getWindow();
	input::InputManager::initialize(window);

	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
																	 graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	auto texture = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);
	auto texture2 = graphics::Texture2D::load("../resources/textures/sun1.png", sampler, false);
	auto mesh = graphics::Mesh("models/sphere.obj");
	auto mesh2 = graphics::Mesh("models/cube.obj");

	auto camera = graphics::Camera(90, .1f, 100);

	auto flyer = Flyer();
	auto time = TimeManager();
	auto meshRenderer = graphics::MeshRenderer();

	// permanent settings
	glClearColor(.25f, .2f, .2f, 1);
	glEnable(GL_DEPTH_TEST);

	float speed_z = 10;
	float trans_z = 0;
	

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		float dt = time.getDeltaTime();

		flyer.update(dt);

		camera.setView(glm::mat4(flyer.getRotation()) * glm::translate(-flyer.getPosition()));
		// Spring example

	
		auto modelMatrix = glm::translate(glm::vec3(0.f, 0.f, 0.f))
											 * glm::rotate(glm::pi<float>() * time.getTime() * .3f, glm::vec3(0, 1, 0));
		
		// physics_spring

		/*modelMatrix = glm::translate(glm::vec3(0.f, 0.f, trans_z));
		trans_z += speed_z *dt;
		speed_z += -trans_z *0.005f;
		*/

		//physics_bounce

		/*modelMatrix *= glm::translate(glm::vec3(0.f, abs(trans_z),0.f ));
		trans_z += speed_z * dt;
		speed_z += -trans_z * 0.5f;
		auto modelMatrix2 = glm::translate(glm::vec3(0.f, -2.f, 0.f));
		meshRenderer.draw(mesh2, *texture2, modelMatrix2);

		meshRenderer.draw(mesh, *texture, modelMatrix);
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		meshRenderer.present(camera);
		glfwSwapBuffers(window);

		meshRenderer.clear();
		time.resetDeltaTime();
		input::InputManager::update();
		std::this_thread::sleep_for(10ms);
	}

	utils::MeshLoader::clear();
	graphics::Device::close();
	return EXIT_SUCCESS;
	*/
}
