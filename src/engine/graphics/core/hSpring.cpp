#include "GameState.hpp"
#include "hSpring.h"
#include <glm\fwd.hpp>
#include <glm\gtx\transform.hpp>




	void hSpring::onPause() {
	}

	void hSpring::onResume() {
	}

	void hSpring::newState() {
		auto modelMatrix = glm::translate(glm::vec3(0.f, 0.f, 0.f));
	}

	void hSpring::update(float _time, float _deltaTime) {
		modelMatrix = glm::translate(glm::vec3(0.f, 0.f, trans_z));
		trans_z += speed_z * _deltaTime;
		speed_z += -trans_z * 0.005f;
	}

	void hSpring::draw(float _time, float _deltaTime) {
		meshRenderer.draw(mesh, *texture, hSpring::modelMatrix);
	}

