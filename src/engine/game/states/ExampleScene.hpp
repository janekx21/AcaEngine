#pragma once
#include "engine/game/GameState.hpp"
#include "engine/graphics/camera.hpp"
#include "engine/graphics/renderer/meshrenderer.hpp"

namespace game {
	class ExampleScene : public game::GameState {
	public:
		ExampleScene();
		void onPause() override{};
		void onResume() override{};

		void update(float _time, float _deltaTime) override;
		void draw(float _time, float _deltaTime) override;
		bool getIsFinished() override { return shouldFinish; }

	private:
		graphics::Camera camera;
		graphics::MeshRenderer meshRenderer;
		graphics::Mesh scene;
		graphics::Texture2D::Handle white;
		bool shouldFinish = false;
	};
}// namespace game
