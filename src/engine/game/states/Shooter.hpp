#pragma once
#include "engine/game/GameState.hpp"
#include "engine/graphics/camera.hpp"
#include "engine/graphics/core/texture.hpp"
#include "engine/graphics/renderer/mesh.hpp"
#include "engine/graphics/renderer/meshrenderer.hpp"
#include "engine/game/registry/Registry.hpp"

namespace game {
	class Shooter : public game::GameState {
	public:
		Shooter();
		void onPause() override {};
		void onResume() override {};

		void update(float _time, float _deltaTime) override;
		void draw(float _time, float _deltaTime) override;
		bool getIsFinished() override;

	private:
		Registry registry;
		graphics::Camera camera;
		graphics::MeshRenderer meshRenderer;
		graphics::Mesh mesh_planet;
		graphics::Mesh mesh_box;
		graphics::Texture2D::Handle texture_planet;
		graphics::Texture2D::Handle texture_box;

		std::vector<Entity> boxes;
		std::vector<Entity> planets;
		int number_boxes;
		float counter_time;
		int counter_boxes;
	};
}
