#pragma once
#include "engine/game/registry/Components.hpp"
#include <engine/graphics/renderer/meshrenderer.hpp>
#include <glm/gtx/quaternion.hpp>
namespace game {
	class Actions {
	public:
		static void Draw(graphics::MeshRenderer& _meshRenderer, Registry& _registry) {
			_registry.execute<Mesh, Texture, Transform>([&](Mesh _mesh, Texture _texture, Transform& _transform) {
				_meshRenderer.draw(*_mesh.mesh, *_texture.texture, glm::translate(glm::mat4(1), _transform.position));

				}); 
		}

	};
	
	/*class Physics {
		Update(float _time, float _deltaTime) {

		}
	public:
	private:
	};*/
}