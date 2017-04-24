#ifndef TRIANGLE_OBJECT_HPP
#define TRIANGLE_OBJECT_HPP

#include "model.hpp"
#include "shader.hpp"
#include <glm/glm.hpp>
#include <memory>

class Object {
public:
    Object(Model *model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    ~Object();

    void Draw(std::shared_ptr<ShaderProgram> shader);

    Model *model;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

#endif //TRIANGLE_OBJECT_HPP
