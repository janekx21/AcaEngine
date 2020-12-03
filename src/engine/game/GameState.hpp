#pragma once
#include "Game.hpp"
#include "GameState.hpp"
#include "memory"
#include <vector>
#include <engine/graphics/core/sampler.hpp>
#include <engine/graphics/core/texture.hpp>
#include "engine/graphics/renderer/mesh.hpp"
#include "engine/graphics/camera.hpp"
#include "engine/graphics/renderer/meshrenderer.hpp"

namespace game {
	class GameState
	{
	public:
		GameState() : is_finished(false) {
			auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
				graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
			auto texture = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);
			auto mesh = graphics::Mesh("models/sphere.obj");
			auto camera = graphics::Camera(90, .1f, 100);
			auto meshRenderer = graphics::MeshRenderer();
			float speed_z = 10;
			float trans_z = 0;
		}
		void Cleanup();
		virtual void newState() =0;

		virtual void onPause() = 0;
		virtual void onResume() = 0;

		virtual void update(float _time, float _deltaTime) = 0;
		virtual void draw(float _time, float _deltaTime) = 0;

		bool isFinished();


		bool is_finished = 0;
		graphics::MeshRenderer meshRenderer;
		graphics::Camera camera;
		graphics::Mesh mesh;
		graphics::Texture2D::Handle texture;
		float speed_z;
		float trans_z;
	};
}// namespace game
