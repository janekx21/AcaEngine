#pragma once

#include <vector>
#include "geometrybuffer.hpp"

namespace graphics {
  class Mesh {
  public:
    Mesh(const std::string &path);
    void draw() const;

  private:
    static const std::vector<VertexAttribute> attributes;
    GeometryBuffer geometryBuffer;
  };
}
