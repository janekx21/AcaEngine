#include "HorizontalSpring.hpp"
#include <glm/gtx/transform.hpp>

void game::HorizontalSpring::update(float _time, float _deltaTime) {
	speed_z += -trans_z * 0.005f;
	trans_z += speed_z * _deltaTime;
	// modelMatrix = glm::translate(glm::vec3(0.f, 0.f, trans_z));
}

void game::HorizontalSpring::draw(float _time, float _deltaTime) {
	meshRenderer.clear();
	meshRenderer.draw(mesh, *texture, modelMatrix);
	meshRenderer.present(camera);
}

game::HorizontalSpring::HorizontalSpring() : game::GameState(), camera(graphics::Camera(90, .1f, 100)), mesh(graphics::Mesh("models/sphere.obj")), modelMatrix(glm::identity<glm::mat4>()) {
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
																	 graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	texture = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);
	meshRenderer = graphics::MeshRenderer();
	speed_z = 10;
	trans_z = 0;
	// modelMatrix = glm::translate(glm::vec3(0.f, 0.f, 0.f));
}

bool game::HorizontalSpring::getIsFinished() {
	return false;
}
