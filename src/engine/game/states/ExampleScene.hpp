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

	private:
		graphics::Camera camera;
		graphics::MeshRenderer meshRenderer;
		graphics::Mesh scene;
		graphics::Mesh quad;
		graphics::Program program;
		graphics::Texture2D::Handle white;
		bool shouldFinish = false;
		graphics::FrameBuffer backBuffer;
		graphics::Texture2D *depthTexture;
		graphics::Texture2D *colorTexture;
		graphics::Texture2D *normalTexture;
	};
}// namespace game
