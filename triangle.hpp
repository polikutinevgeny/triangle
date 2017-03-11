#ifndef TRIANGLE_TRIANGLE_HPP
#define TRIANGLE_TRIANGLE_HPP

#include "model.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include <GL/glew.h>


class Triangle: public Model {
public:
    Triangle(GLfloat vertices[18]);
    void Draw() override;
    void Update(float time) override;

private:
    GLfloat vertices[18];
    VBO vbo;
    VAO vao;
};


#endif //TRIANGLE_TRIANGLE_HPP
