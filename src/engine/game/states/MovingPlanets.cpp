#include "MovingPlanets.hpp"
#include <glm/gtx/transform.hpp>
#include <iostream>

void game::MovingPlanets::update(float _time, float _deltaTime) {
	if (input::InputManager::isKeyPressed(input::Key::ESCAPE)) {
		end = true;
	}
	for (int i = 0; i < planets.size();i++) {
		planets[i].update(_deltaTime);
	}
	
}

void game::MovingPlanets::draw(float _time, float _deltaTime, game::Flyer& _flyer) {
	
	
	meshRenderer.clear();
	/*for (int i = 0; i < planets.size(); i++) {
		meshRenderer.draw(mesh, *textures[i], planets[i].getModel());
	}*/ // doesn't work for some reason
	
	meshRenderer.draw(mesh, *textures[0], planets[0].getModel());
	meshRenderer.draw(mesh, *textures[1], planets[1].getModel());
	meshRenderer.draw(mesh, *textures[2], planets[2].getModel());
	_flyer.update(_deltaTime);
	camera.setView(_flyer.getView());

	meshRenderer.present(camera);
}

bool game::MovingPlanets::pushNext()
{
	return false;
}


game::MovingPlanets::MovingPlanets() : game::GameState(),
camera(graphics::Camera(90, .1f, 2000)),
meshRenderer(),
mesh(graphics::Mesh("models/sphere.obj"))
{
	end = false;

	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
		graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	
	velocity = 10;
	position = 0;
	camera.setView(glm::translate(glm::vec3(0, 0, -5)));

	// Planet(CenterPosition|RotationAxis|RevolutionAxis|RotationSpeed|RevolutionSpeed|Size|Radius)
	planets.push_back(Planet(glm::vec3(0.0f, 0.0f, 0.0f), glm::mat3(glm::rotate(0.4014257279577f, glm::vec3(0, 0, 1))) * glm::vec3(0, 1, 0), glm::vec3(0.f, 1.f, 0.f), 0.02f, 0.f, 3.f, 0.f));
	planets.push_back(Planet(glm::vec3(0.0f, 0.0f, 70.0f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), 0.0f, 0.0f, 20.f, 0.f));
	planets.push_back(Planet(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(1.f, 1.f, 1.f), 0.02f, 0.02f, 1.f, 40.f));
	textures.push_back(graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false));
	textures.push_back(graphics::Texture2D::load("../resources/textures/sun1.png", sampler, false));
	textures.push_back(graphics::Texture2D::load("../resources/textures/moon1.png", sampler, false));
	
}

bool game::MovingPlanets::getIsFinished() {
	return end;
}
