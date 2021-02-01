#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
#include <engine/graphics/core/texture.hpp>
#include <engine/graphics/renderer/mesh.hpp>


//Render Components:
struct Mesh {
	std::shared_ptr<graphics::Mesh> mesh;
	
};
struct Texture {
	std::shared_ptr<graphics::Texture2D> texture;
};

struct Transform {
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
};


