#include "Planet.hpp"
game::Planet::Planet()
{
	model = glm::mat4(1.0f);
	this->centerPosition = glm::vec3(0,0,0);
	this->tiltRotation = glm::vec3(0, 1, 0);
	this->tiltRevolution = glm::vec3(0, 1, 0);
	this->rotationSpeed = 0;
	this->revolutionSpeed = 0;
	this->size = 1;
	this->radius = 0;
	time = 0;
}
game::Planet::Planet(glm::vec3 centerPosition, glm::vec3 tiltRotation, glm::vec3 tiltRevolution, float rotationSpeed, float revolutionSpeed, float size, float radius)
{
	model = glm::mat4(1.0f);
	this->centerPosition = centerPosition;
	this->tiltRotation = normalize(tiltRotation);
	this->tiltRevolution = normalize(tiltRevolution);
	this->rotationSpeed = rotationSpeed;
	this->revolutionSpeed = revolutionSpeed;
	this->size = size;
	this->radius = radius;
	time = 0;
}

void game::Planet::update(float dt)
{
	time += dt;
	model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0) * size); // scale size
	model *= glm::translate(glm::mat3(glm::rotate(glm::pi<float>() * time * revolutionSpeed, normalize(tiltRevolution))) * (centerPosition + orthoVec(tiltRevolution) * radius)); // revolution
	model *= glm::rotate(glm::pi<float>() * time * rotationSpeed, tiltRotation) * glm::rotate(glm::pi<float>(), glm::vec3(0, 0, 1));   // rotation   

}

glm::mat4 game::Planet::getModel()
{
	return model;
}

glm::vec3 game::Planet::orthoVec(glm::vec3 v)
{
	if (v.x != 0.f) {
		return glm::vec3(v.y, -v.x, 0.f);
	}
	else {
		return glm::vec3(0, -v.z, 0.f);
	}
}
