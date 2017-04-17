#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lightsources.hpp"
#include "shader.hpp"
#include "vao.hpp"
#include "vbo.hpp"

void DirLight::Load(ShaderProgram &shader, std::string name) {
    glUniform3f(shader.GetUniformLocation(name + ".direction"), direction.x, direction.y, direction.z);
    glUniform3f(shader.GetUniformLocation(name + ".ambient"), ambient.r, ambient.g, ambient.b);
    glUniform3f(shader.GetUniformLocation(name + ".diffuse"), diffuse.r, diffuse.g, diffuse.b);
    glUniform3f(shader.GetUniformLocation(name + ".specular"), specular.r, specular.g, specular.b);
}

DirLight::DirLight(const glm::vec3 &direction, const glm::vec3 ambient, const glm::vec3 diffuse,
                   const glm::vec3 specular)
        : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular) {}

void PointLight::Load(ShaderProgram &shader, std::string name) {
    glUniform3f(shader.GetUniformLocation(name + ".position"), position.x, position.y, position.z);
    glUniform3f(shader.GetUniformLocation(name + ".ambient"), ambient.r, ambient.g, ambient.b);
    glUniform3f(shader.GetUniformLocation(name + ".diffuse"), diffuse.r, diffuse.g, diffuse.b);
    glUniform3f(shader.GetUniformLocation(name + ".specular"), specular.r, specular.g, specular.b);
    glUniform1f(shader.GetUniformLocation(name + ".constant"), constant);
    glUniform1f(shader.GetUniformLocation(name + ".linear"), linear);
    glUniform1f(shader.GetUniformLocation(name + ".quadratic"), quadratic);
}

PointLight::PointLight(const glm::vec3 &position, GLfloat constant, GLfloat linear, GLfloat quadratic,
                       const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular) :
        position(position), constant(constant), linear(linear), quadratic(quadratic), ambient(ambient),
        diffuse(diffuse), specular(specular) {}

void PointLight::Visualize(glm::mat4 view, glm::mat4 projection) {
    cube.sp->Enable();
    glm::mat4 model;
    model = glm::translate(model, position);
    GLint model_loc = cube.sp->GetUniformLocation("model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    GLint view_loc = cube.sp->GetUniformLocation("view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
    GLint projection_loc = cube.sp->GetUniformLocation("projection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(cube.sp->GetUniformLocation("lightColor"), 1, 1, 1);
    cube.Draw(*cube.sp);
    cube.sp->Disable();
}

PointLight::PointLight(PointLight &&other) {
    position = other.position;
    constant = other.constant;
    linear = other.linear;
    quadratic = other.quadratic;
    ambient = other.ambient;
    diffuse = other.diffuse;
    specular = other.specular;
}

void SpotLight::Load(ShaderProgram &shader, std::string name) {
    glUniform3f(shader.GetUniformLocation(name + ".position"), position.x, position.y, position.z);
    glUniform3f(shader.GetUniformLocation(name + ".ambient"), ambient.r, ambient.g, ambient.b);
    glUniform3f(shader.GetUniformLocation(name + ".diffuse"), diffuse.r, diffuse.g, diffuse.b);
    glUniform3f(shader.GetUniformLocation(name + ".specular"), specular.r, specular.g, specular.b);
    glUniform1f(shader.GetUniformLocation(name + ".constant"), constant);
    glUniform1f(shader.GetUniformLocation(name + ".linear"), linear);
    glUniform1f(shader.GetUniformLocation(name + ".quadratic"), quadratic);
    glUniform3f(shader.GetUniformLocation(name + ".direction"), direction.x, direction.y, direction.z);
    glUniform1f(shader.GetUniformLocation(name + ".cutOff"), glm::cos(glm::radians(cutOff)));
    glUniform1f(shader.GetUniformLocation(name + ".outerCutOff"), glm::cos(glm::radians(outerCutOff)));
}

SpotLight::SpotLight(const glm::vec3 &position, const glm::vec3 &direction, GLfloat cutOff, GLfloat outerCutOff,
                     GLfloat constant, GLfloat linear, GLfloat quadratic, const glm::vec3 &ambient,
                     const glm::vec3 &diffuse, const glm::vec3 &specular) : position(position), direction(direction),
                                                                            cutOff(cutOff), outerCutOff(outerCutOff),
                                                                            constant(constant), linear(linear),
                                                                            quadratic(quadratic), ambient(ambient),
                                                                            diffuse(diffuse), specular(specular) {}
