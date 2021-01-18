#include "HorizontalSpring.hpp"
#include <glm/gtx/transform.hpp>

void game::HorizontalSpring::update(float _time, float _deltaTime) {
	velocity -= position * _deltaTime * 8;
	position += velocity * _deltaTime;
	modelMatrix = glm::translate(glm::vec3(position, 0, 0));
}

void game::HorizontalSpring::draw(float _time, float _deltaTime) {
	meshRenderer.clear();
	meshRenderer.draw(mesh, *texture, modelMatrix);
	meshRenderer.present(camera);
}

game::HorizontalSpring::HorizontalSpring() : game::GameState(),
																						 camera(graphics::Camera(90, .1f, 100)),
																						 mesh(graphics::Mesh("models/sphere.obj")),
																						 meshRenderer(),
																						 modelMatrix(glm::identity<glm::mat4>()) {
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
																	 graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	texture = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);
	velocity = 10;
	position = 0;
	camera.setView(glm::translate(glm::vec3(0, 0, -5)));
}

bool game::HorizontalSpring::getIsFinished() {
	return false;
}
