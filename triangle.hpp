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
    void Load(ShaderProgram& shader_program) override;
    void Draw(ShaderProgram& shader_program) override;
    void Update(float time) override;

private:
    GLfloat vertices[18];
    VBO vbo;
    VAO vao;
    GLfloat x0, y0, z0, x, y, z;
};


#endif //TRIANGLE_TRIANGLE_HPP
