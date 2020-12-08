#include "HorizontalSpring.hpp"
#include <glm/gtx/transform.hpp>

void game::HorizontalSpring::update(float _time, float _deltaTime) {
	if (input::InputManager::isKeyPressed(input::Key::ESCAPE)) {
		end = true;
	}
	velocity -= position * _deltaTime * 8;
	position += velocity * _deltaTime;
	modelMatrix = glm::translate(glm::vec3(position, 0, 0));
}

void game::HorizontalSpring::draw(float _time, float _deltaTime, game::Flyer &_flyer) {
	meshRenderer.clear();
	meshRenderer.draw(mesh, *texture, modelMatrix);
	_flyer.setView(glm::vec3(0,0,-5), glm::vec3(0, 0, -1));
	camera.setView(_flyer.getView());
	
	meshRenderer.present(camera);
}

bool game::HorizontalSpring::pushNext()
{
	return false;
}


game::HorizontalSpring::HorizontalSpring() : game::GameState(),
																						 camera(graphics::Camera(90, .1f, 100)),																						 
																						 meshRenderer(),
																						 mesh(graphics::Mesh("models/sphere.obj")),
																						 modelMatrix(glm::identity<glm::mat4>()) 
{
	end = false;
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
																	 graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);

	texture = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);
	velocity = 10;
	position = 0;
	camera.setView(glm::translate(glm::vec3(0, 0, -5)));
}

bool game::HorizontalSpring::getIsFinished() {
	return end;
}
