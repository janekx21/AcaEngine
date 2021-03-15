#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
#include <engine/graphics/core/texture.hpp>
#include <engine/graphics/renderer/mesh.hpp>
#include <engine/math/geometrictypes.hpp>


//Render Components:
struct Mesh {
	graphics::Mesh* mesh;
	
};
struct Texture {
	graphics::Texture2D::Handle texture;
};

struct Transform {	
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 position;
};
struct Velocity {
	glm::vec3 velocity;
	
};
struct AngularVelocity {
	glm::quat angularVelocity; 
};
struct Visibility {
	bool visible;
};
struct ObjectType {
	int type; //0 = box = target| 1 = planet = projectile
};
struct AABB {
	math::AABB<3> untransformed_box;
	math::AABB<3> transformed_box;
	bool projectile;
};


