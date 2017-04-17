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

    void Load(ShaderProgram &shader_program);
    void Draw(ShaderProgram& shader_program) override;

private:
    GLfloat vertices[18];
    VBO vbo;
    VAO vao;
    GLfloat x0, y0, z0, x, y, z;
};

class Line: public Model {
public:
    Line(GLfloat vertices[12], GLfloat x, GLfloat y, GLfloat z);

    void Load(ShaderProgram &shader_program);
    void Draw(ShaderProgram& shader_program) override;

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

    void Draw(ShaderProgram &shader_program) override;

    ShaderProgram *sp;

private:
    GLfloat *vertices;
    VBO vbo;
    VAO vao;
};

#endif //TRIANGLE_TRIANGLE_HPP
