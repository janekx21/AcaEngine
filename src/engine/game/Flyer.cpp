#include "Flyer.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include "engine/math/directions.h"
#include <map>
#include "engine/input/inputmanager.hpp"
#include <iostream>
game::Flyer::Flyer() {
	position = glm::vec3{ 0.f,0.f,5.f };
	front = glm::vec3{ 0.f,0.f,1.f };
	target = glm::vec3{ 0.f,0.f,0.f };
	direction = glm::normalize(position - target);
	up = glm::cross(direction, glm::normalize(glm::cross(math::up, direction)));
	view = glm::lookAt(position, position + front, up);
	velocity = { 0.f,0.f,1.f };
	yaw = -90.0f;
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
	//std::cout << "move: " << move.x << ", " << move.y << ", " << move.z << "\n ";
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
	//auto deltaMouse = glm::vec2(0,0);
	yaw += deltaMouse.x * sensitivity * dt;
	pitch -= deltaMouse.y * sensitivity * dt;
	std::cout << "deltaMouse.x" << deltaMouse.x << "deltaMouse.y" << deltaMouse.y << "\n";
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	//for scene
	/*yaw = 90;
	pitch = 0;*/
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//std::cout << "direction: " <<  direction.x << ", " << direction.y << ", " << direction.z << "\n ";
	front = glm::normalize(direction);
}
