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
#include <engine/input/inputmanager.hpp>

class Flyer {
public:
  void update(float dt) {
    auto move = glm::vec3(0, 0, 0);
    if (input::InputManager::isKeyPressed(input::Key::W)) {
      move += glm::vec3(0, 0, -1);
    }
    if (input::InputManager::isKeyPressed(input::Key::S)) {
      move += glm::vec3(0, 0, 1);
    }
    if (input::InputManager::isKeyPressed(input::Key::D)) {
      move += glm::vec3(1, 0, 0);
    }
    if (input::InputManager::isKeyPressed(input::Key::A)) {
      move += glm::vec3(-1, 0, 0);
    }
    position += move * dt * speed;
  }

  glm::vec3 getPosition() { return position; }

private:
  glm::vec3 position = glm::vec3(0, 0, 0);
  float speed = 5.f;
};

class TimeManager {
  typedef std::chrono::high_resolution_clock clock;
  typedef std::chrono::duration<float> duration;

public:
  TimeManager() : startMoment(clock::now()), lastResetMoment(startMoment) {
  }

  float getTime() {
    duration elapsed = clock::now() - startMoment;
    return elapsed.count();
  }

  float getDeltaTime() {return deltaTime;}

  void resetDeltaTime() {
    auto now = clock::now();
    duration elapsed = now - lastResetMoment;
    deltaTime = elapsed.count();
    lastResetMoment = now;
  }

private:
  clock::time_point startMoment;
  clock::time_point lastResetMoment;
  float deltaTime = 0;
};


int main(int argc, char *argv[]) {
  graphics::Device::initialize(1366, 768, false);
  GLFWwindow *window = graphics::Device::getWindow();
  input::InputManager::initialize(window);

  auto vertexShader = graphics::Shader::load("../resources/shader/simple_pass.vert", graphics::ShaderType::VERTEX,
                                             nullptr);
  auto fragmentShader = graphics::Shader::load("../resources/shader/simple_pass.frag", graphics::ShaderType::FRAGMENT,
                                               nullptr);
  auto program = graphics::Program();
  program.attach(vertexShader);
  program.attach(fragmentShader);
  program.link();

  auto sampler = graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR,
                                   graphics::Sampler::Filter::LINEAR, graphics::Sampler::Border::CLAMP);
  auto texture = graphics::Texture2D::load("../resources/textures/planet1.png", sampler, false);
  auto mesh = graphics::Mesh("models/sphere.obj");

  auto camera = graphics::Camera(90, .1, 100);

  auto flyer = Flyer();
  auto time = TimeManager();

  // permanent settings
  glClearColor(.25, .2, .2, 1);
  texture->bind(0);
  program.use();
  auto mvpLocation = program.getUniformLoc("c_modelViewProjection");
  auto mLocation = program.getUniformLoc("c_modelProjection");
  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    float dt = time.getDeltaTime();

    flyer.update(dt);

    camera.setView(glm::translate(-flyer.getPosition()));

    auto modelMatrix = glm::translate(glm::vec3(0, 0, -3.0))
                       * glm::rotate(glm::pi<float>() * time.getTime() * .0003f, glm::vec3(0, 1, 0));

    program.setUniform(mvpLocation, camera.getViewProjection() * modelMatrix);
    program.setUniform(mLocation, modelMatrix);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mesh.draw();

    glfwSwapBuffers(window);
    time.resetDeltaTime();
  }

  graphics::Device::close();
  return EXIT_SUCCESS;
}
