#pragma once
#include "GameState.hpp"

class hSpring : public GameState {

	void onPause();
	void onResume();

	void update(float _time, float _deltaTime);
	void draw(float _time, float _deltaTime);

	void newState();
	glm::mat4 modelMatrix;
};

