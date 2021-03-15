#include "Start.hpp"
#include "engine/graphics/core/opengl.hpp"
#include <chrono>
#include <engine/game/actions/Actions.hpp>


void game::Start::update(float _time, float _deltaTime) {
	nextState = 0;
	float c_value = abs((int(_time * 20) % 200 - 100) / 100.f);
	glClearColor(1 - c_value, 0.5, c_value, 1);

	if (input::InputManager::isKeyPressed(input::Key::ESCAPE)) {
		isFinished = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	glm::vec2 X = glm::vec2(input::InputManager::getCursorPos().x, input::InputManager::getCursorPos().y);
	if (X.x >= 299 && X.x <= 452 && X.y >= 307 && X.y <= 460 && input::InputManager::isButtonPressed(input::MouseButton::LEFT)) {
		nextState = 1;
	}

	if (X.x >= 606 && X.x <= 759 && X.y >= 307 && X.y <= 460 && input::InputManager::isButtonPressed(input::MouseButton::LEFT)) {
		nextState = 2;
	}

	if (X.x >= 914 && X.x <= 1067 && X.y >= 307 && X.y <= 460 && input::InputManager::isButtonPressed(input::MouseButton::LEFT)) {
		nextState = 3;
	}
}

void game::Start::draw(float _time, float _deltaTime) {
	meshRenderer.clear();
	game::Actions::Draw(meshRenderer, registry);
	meshRenderer.present(camera);
}

game::Start::Start() : game::GameState(),
											 camera(graphics::Camera(90, .1f, 100)),
											 meshRenderer(),
											 mesh(graphics::Mesh("models/Square1.obj")) {
	nextState = 0;
	isFinished = false;
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
																	 graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	texture_white = graphics::Texture2D::load("../resources/textures/texture_white.png", sampler, false);
	texture_blue = graphics::Texture2D::load("../resources/textures/texture_blue.png", sampler, false);
	texture_orange = graphics::Texture2D::load("../resources/textures/texture_orange.png", sampler, false);

	camera.setView(glm::translate(glm::vec3(0, 0, -5)));

	Entity triangle1 = registry.create();
	registry.addComponent<Mesh>(triangle1, &mesh);
	registry.addComponent<Texture>(triangle1, texture_orange);
	registry.addComponent<Transform>(triangle1, glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(-4, 0, 0));
	registry.addComponent<Visibility>(triangle1, true);
	Entity triangle2 = registry.create();
	registry.addComponent<Mesh>(triangle2, &mesh);
	registry.addComponent<Texture>(triangle2, texture_white);
	registry.addComponent<Transform>(triangle2, glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));
	registry.addComponent<Visibility>(triangle2, true);
	Entity triangle3 = registry.create();
	registry.addComponent<Mesh>(triangle3, &mesh);
	registry.addComponent<Texture>(triangle3, texture_blue);
	registry.addComponent<Transform>(triangle3, glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(4, 0, 0));
	registry.addComponent<Visibility>(triangle3, true);
}

bool game::Start::getIsFinished() {
	return isFinished;
}

bool game::Start::getIsMenu() {
	return true;
}

int game::Start::goToState() {
	return nextState;
}
