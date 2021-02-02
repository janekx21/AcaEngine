#include "HorizontalSpring.hpp"
#include <glm/gtx/transform.hpp>
#include "engine/game/registry/Components.hpp"
#include "engine/game/actions/Actions.hpp"

void game::HorizontalSpring::update(float _time, float _deltaTime) {
	velocity -= position * _deltaTime * 8;
	position += velocity * _deltaTime;
	modelMatrix = glm::translate(glm::vec3(position, 0, 0));
}

void game::HorizontalSpring::draw(float _time, float _deltaTime) {

	meshRenderer.clear();
	//registry.execute<Mesh, Texture, Transform>(Draw(meshRenderer));
	registry.execute<Mesh, Texture, Transform>([&](Mesh _mesh, Texture _texture, Transform _transform) {
		meshRenderer.draw(*_mesh.mesh, *_texture.texture, glm::translate(glm::mat4(1), _transform.position));
		
		}); // ToDo: execute Action not Lambda
	
	meshRenderer.present(camera);
}

game::HorizontalSpring::HorizontalSpring() : game::GameState(),
																						 camera(graphics::Camera(90, .1f, 2000)),
																						 mesh(graphics::Mesh("models/sphere.obj")),
																						 meshRenderer(),
																						 modelMatrix(glm::identity<glm::mat4>()),
																						 registry(){
	Entity entity = registry.create();
	auto _mesh = &mesh;
	registry.addComponent<Mesh>(entity, _mesh);


	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
																	 graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	texture  = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);

	registry.addComponent<Texture>(entity, texture);

	 

	registry.addComponent<Transform>(entity, glm::vec3(0, 0, 0), glm::identity<glm::quat>(), glm::vec3(1, 1, 1));
	velocity = 10;
	position = 0;
	camera.setView(glm::translate(glm::vec3(0,0,-5)));
}

bool game::HorizontalSpring::getIsFinished() {
	return false;
}
