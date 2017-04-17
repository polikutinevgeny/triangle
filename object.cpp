#include "object.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Object::Object(Model *model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : model(model),
                                                                                        position(position),
                                                                                        rotation(rotation),
                                                                                        scale(scale) {}

Object::~Object() {
    delete model;
}

void Object::Draw(ShaderProgram &shader) {
    glm::mat4 m;
    m = glm::translate(m, position);
    m = glm::scale(m, scale);
    m = glm::rotate(m, (glm::mediump_float) rotation.x, glm::vec3(1.f, 0.f, 0.f));
    m = glm::rotate(m, (glm::mediump_float) rotation.y, glm::vec3(0.f, 1.f, 0.f));
    m = glm::rotate(m, (glm::mediump_float) rotation.z, glm::vec3(0.f, 0.f, 1.f));
    glUniformMatrix4fv(shader.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(m));
    model->Draw(shader);
}
