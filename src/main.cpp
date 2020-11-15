#include <string>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <iostream>
#include <engine/utils/meshloader.hpp>
#include "engine/graphics/core/device.hpp"
#include "engine/graphics/core/shader.hpp"
#include "engine/graphics/core/geometrybuffer.hpp"

int main(int argc, char *argv[]) {
  graphics::Device::initialize(1366, 768, false);
  GLFWwindow *window = graphics::Device::getWindow();

  graphics::VertexAttribute attributes[] = {
          {graphics::PrimitiveFormat::FLOAT, 3, false, false},
          // {graphics::PrimitiveFormat::FLOAT, 3, false, false}
  };
  auto vertexShader = graphics::Shader::load("../resources/shader/simple_pass.vert", graphics::ShaderType::VERTEX,
                                             nullptr);
  auto fragmentShader = graphics::Shader::load("../resources/shader/simple_pass.frag", graphics::ShaderType::FRAGMENT,
                                               nullptr);
  auto program = graphics::Program();
  program.attach(vertexShader);
  program.attach(fragmentShader);
  program.link();
  program.use();

  auto geo = graphics::GeometryBuffer(graphics::GLPrimitiveType::TRIANGLES, attributes, 1, 1);

  auto meshData = utils::MeshLoader::get("models/crate.obj");
  std::vector<float> positionData(meshData->positions.size() * 3);
  int j = 0;
  for (auto &position : meshData->positions) {
    positionData[j] = position.x;
    positionData[j + 1] = position.y;
    positionData[j + 2] = position.z;
    j += 3;
  }

  geo.setData(positionData.data(), positionData.size() * sizeof(float));

  std::vector<int> indexData(meshData->faces.size() * 3);
  int i = 0;
  for (auto &face : meshData->faces) {
    indexData[i] = face.indices[0].positionIdx;
    indexData[i + 1] = face.indices[1].positionIdx;
    indexData[i + 2] = face.indices[2].positionIdx;
    i += 3;
  }
  geo.setIndexData(indexData.data(), sizeof(int) * indexData.size());
  glClearColor(.25, .2, .2, 1);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    geo.draw();

    glfwSwapBuffers(window);
  }

  graphics::Device::close();
  return EXIT_SUCCESS;
}
