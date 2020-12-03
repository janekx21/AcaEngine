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
		GameState();
		virtual void onPause() = 0;
		virtual void onResume() = 0;

		virtual void update(float _time, float _deltaTime) = 0;
		virtual void draw(float _time, float _deltaTime) = 0;

		virtual bool getIsFinished() = 0;
	};
}
