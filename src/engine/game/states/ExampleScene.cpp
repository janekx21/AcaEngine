#include "ExampleScene.hpp"
#include "MovingPlanets.hpp"
#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>

game::ExampleScene::ExampleScene() : camera(44, .1, 10),
																		 meshRenderer(),
																		 scene(graphics::Mesh("models/scene.obj")) {
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
																	 graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	white = graphics::Texture2D::load("../resources/textures/white.png", sampler, false);

	auto location = glm::vec3(-.5, .6, 1.33);
	auto rotation = glm::quat(glm::vec3(0, glm::radians(-133.0), 0));
	camera.setView(glm::translate(-location) * glm::toMat4(rotation));
}

void game::ExampleScene::update(float _time, float _deltaTime) {
	if (input::InputManager::isKeyPressed(input::Key::ESCAPE)) {
		shouldFinish = true;
	}
}

void game::ExampleScene::draw(float _time, float _deltaTime) {
	meshRenderer.draw(scene, *white, glm::identity<glm::mat4>());

	meshRenderer.present(camera);
	meshRenderer.clear();
}
