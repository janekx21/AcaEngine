#include "ExampleScene.hpp"
#include "engine/graphics/core/opengl.hpp"
#include "engine/input/inputmanager.hpp"
#include <engine/graphics/core/device.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

game::ExampleScene::ExampleScene() : camera(44, .1, 10),
																		 meshRenderer(),
																		 scene(graphics::Mesh("models/scene.obj")),
																		 quad(graphics::Mesh("models/quad.obj")),
																		 backBuffer() {
	auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
																	 graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
	white = graphics::Texture2D::load("../resources/textures/white.png", sampler, false);

	auto location = glm::vec3(-.5, .6, 1.33);
	auto rotation = glm::quat(glm::vec3(0, glm::radians(-133.0), 0));
	camera.setView(glm::translate(-location) * glm::toMat4(rotation));

	auto size = graphics::Device::getBufferSize();

	depthTexture = graphics::Texture2D::create(size.x, size.y, graphics::TexFormat::D32F, sampler);
	colorTexture = graphics::Texture2D::create(size.x, size.y, graphics::TexFormat::RGB8, sampler);
	normalTexture = graphics::Texture2D::create(size.x, size.y, graphics::TexFormat::RGB8, sampler);
	backBuffer.attachDepth(*depthTexture, 0);
	backBuffer.attach(0, *colorTexture, 0);
	backBuffer.attach(1, *normalTexture, 0);

	program = graphics::Program();
	program.attach(graphics::ShaderManager::get("shader/fullscreen.vert",graphics:: ShaderType::VERTEX));
	program.attach(graphics::ShaderManager::get("shader/ambientOcclusion.frag", graphics::ShaderType::FRAGMENT));
	program.link();
}

void game::ExampleScene::update(float _time, float _deltaTime) {
	if (input::InputManager::isKeyPressed(input::Key::ESCAPE)) {
		shouldFinish = true;
	}
}

void game::ExampleScene::draw(float _time, float _deltaTime) {
	meshRenderer.draw(scene, *white, glm::identity<glm::mat4>());

	if (!input::InputManager::isKeyPressed(input::Key::SPACE)) {
		backBuffer.bind();
		backBuffer.clear();
		meshRenderer.present(camera);
		meshRenderer.clear();
		backBuffer.unbind();

		auto slot = 0;
		auto slot2 = 1;
		colorTexture->bind(slot);
		depthTexture->bind(slot2);
		normalTexture->bind(2);
		program.use();
		auto loc = program.getUniformLoc("color_texture");
		auto loc2 = program.getUniformLoc("depth_texture");
		program.setUniform(loc, slot);
		program.setUniform(loc2, slot2);
		program.setUniform(program.getUniformLoc("normal_texture"), 2);
		quad.draw();
	} else{
		meshRenderer.present(camera);
		meshRenderer.clear();
	}


	/*
	meshRenderer.draw(scene, *colorTexture, glm::identity<glm::mat4>());
	meshRenderer.present(camera);
	meshRenderer.clear();
	 */
}
