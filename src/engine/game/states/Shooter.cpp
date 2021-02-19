#include "Shooter.hpp"
#include <glm/gtx/transform.hpp>
#include "engine/game/registry/Components.hpp"
#include "engine/game/actions/Actions.hpp"
#include <cstdlib>
#include <iostream>

void game::Shooter::update(float _time, float _deltaTime) {
	//ToDo: spawn crates 


	//update position
	registry.execute<Velocity, Transform>([&](Velocity& _velocity, Transform& _transform) {
		if (_transform.position.x >= 10 || _transform.position.x <= -10) {
			_velocity.velocity.x = -_velocity.velocity.x;
		}
		if (_transform.position.y >= 10 || _transform.position.y <= -10) {
			_velocity.velocity.y = -_velocity.velocity.y;
		}
		if (_transform.position.z >= 10 || _transform.position.z <= -10) {
			_velocity.velocity.z = -_velocity.velocity.z;
		}
		_transform.position.x += _velocity.velocity.x * _deltaTime ;
		_transform.position.y += _velocity.velocity.y * _deltaTime ;
		_transform.position.z += _velocity.velocity.z * _deltaTime ;
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
	
	auto _mesh_planet = &mesh_planet;
	auto _mesh_box = &mesh_box;
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
		graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	texture_planet = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);
	texture_box = graphics::Texture2D::load("../resources/textures/cratetex.png", sampler, false);
	//create starting ammount of boxes

	std::srand(666);

	std::vector<Entity> boxes;
	for (int i = 0; i < 400; i++) {
		boxes.push_back(registry.create());
		registry.addComponent<Mesh>(boxes[i], _mesh_box);
		registry.addComponent<Texture>(boxes[i], texture_box);
		registry.addComponent<Transform>(boxes[i], glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(std::rand()%20 -10, std::rand() % 20 -10, std::rand() % 20 - 10));
		registry.addComponent<Velocity>(boxes[i], glm::vec3(std::rand()% 3, std::rand() % 3, std::rand() % 3));
	}


	camera.setView(glm::translate(glm::vec3(0, -5, -20)));
}

bool game::Shooter::getIsFinished() {
	return false;
}
