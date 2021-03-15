#pragma once
#include "engine/game/GameState.hpp"
#include "engine/graphics/camera.hpp"
#include "engine/graphics/core/texture.hpp"
#include "engine/graphics/renderer/mesh.hpp"
#include "engine/graphics/renderer/meshrenderer.hpp"
#include "engine/game/Flyer.hpp"
#include "engine/game/registry/Registry.hpp"

namespace game {
	class Start : public game::GameState {
	public:
		Start();
		void onPause() override {};
		void onResume() override {};

		void update(float _time, float _deltaTime) override;
		void draw(float _time, float _deltaTime) override;
		bool getIsFinished() override;
		bool getIsMenue() override;
		int goToState() override;

	private:
		graphics::Camera camera;
		glm::mat4 modelMatrix;
		graphics::MeshRenderer meshRenderer;
		graphics::Mesh mesh;
		graphics::Texture2D::Handle texture_white;
		graphics::Texture2D::Handle texture_blue;
		graphics::Texture2D::Handle texture_orange;
		Registry registry;

		int nextState;
		bool isFinished;
	};
}
