#include <string>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <iostream>
#include <engine/utils/meshloader.hpp>
#include <engine/graphics/core/mesh.h>
#include <engine/graphics/core/texture.hpp>
#include "engine/graphics/core/device.hpp"
#include "engine/graphics/core/shader.hpp"
#include "engine/graphics/camera.hpp"
#include <glm/gtx/transform.hpp>

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

  auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
  auto texture = graphics::Texture2D::load("../resources/textures/cratetex.png", sampler, true);
  auto mesh = graphics::Mesh("models/crate.obj");

  auto camera = graphics::Camera(90, .1, 100);

  typedef std::chrono::high_resolution_clock clock;
  typedef std::chrono::duration<float, std::milli> duration;
  static clock::time_point start = clock::now();

  // permanent settings
  glClearColor(.25, .2, .2, 1);
  texture->bind(0);
  program.use();
  auto mvpLocation = program.getUniformLoc("c_modelViewProjection");
  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    duration elapsed = clock::now() - start;
    auto modelMatrix = glm::translate(glm::vec3(0, 0, -3.0))
                       * glm::rotate(glm::pi<float>() * elapsed.count() * .0003f, glm::vec3(0, 1, 0));

    program.setUniform(mvpLocation, camera.getViewProjection() * modelMatrix);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mesh.draw();

    glfwSwapBuffers(window);
  }

  graphics::Device::close();
  return EXIT_SUCCESS;
}
