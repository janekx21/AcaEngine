#include "meshrenderer.hpp"
#include "engine/graphics/core/shader.hpp"

namespace graphics {
  MeshRenderer::MeshRenderer() : meshQueue() {
    program = Program();
    program.attach(ShaderManager::get("shader/model.vert", ShaderType::VERTEX));
    program.attach(ShaderManager::get("shader/model.frag", ShaderType::FRAGMENT));
    program.link();

    cameraPositionLocation = program.getUniformLoc("camera_position");
    modelMatrixLocation = program.getUniformLoc("model_matrix");
    mvpMatrixLocation = program.getUniformLoc("mvp_matrix");
    albedoTextureLocation = program.getUniformLoc("albedo_texture");
  }

  void MeshRenderer::draw(const Mesh& _mesh, const Texture2D& _texture, const glm::mat4& _transform) {
    struct MeshInstance mesh_instance = { _mesh, _texture, _transform};
    MeshRenderer::meshQueue.push_back(mesh_instance);
  }

  void MeshRenderer::present(const Camera& _camera) {
    if (meshQueue.empty()) return;
    program.use();
    int textureBindLocation = 0;
    // TODO
    // program.setUniform(cameraPositionLocation, _camera.)

    for(auto& instance : meshQueue) {
      instance.texture.bind(textureBindLocation);
      program.setUniform(albedoTextureLocation, textureBindLocation);
      program.setUniform(modelMatrixLocation, instance.transform);
      program.setUniform(mvpMatrixLocation, _camera.getViewProjection() * instance.transform);
      instance.mesh.draw();
    }
  }

  void MeshRenderer::clear() {
    meshQueue.clear();
  }
}