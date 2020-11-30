#pragma once

#include "../core/shader.hpp"
#include "../camera.hpp"
#include "glm/glm.hpp"
#include "mesh.hpp"
#include <vector>
#include "engine/graphics/core/texture.hpp"

namespace graphics {
	class MeshRenderer
	{
	public:
		MeshRenderer();

		void draw(const Mesh& _mesh, const Texture2D& _texture, const glm::mat4& _transform);
		void present(const Camera& _camera);
		void clear();
	private:
    struct MeshInstance {
		  const Mesh& mesh;
      const Texture2D& texture;
      const glm::mat4& transform;
    };

		Program program;
    std::vector<MeshInstance> meshQueue;
    int cameraPositionLocation;
    int modelMatrixLocation;
    int mvpMatrixLocation;
    int albedoTextureLocation;
  };
}