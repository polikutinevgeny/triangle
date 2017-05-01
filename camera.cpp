#include "camera.hpp"
#include <cmath>


Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
        : front(glm::vec3(0.0f, 0.0f, -1.0f)),
          movement_speed(SPEED),
          mouse_sensitivity(SENSITIVTY),
          zoom(ZOOM) {
    this->position = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
    GLfloat velocity = this->movement_speed * deltaTime;
    if (direction == FORWARD)
        this->position +=
                glm::normalize(glm::vec3(this->front.x, 0, this->front.z)) *
                velocity;
    if (direction == BACKWARD)
        this->position -=
                glm::normalize(glm::vec3(this->front.x, 0, this->front.z)) *
                velocity;
    if (direction == LEFT)
        this->position -= this->right * velocity;
    if (direction == RIGHT)
        this->position += this->right * velocity;
    if (direction == UP)
        this->position += world_up * velocity;
    if (direction == DOWN)
        this->position -= world_up * velocity;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset) {
    xoffset *= this->mouse_sensitivity;
    yoffset *= this->mouse_sensitivity;
    this->yaw = std::fmod((yaw + xoffset), (GLfloat) 360.0f);
    this->pitch += yoffset;
    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;
    this->UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset) {
    if (this->zoom >= 1.0f && this->zoom <= 45.0f)
        this->zoom -= yoffset;
    if (this->zoom <= 1.0f)
        this->zoom = 1.0f;
    if (this->zoom >= 45.0f)
        this->zoom = 45.0f;
}

void Camera::UpdateCameraVectors() {
    glm::vec3 front;
    front.x = std::cos(glm::radians(this->yaw)) *
              std::cos(glm::radians(this->pitch));
    front.y = std::sin(glm::radians(this->pitch));
    front.z = std::sin(glm::radians(this->yaw)) *
              std::cos(glm::radians(this->pitch));
    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(this->front, this->world_up));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
