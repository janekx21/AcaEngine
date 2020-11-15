#pragma once

#include "../core/texture.hpp"
#include "../core/mesh.h"
#include <glm/glm.hpp>
#include "../camera.hpp"

namespace graphics {
  class MeshRenderer {
  public:
    MeshRenderer() {}

    void draw(const Mesh &_mesh, const Texture2D &_texture, const glm::mat4 &_transform) {}

    void present(const Camera &_camera) {}

    void clear() {}

  private:
  };
}