#ifndef TRIANGLE_LIGHTSOURCES_HPP
#define TRIANGLE_LIGHTSOURCES_HPP

#include <glm/glm.hpp>
#include "GL/glew.h"
#include "shader.hpp"
#include "triangle.hpp"
#include <memory>


struct Light {
    virtual ~Light() {};

    virtual void Load(std::shared_ptr<ShaderProgram> shader, std::string name) = 0;

    virtual void Visualize(glm::mat4 view, glm::mat4 projection) = 0;
};


struct DirLight : Light {
    DirLight(const glm::vec3 &direction, const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular);

    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    void Load(std::shared_ptr<ShaderProgram> shader, std::string name) override;

    void Visualize(glm::mat4 view, glm::mat4 projection) override;
};

struct PointLight : Light {
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

    void Load(std::shared_ptr<ShaderProgram> shader, std::string name) override;

    void Visualize(glm::mat4 view, glm::mat4 projection) override;
};

struct SpotLight : Light {
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
    bool on;

    void Load(std::shared_ptr<ShaderProgram> shader, std::string name) override;

    void Visualize(glm::mat4 view, glm::mat4 projection) override;
};

#endif //TRIANGLE_LIGHTSOURCES_HPP
