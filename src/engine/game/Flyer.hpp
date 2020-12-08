#pragma once
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include "engine/math/directions.h"
#include <map>
#include "engine/input/inputmanager.hpp"
namespace game {
	class Flyer {
	public:
		Flyer();
		~Flyer();
		void update(float dt);
		glm::vec3 getPosition();
		void setView(glm::vec3 _position, glm::vec3 _direction);
		glm::mat4 getView();
	private:

		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 target;
		glm::vec3 direction;
		glm::vec3 up;
		glm::mat4 view;
		glm::vec3 velocity;

		float yaw;
		float pitch;
		float angle;


		const float friction = 10.0f;
		const float acceleration = 100.f;
		const float sensitivity = 10.0f;
		const std::map<input::Key, glm::vec3> keyToDirection = {
						{input::Key::W, math::forward},
						{input::Key::S, math::backwards},
						{input::Key::A, math::left},
						{input::Key::D, math::right},
		};
		


		glm::vec3 getMove();

		void applyFriction(float dt);

		void applyAcceleration(glm::vec3 move, float dt);

		void applyRotation(float dt);
	};
}