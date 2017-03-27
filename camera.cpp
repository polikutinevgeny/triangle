#include "camera.hpp"
#include <cmath>


Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                                                               MovementSpeed(SPEED),
                                                                               MouseSensitivity(SENSITIVTY),
                                                                               Zoom(ZOOM) {
    this->Position = position;
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
    GLfloat velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD)
        this->Position += this->Front * velocity;
    if (direction == BACKWARD)
        this->Position -= this->Front * velocity;
    if (direction == LEFT)
        this->Position -= this->Right * velocity;
    if (direction == RIGHT)
        this->Position += this->Right * velocity;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset) {
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;
    this->Yaw = std::fmod((Yaw + xoffset), (GLfloat) 360.0f);
    this->Pitch += yoffset;
    if (this->Pitch > 89.0f)
        this->Pitch = 89.0f;
    if (this->Pitch < -89.0f)
        this->Pitch = -89.0f;
    this->UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset) {
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
        this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
}

void Camera::UpdateCameraVectors() {
    glm::vec3 front;
    front.x = std::cos(glm::radians(this->Yaw)) * std::cos(glm::radians(this->Pitch));
    front.y = std::sin(glm::radians(this->Pitch));
    front.z = std::sin(glm::radians(this->Yaw)) * std::cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
    this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}
