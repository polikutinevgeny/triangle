#ifndef TRIANGLE_CAMERA_HPP
#define TRIANGLE_CAMERA_HPP


#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};


const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.1f;
const GLfloat ZOOM = 45.0f;


class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    GLfloat yaw;
    GLfloat pitch;
    GLfloat movement_speed;
    GLfloat mouse_sensitivity;
    GLfloat zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat yaw = YAW, GLfloat pitch = PITCH);

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset);

    void ProcessMouseScroll(GLfloat yoffset);

private:
    void UpdateCameraVectors();
};


#endif //TRIANGLE_CAMERA_HPP
