#pragma once
#include "engine/game/registry/Components.hpp"
#include <engine/graphics/renderer/meshrenderer.hpp>
#include <glm/gtx/quaternion.hpp>
namespace game {
	class Draw {
	public:
		Draw(std::shared_ptr<graphics::MeshRenderer> _meshRenderer)  {
			meshRenderer = _meshRenderer;
		}
		void operator()(Mesh _mesh, Texture _texture, Transform _transform) {
			meshRenderer.get()->draw(*_mesh.mesh, *_texture.texture, glm::scale(glm::translate(glm::mat4(1), _transform.position) * glm::toMat4(_transform.rotation), _transform.scale));
		}

	private:
		std::shared_ptr<graphics::MeshRenderer> meshRenderer;
	};
}