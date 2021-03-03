#include "Shooter.hpp"
#include <glm/gtx/transform.hpp>
#include "engine/game/registry/Components.hpp"
#include "engine/game/actions/Actions.hpp"
#include <cstdlib>
#include <iostream>
#include <numbers>
#include "engine/input/inputmanager.hpp"
#include "engine/graphics/camera.hpp"
#include "engine/utils/containers/octree.hpp"
#include <GL/glew.h>
#include <GLFW\glfw3.h>


void game::Shooter::update(float _time, float _deltaTime) {
	color++;
	glClearColor((color%300 < 100 ) ? color % 300 /100.f : 0, (color % 300 < 200 && color % 300 >= 100) ? (color % 300 -100)/ 100.f : 0, (color % 300 >= 200 ) ? (color % 300 - 200) / 100.f : 0, 1);
	
	//ToDo: update AABB
	game::Actions::UpdateAABB(registry, camera);
	//ToDo: collisiontest
	//utils::SparseOctree<Entity, 3, float> Collisiontree;


	//ToDo: destroy planets if far away
	//ToDo: spawning crates  
	if (counter_time >= 1.0) {
		
		
		counter_time = 0.0;
		int counter_while = 0;
		while (true) {
			counter_while++;
			if (counter_while > number_boxes){
			
				break;
			}
			bool& visible = registry.getComponentUnsafe<Visibility>(boxes[counter_boxes]).visible;
			if (visible) {
				counter_boxes++;
				if (counter_boxes == number_boxes) {
					counter_boxes = 0;
				}
			}
			else {
				visible = true;				
				break;
			}
		}
	}
	counter_time += _deltaTime;
	
	game::Actions::UpdateCratePosition(registry, _deltaTime);
	game::Actions::UpdateRotation(registry, _deltaTime);

		
	
	//spawning planets acurrate
	if (input::InputManager::isButtonPressed(input::MouseButton::LEFT)) {
		planets.push_back(registry.create());
		registry.addComponent<Mesh>(planets[planets.size() - 1], &mesh_planet);
		registry.addComponent<Texture>(planets[planets.size() - 1], texture_planet);
		registry.addComponent<Transform>(planets[planets.size() - 1], glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(0, 0, 40));
		int speeed = 10;
		registry.addComponent<Velocity>(planets[planets.size() - 1], glm::vec3(camera.toWorldSpace(input::InputManager::getCursorPos()).x*speeed, camera.toWorldSpace(input::InputManager::getCursorPos()).y*speeed, -speeed));
		registry.addComponent<Visibility>(planets[planets.size() - 1], true);
		registry.addComponent<AngularVelocity>(planets[planets.size() - 1], glm::quat(glm::vec3(std::rand() % 360 - 180, std::rand() % 360 - 180, std::rand() % 360 - 180)));
		registry.addComponent<ObjectType>(planets[planets.size() - 1], 1);
	}
	//spawning planets spray
	if (input::InputManager::isButtonPressed(input::MouseButton::RIGHT)) {
		planets.push_back(registry.create());
		registry.addComponent<Mesh>(planets[planets.size() - 1], &mesh_planet);
		registry.addComponent<Texture>(planets[planets.size() - 1], texture_planet);
		registry.addComponent<Transform>(planets[planets.size() - 1], glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(0, 0, 40));
		int speeed = 10;
		registry.addComponent<Velocity>(planets[planets.size() - 1], glm::vec3(camera.toWorldSpace(input::InputManager::getCursorPos()).x * speeed + (std::rand() % 200 - 100)/10, camera.toWorldSpace(input::InputManager::getCursorPos()).y * speeed + (std::rand() % 200 - 100) / 10, -speeed));
		registry.addComponent<Visibility>(planets[planets.size() - 1], true);
		registry.addComponent<AngularVelocity>(planets[planets.size() - 1], glm::quat(glm::vec3(std::rand() % 360 - 180, std::rand() % 360 - 180, std::rand() % 360 - 180)));
		registry.addComponent<ObjectType>(planets[planets.size() - 1], 1);		
	}




}

void game::Shooter::draw(float _time, float _deltaTime) {

	meshRenderer.clear();
	game::Actions::Draw(meshRenderer, registry);

	meshRenderer.present(camera);
}

game::Shooter::Shooter() :			game::GameState(),
									camera(graphics::Camera(90, .1f, 500)),
									mesh_planet(graphics::Mesh("models/sphere.obj")),
									mesh_box(graphics::Mesh("models/crate.obj")),
									meshRenderer(),
									registry() {
	
	//initialise variables
	color = 0;
	counter_time = 1.0;
	counter_boxes = 0;
	number_boxes = 100;
	auto _mesh_planet = &mesh_planet;
	auto _mesh_box = &mesh_box;
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
		graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	texture_planet = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);
	texture_box = graphics::Texture2D::load("../resources/textures/cratetex.png", sampler, false);

	
	std::srand(666);
	//create boxes	
	for (int i = 0; i < number_boxes; i++) {
		boxes.push_back(registry.create());
		registry.addComponent<Visibility>(boxes[i], true);		
		registry.addComponent<Mesh>(boxes[i], _mesh_box);
		registry.addComponent<Velocity>(boxes[i], glm::vec3(std::rand() % 6 - 3, std::rand() % 6 - 3, std::rand() % 6 - 3));
		registry.addComponent<Texture>(boxes[i], texture_box);
		registry.addComponent<Transform>(boxes[i], glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(std::rand()%70 -35, std::rand() % 50 -25, std::rand() % 50 - 25));		
		registry.addComponent<Number>(boxes[i], i);		
		registry.addComponent<AngularVelocity>(boxes[i], glm::quat(glm::vec3(std::rand()%360 - 180, std::rand() % 360 - 180,std::rand() % 360 - 180)));
		registry.addComponent<ObjectType>(boxes[i], 0);
		game::Actions::AddAABB(registry, boxes[i], "models/sphere.obj", camera);		
	}
	
	


	camera.setView(glm::translate(glm::vec3(0, 0, -40)));
}

bool game::Shooter::getIsFinished() {
	return false;
}
