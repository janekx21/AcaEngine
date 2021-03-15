#pragma once
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
namespace game {
	class Planet {

	public:
		Planet();
		Planet(glm::vec3 centerPosition, glm::vec3 tiltRotation, glm::vec3 tiltRevolution, float rotationSpeed, float revolutionSpeed, float size, float radius);
		void update(float dt);
		glm::mat4 getModel();

	private:
		glm::mat4 model;
		glm::vec3 centerPosition;
		glm::vec3 tiltRotation;
		glm::vec3 tiltRevolution;
		float radius;
		float rotationSpeed;
		float revolutionSpeed;
		float size;
		float time = 0;

		glm::vec3 orthoVec(glm::vec3 v);
	};
}// namespace game
