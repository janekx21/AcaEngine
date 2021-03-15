#pragma once
#include "engine/game/GameState.hpp"
#include "engine/graphics/camera.hpp"
#include "engine/graphics/core/texture.hpp"
#include "engine/graphics/renderer/mesh.hpp"
#include "engine/graphics/renderer/meshrenderer.hpp"
#include "engine/game/Flyer.hpp"
#include "engine/game/Planet.hpp"

namespace game {
	class MovingPlanets : public game::GameState {
	public:
		MovingPlanets();
		void onPause() override {};
		void onResume() override {};

		void update(float _time, float _deltaTime) override;
		void draw(float _time, float _deltaTime) override;
		bool getIsFinished() override;

	private:
		graphics::Camera camera;
		graphics::MeshRenderer meshRenderer;
		graphics::Mesh mesh;

		std::vector<graphics::Texture2D::Handle> textures;
		std::vector<Planet> planets;

		float velocity;
		float position;

		bool end;
	};
}
