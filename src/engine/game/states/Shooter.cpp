#include "Shooter.hpp"
#include <glm/gtx/transform.hpp>
#include "engine/game/registry/Components.hpp"
#include "engine/game/actions/Actions.hpp"
#include <cstdlib>
#include <iostream>
#include <numbers>
#include "engine/input/inputmanager.hpp"
#include "engine/graphics/camera.hpp"

void game::Shooter::update(float _time, float _deltaTime) {
	game::Actions::UpdateAABB(registry, camera);

	counter_boxes -= game::Actions::CollisionCheck(registry);

	game::Actions::deleteFarAwayPlanets(registry, renderdistance);
	// spawning crates  
	if (counter_time >= 0.05) {
		counter_time = 0.0;
		game::Shooter::addBox();
	}
	counter_time += _deltaTime;

	game::Actions::UpdateCratePosition(registry, _deltaTime);
	game::Actions::UpdateRotation(registry, _deltaTime);

	// spawning planets acurrate
	if (input::InputManager::isButtonPressed(input::MouseButton::LEFT)) {
		game::Shooter::addPlanet(false);
	}
	// spawning planets spray
	if (input::InputManager::isButtonPressed(input::MouseButton::RIGHT)) {
		game::Shooter::addPlanet(true);
	}
}

void game::Shooter::draw(float _time, float _deltaTime) {
	meshRenderer.clear();
	game::Actions::Draw(meshRenderer, registry);

	meshRenderer.present(camera);
}

game::Shooter::Shooter() :			game::GameState(),
									camera(graphics::Camera(90, 2.f, renderdistance)),
									mesh_planet(graphics::Mesh("models/sphere.obj")),
									mesh_box(graphics::Mesh("models/crate.obj")),
									meshRenderer(),
									registry() {	
	//initialise variables
	std::srand(666);
	counter_time = 0;
	counter_boxes = 0;	
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
		graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	texture_planet = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);
	texture_box = graphics::Texture2D::load("../resources/textures/cratetex.png", sampler, false);	
	camera.setView(glm::translate(glm::vec3(0, 0, -40)));
}

bool game::Shooter::getIsFinished() {
	return false;
}

void game::Shooter::addBox() {
	if (counter_boxes <= 1000) {
		Entity ent = registry.create();
		registry.addComponent<Visibility>(ent, true);
		registry.addComponent<Mesh>(ent, &mesh_box);
		registry.addComponent<Velocity>(ent, glm::vec3(std::rand() % 6 - 3, std::rand() % 6 - 3, std::rand() % 6 - 3));
		registry.addComponent<Texture>(ent, texture_box);
		registry.addComponent<Transform>(ent, glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(std::rand() % 70 - 35, std::rand() % 50 - 25, std::rand() % 50 - 25));
		registry.addComponent<AngularVelocity>(ent, glm::quat(glm::vec3(std::rand() % 360 - 180, std::rand() % 360 - 180, std::rand() % 360 - 180)));
		registry.addComponent<ObjectType>(ent, 0);
		game::Actions::AddAABB(registry, ent, "models/crate.obj", camera, false);
		counter_boxes++;
	}	
}

void game::Shooter::addPlanet(bool random)
{
	Entity ent = registry.create();
	registry.addComponent<Mesh>(ent, &mesh_planet);
	registry.addComponent<Texture>(ent, texture_planet);
	registry.addComponent<Transform>(ent, glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(0, 0, 40));
	int speeed = 100;
	if(random){
		registry.addComponent<Velocity>(ent, glm::vec3(camera.toWorldSpace(input::InputManager::getCursorPos()).x * speeed + (std::rand() % (2*speeed) - speeed)/10 , camera.toWorldSpace(input::InputManager::getCursorPos()).y * speeed + (std::rand() % (2 * speeed) - speeed)/10, -speeed));
	}
	else {
		registry.addComponent<Velocity>(ent, glm::vec3(camera.toWorldSpace(input::InputManager::getCursorPos()).x * speeed, camera.toWorldSpace(input::InputManager::getCursorPos()).y * speeed, -speeed));

	}	
	registry.addComponent<Visibility>(ent, true);
	registry.addComponent<AngularVelocity>(ent, glm::quat(glm::vec3(std::rand() % 360 - 180, std::rand() % 360 - 180, std::rand() % 360 - 180)));
	registry.addComponent<ObjectType>(ent, 1);
	game::Actions::AddAABB(registry, ent, "models/sphere.obj", camera, true);
}
