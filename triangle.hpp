#ifndef TRIANGLE_TRIANGLE_HPP
#define TRIANGLE_TRIANGLE_HPP

#include "model.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include "shader.hpp"
#include <GL/glew.h>


class Triangle: public Model {
public:
    Triangle(GLfloat vertices[18], GLfloat x, GLfloat y, GLfloat z);

    void Load(std::shared_ptr<ShaderProgram> shader_program) override;

    void Draw() override;

private:
    GLfloat vertices[18];
    VBO vbo;
    VAO vao;
    GLfloat x0, y0, z0, x, y, z;
};

class Line: public Model {
public:
    Line(GLfloat vertices[12], GLfloat x, GLfloat y, GLfloat z);

    void Load(std::shared_ptr<ShaderProgram> shader_program) override;

    void Draw() override;

private:
    GLfloat vertices[12];
    VBO vbo;
    VAO vao;
    GLfloat x, y, z;
};

class LightCube : public Model {
public:
    LightCube();

    ~LightCube();

    void Load(std::shared_ptr<ShaderProgram> shader_program) override;

    void Draw() override;

    std::shared_ptr<ShaderProgram> sp;

    GLfloat *vertices;
    VBO vbo;
    VAO vao;
};

class Dirt : public Model {
public:
    Dirt();

    ~Dirt();

    void Load(std::shared_ptr<ShaderProgram> shader_program) override;

    void Draw() override;

    std::shared_ptr<ShaderProgram> sp;

    GLfloat *vertices;
    VBO vbo;
    VAO vao;
};

#endif //TRIANGLE_TRIANGLE_HPP
