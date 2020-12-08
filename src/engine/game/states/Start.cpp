#include "Start.hpp"
#include <iostream>



void game::Start::update(float _time, float _deltaTime)
{
	if (input::InputManager::isKeyPressed(input::Key::ESCAPE)) {
		end = true;
	}

	glm::vec2 A = glm::vec2(530, 536);
	glm::vec2 B = glm::vec2(530, 230);
	glm::vec2 C = glm::vec2(834, 384);
	glm::vec2 X = glm::vec2(input::InputManager::getCursorPos().x, input::InputManager::getCursorPos().y);

	// compare Area to see if cursor in green field (area is still hardcoded could be implemented as affine transformation)
	if (0.5 * abs((B - A).x * (C - A).y - (B - A).y * (C - A).x) >= 0.5 * abs((B - X).x * (C - X).y - (B - X).y * (C - X).x) + 0.5 * abs((C - X).x * (A - X).y - (C - X).y * (A - X).x) + 0.5 * abs((A - X).x * (B - X).y - (A - X).y * (B - X).x)
		&& input::InputManager::isButtonPressed(input::MouseButton::LEFT)) {
		start = true;
	}

}

void game::Start::draw(float _time, float _deltaTime, Flyer& _flyer)
{
	meshRenderer.clear();
	meshRenderer.draw(mesh, *texture, modelMatrix);
	_flyer.setView(glm::vec3(0, 0, -5), glm::vec3(0, 0, -1));
	camera.setView(_flyer.getView());

	meshRenderer.present(camera);
	

}

bool game::Start::pushNext()
{
	return start;
}



game::Start::Start() : game::GameState(),
	camera(graphics::Camera(90, .1f, 100)),
	meshRenderer(),
	mesh(graphics::Mesh("models/triangle.obj")),
	modelMatrix(glm::identity<glm::mat4>())
{
	start = false;
	end = false;

	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
		graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);

	texture = graphics::Texture2D::load("../resources/textures/green1.png", sampler, false);

	camera.setView(glm::translate(glm::vec3(0, 0, -5)));
}

bool game::Start::getIsFinished()
{
	return end;
}
