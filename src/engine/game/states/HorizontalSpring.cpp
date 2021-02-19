#include "HorizontalSpring.hpp"
#include <glm/gtx/transform.hpp>
#include "engine/game/registry/Components.hpp"
#include "engine/game/actions/Actions.hpp"

void game::HorizontalSpring::update(float _time, float _deltaTime) {
	
	registry.execute<Velocity, Transform>([&](Velocity& _velocity, Transform& _transform) {
		_velocity.velocity.x = _velocity.velocity.x - (_transform.position.x * _deltaTime );
		_transform.position += _velocity.velocity * _deltaTime;
		});
	
}

void game::HorizontalSpring::draw(float _time, float _deltaTime) {

	meshRenderer.clear();
	game::Actions::Draw(meshRenderer, registry);
	
	meshRenderer.present(camera);
}

game::HorizontalSpring::HorizontalSpring() : game::GameState(),
																						 camera(graphics::Camera(90, .1f, 100)),
																						 mesh(graphics::Mesh("models/sphere.obj")),
																						 meshRenderer(),
																						 modelMatrix(glm::identity<glm::mat4>()),
																						 registry(){
	std::vector<Entity> entities;
	auto _mesh = &mesh;
	
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
		graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	texture = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);


	for (int i = 0; i < 2000; i++) {
		entities.push_back(registry.create());
		registry.addComponent<Mesh>(entities[i], _mesh);
		registry.addComponent<Texture>(entities[i], texture);
		registry.addComponent<Transform>(entities[i], glm::identity<glm::quat>(), glm::vec3(1, 1, 1), glm::vec3(i%10 -5, 0, -i));
		registry.addComponent<Velocity>(entities[i], glm::vec3(i%10, 0, 0));
	}
	camera.setView(glm::translate(glm::vec3(0, -5, -10)));
}

bool game::HorizontalSpring::getIsFinished() {
	return false;
}
