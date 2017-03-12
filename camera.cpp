#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 target): position(0), target(0) {
    Move(position);
    LookAt(target);
}

void Camera::Move(glm::vec3 shift) {
    position += shift;
}

void Camera::LookAt(glm::vec3 target) {
    this->target = target;
}

glm::mat4 Camera::View() {
    return glm::lookAt(position, target, glm::vec3(0, 1, 0));
}

void Camera::MoveTo(glm::vec3 position) {
    this->position = position;
}
