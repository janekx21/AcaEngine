#include "Flyer.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include "engine/math/directions.h"
#include <map>
#include "engine/input/inputmanager.hpp"

game::Flyer::Flyer() {
	position = glm::vec3{ 15.f,15.f,-35.f};
	front = glm::vec3{ 0.f,0.f,1.f };
	target = glm::vec3{ 0.f,0.f,0.f };
	direction = glm::normalize(position - target);
	up = glm::cross(direction, glm::normalize(glm::cross(math::up, direction)));
	view = glm::lookAt(position, position + front, up);
	velocity = { 0.f,0.f,1.f };
	yaw = 90.0f;
	pitch = 0.0f;
	angle = 1.0f;
}

game::Flyer::~Flyer()
{
}

void game::Flyer::update(float dt){
	applyRotation(dt);
	applyFriction(dt);
	auto move = getMove();
	move = glm::mat3(inverse(view)) * move;
	applyAcceleration(move, dt);
	view = glm::lookAt(position, position + front, up);
}

glm::vec3 game::Flyer::getPosition()
{
	return position;
}

void game::Flyer::setView(glm::vec3 _position, glm::vec3 _direction)
{
	position = _position;
	direction = glm::normalize(_direction);
	view = view = glm::lookAt(position, position + front, glm::cross(direction, glm::normalize(glm::cross(math::up, direction))));

	
}

glm::mat4 game::Flyer::getView()
{
	return view;
}

glm::vec3 game::Flyer::getMove()
{
	auto move = math::zero;
	for (auto& [key, direction] : keyToDirection) {

		if (input::InputManager::isKeyPressed(key)) {
			move += direction;
		}
	}
	if (glm::length(move) > 1) {
		move = glm::normalize(move);
	}
	return move;
}

void game::Flyer::applyFriction(float dt)
{
	if (glm::length(velocity) > 0) {
		velocity += -glm::normalize(velocity) * glm::length(velocity) * friction * dt;
	}
}

void game::Flyer::applyAcceleration(glm::vec3 move, float dt)
{
	velocity += move * acceleration * dt;
	position += velocity * dt;
}

void game::Flyer::applyRotation(float dt)
{
	auto deltaMouse = input::InputManager::getDeltaCursorPos();
	yaw += deltaMouse.x * sensitivity * dt;
	pitch -= deltaMouse.y * sensitivity * dt;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);
}
