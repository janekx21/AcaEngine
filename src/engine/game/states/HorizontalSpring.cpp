#include "HorizontalSpring.hpp"
#include "engine/game/GameState.hpp"
#include <glm/gtx/transform.hpp>
#include <glm\fwd.hpp>


void game::HorizontalSpring::onPause() {
	}

	void game::HorizontalSpring::onResume() {
	}

	void game::HorizontalSpring::newState() {
		auto modelMatrix = glm::translate(glm::vec3(0.f, 0.f, 0.f));
	}

	void game::HorizontalSpring::update(float _time, float _deltaTime) {
		modelMatrix = glm::translate(glm::vec3(0.f, 0.f, trans_z));
		trans_z += speed_z * _deltaTime;
		speed_z += -trans_z * 0.005f;
	}

	void game::HorizontalSpring::draw(float _time, float _deltaTime) {
		meshRenderer.draw(mesh, *texture, HorizontalSpring::modelMatrix);
	}

