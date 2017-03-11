#ifndef TRIANGLE_TRIANGLE_HPP
#define TRIANGLE_TRIANGLE_HPP

#include "model.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include <GL/glew.h>


class Triangle: public Model {
public:
    Triangle(GLfloat vertices[9]);
    void Draw() override;

private:
    GLfloat vertices[9];
    VBO vbo;
    VAO vao;
};


#endif //TRIANGLE_TRIANGLE_HPP
