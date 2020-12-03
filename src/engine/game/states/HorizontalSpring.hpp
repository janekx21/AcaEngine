#pragma once
#include "engine/game/GameState.hpp"

namespace game {
	class HorizontalSpring : public game::GameState {

		void onPause();
		void onResume();

		void update(float _time, float _deltaTime);
		void draw(float _time, float _deltaTime);

		void newState();
		glm::mat4 modelMatrix;
	};
}
