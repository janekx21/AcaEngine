#pragma once
#include "engine/game/GameState.hpp"
#include "engine/graphics/camera.hpp"
#include "engine/graphics/core/framebuffer.hpp"
#include "engine/graphics/renderer/meshrenderer.hpp"

namespace game {
	class ExampleScene : public game::GameState {
	public:
		ExampleScene();
		ExampleScene(const game::ExampleScene &) = delete;
		void onPause() override{};
		void onResume() override{};

		void update(float _time, float _deltaTime) override;
		void draw(float _time, float _deltaTime) override;
		bool getIsFinished() override { return shouldFinish; }

		bool getIsMenu() override { return false; }
		int goToState() override { return 0; }

	private:
		graphics::Camera camera;
		graphics::MeshRenderer meshRenderer;
		graphics::Mesh scene;
		graphics::Mesh quad;
		graphics::Program ambientOcclusionProgram;
		graphics::Texture2D::Handle white;
		bool shouldFinish = false;
		glm::vec3 pos;
		float rot;

		graphics::Texture2D *noiseTexture;
		std::vector<glm::vec3> sampleList;
	};
}// namespace game
