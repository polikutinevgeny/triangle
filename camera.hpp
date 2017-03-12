#ifndef TRIANGLE_CAMERA_HPP
#define TRIANGLE_CAMERA_HPP


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 target);
    void LookAt(glm::vec3 target);
    void Move(glm::vec3 shift);
    void MoveTo(glm::vec3 position);
    glm::mat4 View();

private:
    glm::vec3 position;
    glm::vec3 target;
};


#endif //TRIANGLE_CAMERA_HPP
