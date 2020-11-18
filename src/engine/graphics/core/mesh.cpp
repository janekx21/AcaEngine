#include <string>
#include "mesh.h"

#include "engine/utils/meshloader.hpp"

namespace graphics {
  const int VERTEX_PER_POLYGON = 3;
  const int POSITION_DIMENSIONS = 3;
  const int TEXTURE_COORD_DIMENSIONS = 2;

  const std::vector<VertexAttribute> Mesh::attributes = {
          {graphics::PrimitiveFormat::FLOAT, POSITION_DIMENSIONS,      false, false},
          {graphics::PrimitiveFormat::FLOAT, POSITION_DIMENSIONS,      false, false},
          {graphics::PrimitiveFormat::FLOAT, TEXTURE_COORD_DIMENSIONS, false, false},
  };

  Mesh::Mesh(const std::string &path) : geometryBuffer(
          GeometryBuffer(graphics::GLPrimitiveType::TRIANGLES, attributes.data(), attributes.size(), false)) {
    auto meshData = utils::MeshLoader::get(path.c_str());
    auto floatCount = 0;
    for(auto& attribute : Mesh::attributes) {
      floatCount += attribute.numComponents;
    }
    std::vector<float> bufferData(0);
    // meshData->faces.size() * floatCount * VERTEX_PER_POLYGON

    for(auto& face : meshData->faces) {
      for(auto& vertex : face.indices) {
        auto pos = meshData->positions[vertex.positionIdx];
        bufferData.push_back(pos.x);
        bufferData.push_back(pos.y);
        bufferData.push_back(pos.z);

        auto normal = glm::vec3(0, 0, 0);
        if (vertex.normalIdx.has_value()) {
          normal = meshData->normals[vertex.normalIdx.value()];
        }
        bufferData.push_back(normal.x);
        bufferData.push_back(normal.y);
        bufferData.push_back(normal.z);

        auto coord = glm::vec2(0, 0);
        if (vertex.normalIdx.has_value()) {
          coord = meshData->textureCoordinates[vertex.textureCoordinateIdx.value()];
        }
        bufferData.push_back(coord.x);
        bufferData.push_back(coord.y);
      }
    }

    geometryBuffer.setData(bufferData.data(), bufferData.size() * sizeof(float));
  }

  void Mesh::draw() const {
    geometryBuffer.draw();
  }
};
