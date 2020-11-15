#include <string>
#include "mesh.h"

#include "engine/utils/meshloader.hpp"

namespace graphics {
  std::vector<float> generatePositionData(const utils::MeshData *meshData);

  std::vector<int> generateIndexData(const utils::MeshData *meshData);

  const std::vector<VertexAttribute> Mesh::attributes = {
          {graphics::PrimitiveFormat::FLOAT, 3, false, false},
  };
  const int VERTEX_PER_POLYGON = 3;

  Mesh::Mesh(const std::string &path) : geometryBuffer(
          GeometryBuffer(graphics::GLPrimitiveType::TRIANGLES, attributes.data(), attributes.size(), 1)) {
    auto meshData = utils::MeshLoader::get(path.c_str());

    auto positionData = generatePositionData(meshData);
    geometryBuffer.setData(positionData.data(), positionData.size() * sizeof(float));

    auto indexData = generateIndexData(meshData);
    geometryBuffer.setIndexData(indexData.data(), sizeof(int) * indexData.size());
  }

  std::vector<float> generatePositionData(const utils::MeshData *meshData) {
    std::vector<float> positionData(meshData->positions.size() * VERTEX_PER_POLYGON);
    int index = 0;
    for (auto &position : meshData->positions) {
      for (unsigned int i = 0; i < VERTEX_PER_POLYGON; i++) {
        positionData[index] = position[i];
        index++;
      }
    }
    return positionData;
  }

  std::vector<int> generateIndexData(const utils::MeshData *meshData) {
    std::vector<int> indexData(meshData->faces.size() * VERTEX_PER_POLYGON);
    int index = 0;
    for (auto &face : meshData->faces) {
      for (auto &idx : face.indices) {
        indexData[index] = idx.positionIdx;
        index++;
      }
    }
    return indexData;
  }

  void Mesh::draw() const {
    geometryBuffer.draw();
  }
};
