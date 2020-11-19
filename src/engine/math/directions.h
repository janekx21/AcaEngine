#pragma once

#include <glm/vec3.hpp>

namespace math {
  const glm::vec3 up(0, 1, 0);
  const glm::vec3 down(0, -1, 0);
  const glm::vec3 left(-1, 0, 0);
  const glm::vec3 right(1, 0, 0);
  const glm::vec3 forward(0, 0, -1);
  const glm::vec3 backwards(0, 0, 1);
  const glm::vec3 one(1, 1, 1);
  const glm::vec3 zero(0, 0, 0);
}
