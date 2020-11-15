#include <string>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <iostream>
#include <engine/utils/meshloader.hpp>
#include <engine/graphics/core/mesh.h>
#include "engine/graphics/core/device.hpp"
#include "engine/graphics/core/shader.hpp"

int main(int argc, char *argv[]) {
  graphics::Device::initialize(1366, 768, false);
  GLFWwindow *window = graphics::Device::getWindow();

  auto vertexShader = graphics::Shader::load("../resources/shader/simple_pass.vert", graphics::ShaderType::VERTEX,
                                             nullptr);
  auto fragmentShader = graphics::Shader::load("../resources/shader/simple_pass.frag", graphics::ShaderType::FRAGMENT,
                                               nullptr);
  auto program = graphics::Program();
  program.attach(vertexShader);
  program.attach(fragmentShader);
  program.link();
  program.use();

  auto mesh = graphics::Mesh("models/crate.obj");

  glClearColor(.25, .2, .2, 1);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mesh.draw();

    glfwSwapBuffers(window);
  }

  graphics::Device::close();
  return EXIT_SUCCESS;
}
