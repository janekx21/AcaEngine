#include "HorizontalSpring.hpp"
#include "engine/game/GameState.hpp"
#include <engine/graphics/core/device.hpp>
#include <glm/gtx/transform.hpp>
#include <glm\fwd.hpp>

void game::HorizontalSpring::update(float _time, float _deltaTime) {
	modelMatrix = glm::translate(glm::vec3(0.f, 0.f, trans_z));
	trans_z += speed_z * _deltaTime;
	speed_z += -trans_z * 0.005f;
}

void game::HorizontalSpring::draw(float _time, float _deltaTime) {
	meshRenderer.draw(mesh, *texture, HorizontalSpring::modelMatrix);
}

game::HorizontalSpring::HorizontalSpring() : camera(graphics::Camera(90, .1f, 100)), mesh(graphics::Mesh("models/sphere.obj")), modelMatrix() {
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
																	 graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	texture = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);
	meshRenderer = graphics::MeshRenderer();
	speed_z = 10;
	trans_z = 0;
	modelMatrix = glm::translate(glm::vec3(0.f, 0.f, 0.f));
}

bool game::HorizontalSpring::getIsFinished() {
	return false;
}
