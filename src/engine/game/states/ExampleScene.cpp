#include "ExampleScene.hpp"
#include "engine/graphics/core/device.hpp"
#include "engine/input/inputmanager.hpp"
#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <random>

float lerp(float a, float b, float f) {
	return a + f * (b - a);
}

game::ExampleScene::ExampleScene() : camera(44, .1, 10),
																		 meshRenderer(),
																		 meshRendererVanilla(),
																		 scene(graphics::Mesh("models/scene.obj")),
																		 quad(graphics::Mesh("models/quad.obj")) {
	white = graphics::Texture2D::load("../resources/textures/white.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP), false);

	auto location = glm::vec3(-.5, .6, 1.33);
	auto rotation = glm::quat(glm::vec3(0, glm::radians(-133.0), 0));
	camera.setView(glm::translate(-location) * glm::toMat4(rotation));

	auto size = graphics::Device::getBufferSize();

	ambientOcclusionProgram = graphics::Program();
	ambientOcclusionProgram.attach(graphics::ShaderManager::get("shader/fullscreen.vert", graphics::ShaderType::VERTEX));
	ambientOcclusionProgram.attach(graphics::ShaderManager::get("shader/ambientOcclusion.frag", graphics::ShaderType::FRAGMENT));
	ambientOcclusionProgram.link();

	meshRenderer.setLightingShader(ambientOcclusionProgram);

	auto random01 = std::uniform_real_distribution<float>(0, 1);
	auto generator = std::default_random_engine();

	// generate kernel
	sampleList = std::vector<glm::vec3>();
	const auto SAMPLE_SIZE = 64;
	for (int i = 0; i < SAMPLE_SIZE; ++i) {
		glm::vec3 sample(
						random01(generator) * 2.0 - 1.0,
						random01(generator) * 2.0 - 1.0,
						random01(generator));

		sample = glm::normalize(sample);
		sample *= random01(generator);
		auto scale = static_cast<float>(i) / static_cast<float>(SAMPLE_SIZE);
		scale = lerp(.1f, 1.f, scale * scale);
		sample *= scale;
		sampleList.push_back(sample);
	}

	// generate noise
	auto noiseList = std::vector<glm::vec4>();
	const auto NOISE_SIZE = 8;
	for (int i = 0; i < NOISE_SIZE * NOISE_SIZE; ++i) {
		glm::vec4 noise(
						random01(generator) * 2.0 - 1.0,
						random01(generator) * 2.0 - 1.0,
						0.0, 0.0);
		noiseList.push_back(noise);
	}

	auto repeatSampler = graphics::Sampler(graphics::Sampler::Filter::POINT, graphics::Sampler::Filter::POINT,
																				 graphics::Sampler::Filter::POINT, graphics::Sampler::Border::REPEAT);
	noiseTexture = graphics::Texture2D::create(NOISE_SIZE, NOISE_SIZE, graphics::TexFormat::RGBA16F, repeatSampler);
	noiseTexture->fillMipMapFloat(0, (float *) noiseList.data());

	ambientOcclusionProgram.setUniform(ambientOcclusionProgram.getUniformLoc("samples"), sampleList);
	{
		const auto slot = 4;
		noiseTexture->bind(slot);
		ambientOcclusionProgram.setUniform(ambientOcclusionProgram.getUniformLoc("noise_texture"), slot);
	}

	pos = glm::vec3(0, 0, 0);
	rot = 0;
	camera.backgroundColor = glm::vec4(0, 0, .01, 1);
}

void game::ExampleScene::update(float _time, float _deltaTime) {
	if (input::InputManager::isKeyPressed(input::Key::ESCAPE)) {
		shouldFinish = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void game::ExampleScene::draw(float _time, float _deltaTime) {
	if (input::InputManager::isKeyPressed(input::Key::A)) {
		pos.x -= _deltaTime;
	}
	if (input::InputManager::isKeyPressed(input::Key::D)) {
		pos.x += _deltaTime;
	}
	if (input::InputManager::isKeyPressed(input::Key::W)) {
		pos.z -= _deltaTime;
	}
	if (input::InputManager::isKeyPressed(input::Key::S)) {
		pos.z += _deltaTime;
	}
	if (input::InputManager::isKeyPressed(input::Key::Q)) {
		rot -= _deltaTime;
	}
	if (input::InputManager::isKeyPressed(input::Key::E)) {
		rot += _deltaTime;
	}


	auto location = glm::vec3(-.5, .6, 1.33) + pos;
	auto rotation = glm::quat(glm::vec3(0, glm::radians(-133.0) + rot * .5, 0));
	camera.setView(glm::translate(-location) * glm::toMat4(rotation));

	if (!input::InputManager::isKeyPressed(input::Key::SPACE)) {
		meshRenderer.draw(scene, *white, glm::identity<glm::mat4>());

		ambientOcclusionProgram.setUniform(ambientOcclusionProgram.getUniformLoc("view_matrix"), camera.getView());
		ambientOcclusionProgram.setUniform(ambientOcclusionProgram.getUniformLoc("projection_matrix"), camera.getProjection());
		meshRenderer.present(camera);
		meshRenderer.clear();
	} else {
		meshRendererVanilla.draw(scene, *white, glm::identity<glm::mat4>());
		meshRendererVanilla.present(camera);
		meshRendererVanilla.clear();
	}
}
