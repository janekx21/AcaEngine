#include "HorizontalSpring.hpp"
#include "engine/game/actions/Actions.hpp"
#include "engine/game/registry/Components.hpp"
#include <GLFW\glfw3.h>
#include <chrono>
#include <cmath>
#include <glm/gtx/transform.hpp>
#include <numbers>

void game::HorizontalSpring::update(float _time, float _deltaTime) {
	float colorValue = abs((static_cast<int>(std::floor(_time)) % 200 - 100) / 100.f);
	glClearColor(colorValue, 0.5, 1 - colorValue, 1);
	if (input::InputManager::isKeyPressed(input::Key::ESCAPE)) {
		isFinished = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	registry.execute<Velocity, Transform>([&](Velocity &_velocity, Transform &_transform) {
		_velocity.velocity.x = _velocity.velocity.x - (_transform.position.x * _deltaTime);
		_transform.position += _velocity.velocity * _deltaTime;
	});
}

void game::HorizontalSpring::draw(float _time, float _deltaTime) {
	game::Actions::cameraMovement(pos, rot, _deltaTime, camera, cameraStartPosition);
	meshRenderer.clear();
	game::Actions::Draw(meshRenderer, registry);
	meshRenderer.present(camera);
}

game::HorizontalSpring::HorizontalSpring() : game::GameState(),
																						 camera(graphics::Camera(90, .1f, 500)),
																						 mesh(graphics::Mesh("models/sphere.obj")),
																						 meshRenderer(),
																						 registry() {
	pos = glm::vec3(0, 0, 0);
	rot = 0;
	cameraStartPosition = glm::vec3(0, 5, 10);
	isFinished = false;
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
																	 graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	texture = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);

	for (int i = 0; i < 100; i++) {
		game::HorizontalSpring::createPlanet(i);
	}
	camera.setView(glm::translate(glm::vec3(0, 5, -10)));
}

bool game::HorizontalSpring::getIsFinished() {
	return isFinished;
}

bool game::HorizontalSpring::getIsMenue() {
	return false;
}

int game::HorizontalSpring::goToState() {
	return 0;
}

void game::HorizontalSpring::createPlanet(int i) {
	Entity _ent = registry.create();
	registry.addComponent<Visibility>(_ent, true);
	registry.addComponent<Mesh>(_ent, &mesh);
	registry.addComponent<Texture>(_ent, texture);
	registry.addComponent<Transform>(_ent, glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(5 * cos(i * (2 * std::numbers::pi / 40)), 0, -i));
	registry.addComponent<Velocity>(_ent, glm::vec3(5 * cos(i * (2 * std::numbers::pi / 40)), 0, 0));
	// triangle function
	// registry.addComponent<Transform>(_ent, glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(abs(i % 40 - 20) - 10, 0, -i));
	// registry.addComponent<Velocity>(_ent, glm::vec3(abs(i % 40 - 20) - 10, 0, 0));
}
