#include <string>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <iostream>
#include "engine/graphics/core/device.hpp"
#include "engine/graphics/core/shader.hpp"
#include "engine/graphics/core/geometrybuffer.hpp"

int main(int argc, char *argv[]) {
  graphics::Device::initialize(1366, 768, false);
  GLFWwindow *window = graphics::Device::getWindow();

  graphics::VertexAttribute attributes[] = {
          {graphics::PrimitiveFormat::FLOAT, 3, false, false},
          {graphics::PrimitiveFormat::FLOAT, 3, false, false}
          };
  auto geo = graphics::GeometryBuffer(graphics::GLPrimitiveType::TRIANGLES, attributes, 2, 0, 4 * 3 * 2 * 3);
  auto vertexShader = graphics::Shader::load("../resources/shader/simple_pass.vert", graphics::ShaderType::VERTEX,
                                             nullptr);
  auto fragmentShader = graphics::Shader::load("../resources/shader/simple_pass.frag", graphics::ShaderType::FRAGMENT,
                                               nullptr);
  auto program = graphics::Program();
  program.attach(vertexShader);
  program.attach(fragmentShader);
  program.link();
  program.use();

  float data[] = {-1, -1, 0, 1, 0, 0,
                  1, -1, 0, 0, 1, 0,
                  0, 1, 0, 0, 0, 1};
  geo.bind();
  geo.setData(data, 4 * 3 * 2 * 3);
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
