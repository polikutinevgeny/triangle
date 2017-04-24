#ifndef TRIANGLE_LIGHTSOURCES_HPP
#define TRIANGLE_LIGHTSOURCES_HPP

#include <glm/glm.hpp>
#include "GL/glew.h"
#include "shader.hpp"
#include "triangle.hpp"
#include <memory>

struct DirLight {
    DirLight(const glm::vec3 &direction, const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular);

    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    void Load(std::shared_ptr<ShaderProgram> shader, std::string name);
};

struct PointLight {
    PointLight(const glm::vec3 &position, GLfloat constant, GLfloat linear, GLfloat quadratic, const glm::vec3 &ambient,
               const glm::vec3 &diffuse, const glm::vec3 &specular, std::shared_ptr<ShaderProgram> vis_shader);

    PointLight(PointLight &&other);

    glm::vec3 position;
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    LightCube cube;

    std::shared_ptr<ShaderProgram> vis_shader;

    void Load(std::shared_ptr<ShaderProgram> shader, std::string name);

    void Visualize(glm::mat4 view, glm::mat4 projection);
};

struct SpotLight {
    SpotLight(const glm::vec3 &position, const glm::vec3 &direction, GLfloat cutOff, GLfloat outerCutOff,
              GLfloat constant, GLfloat linear, GLfloat quadratic, const glm::vec3 &ambient, const glm::vec3 &diffuse,
              const glm::vec3 &specular);

    glm::vec3 position;
    glm::vec3 direction;
    GLfloat cutOff;
    GLfloat outerCutOff;
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    void Load(std::shared_ptr<ShaderProgram> shader, std::string name);
};

#endif //TRIANGLE_LIGHTSOURCES_HPP
