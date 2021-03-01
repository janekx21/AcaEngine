#include "Shooter.hpp"
#include <glm/gtx/transform.hpp>
#include "engine/game/registry/Components.hpp"
#include "engine/game/actions/Actions.hpp"
#include <cstdlib>
#include <iostream>
#include <numbers>

void game::Shooter::update(float _time, float _deltaTime) {
	//ToDo: spawn crates 
	if (counter_time >= 1.0) {
		counter_time = 0.0;
		int counter_while = 0;
		while (true) {
			counter_while++;
			if (counter_while > number_boxes){
				break;
			}
			bool& visible = registry.getComponentUnsafe<Visibility>(boxes[counter_boxes]).visible;
			std::cout << "visible/counter" << visible << "/" << counter_boxes << "\n";
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
	
	
	//update position
	registry.execute</*Number,*/ Velocity, Transform, Number/*, AngularVelocity*/>([&](/*Number& _number,*/ Velocity& _velocity, Transform& _transform, Number& _number/*, AngularVelocity& _angularVelocity*/) {
		std::cout << "pos" << _transform.position.x << "\n";		
		if (_transform.position.x >= 10) {
			_velocity.velocity.x = -_velocity.velocity.x;
		}
		
		if (_transform.position.x <= -10){
			_velocity.velocity.x = -_velocity.velocity.x;
		}
		std::cout << "pos" << _transform.position.y << "\n";
		if (_transform.position.y >= 10 || _transform.position.y <= -10) {
			_velocity.velocity.y = -_velocity.velocity.y;
		}
		std::cout << "pos" << _transform.position.z << "\n";		
		if (_transform.position.z >= 10 || _transform.position.z <= -10) {
			_velocity.velocity.z = -_velocity.velocity.z;
		}
		_transform.position.x += _velocity.velocity.x * _deltaTime ;
		_transform.position.y += _velocity.velocity.y * _deltaTime ;
		_transform.position.z += _velocity.velocity.z * _deltaTime ;

		//_transform.rotation *= glm::quat(1, _angularVelocity.velocity * std::numbers::pi, _angularVelocity.velocity * std::numbers::pi, _angularVelocity.velocity * std::numbers::pi);
		});



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
	counter_time = 1.0;
	counter_boxes = 0;
	number_boxes = 10;
	auto _mesh_planet = &mesh_planet;
	auto _mesh_box = &mesh_box;
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
		graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	texture_planet = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);
	texture_box = graphics::Texture2D::load("../resources/textures/cratetex.png", sampler, false);
	//create starting ammount of boxes

	std::srand(666);

	
	for (int i = 0; i < number_boxes; i++) {
		boxes.push_back(registry.create());
		registry.addComponent<Visibility>(boxes[i], false);
		
		
		registry.addComponent<Mesh>(boxes[i], _mesh_box);
		registry.addComponent<Texture>(boxes[i], texture_box);
		registry.addComponent<Transform>(boxes[i], glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(std::rand()%20 -10, std::rand() % 20 -10, std::rand() % 20 - 10));
		registry.addComponent<Velocity>(boxes[i], glm::vec3(std::rand()% 6 - 3, std::rand() % 6 - 3, std::rand() % 6 - 3));
		registry.addComponent<Number>(boxes[i], i);
		
		//registry.addComponent<AngularVelocity>(boxes[i], std::rand() % 10 / 10);
		//
	}
	


	camera.setView(glm::translate(glm::vec3(0, -5, -20)));
}

bool game::Shooter::getIsFinished() {
	return false;
}
