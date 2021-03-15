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
																		 scene(graphics::Mesh("models/scene.obj")),
																		 quad(graphics::Mesh("models/quad.obj")),
																		 backBuffer() {

	linear = new graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
																 graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);

	white = graphics::Texture2D::load("../resources/textures/white.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
																																												 graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP), false);

	auto location = glm::vec3(-.5, .6, 1.33);
	auto rotation = glm::quat(glm::vec3(0, glm::radians(-133.0), 0));
	camera.setView(glm::translate(-location) * glm::toMat4(rotation));

	auto size = graphics::Device::getBufferSize();

	depthTexture = graphics::Texture2D::create(size.x, size.y, graphics::TexFormat::D32F, graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP));
	colorTexture = graphics::Texture2D::create(size.x, size.y, graphics::TexFormat::RGB8, graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP));
	normalTexture = graphics::Texture2D::create(size.x, size.y, graphics::TexFormat::RGBA16F, graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP));
	positionTexture = graphics::Texture2D::create(size.x, size.y, graphics::TexFormat::RGBA16F, *linear);
	backBuffer.attachDepth(*depthTexture, 0);
	backBuffer.attach(0, *colorTexture, 0);
	backBuffer.attach(1, *normalTexture, 0);
	backBuffer.attach(2, *positionTexture, 0);

	program = graphics::Program();
	program.attach(graphics::ShaderManager::get("shader/fullscreen.vert", graphics::ShaderType::VERTEX));
	program.attach(graphics::ShaderManager::get("shader/ambientOcclusion.frag", graphics::ShaderType::FRAGMENT));
	program.link();


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

	pos = glm::vec3(0, 0, 0);
	rot = 0;
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


	meshRenderer.draw(scene, *white, glm::identity<glm::mat4>());

	if (!input::InputManager::isKeyPressed(input::Key::SPACE)) {
		backBuffer.bind();
		backBuffer.clear();
		meshRenderer.present(camera);
		meshRenderer.clear();
		backBuffer.unbind();

		program.use();
		{
			const auto slot = 0;
			positionTexture->bind(slot);
			auto location = program.getUniformLoc("position_texture");
			program.setUniform(location, slot);
		}
		{
			const auto slot = 1;
			normalTexture->bind(slot);
			auto location = program.getUniformLoc("normal_texture");
			program.setUniform(location, slot);
		}
		{
			const auto slot = 2;
			colorTexture->bind(slot);
			auto location = program.getUniformLoc("color_texture");
			program.setUniform(location, slot);
		}
		{
			const auto slot = 4;
			noiseTexture->bind(slot);
			auto location = program.getUniformLoc("noise_texture");
			program.setUniform(location, slot);
		}

		auto location2 = program.getUniformLoc("samples");
		program.setUniform(location2, sampleList);
		program.setUniform(program.getUniformLoc("projection_matrix"), camera.getProjection());

		quad.draw();
	} else {
		meshRenderer.present(camera);
		meshRenderer.clear();
	}
}
