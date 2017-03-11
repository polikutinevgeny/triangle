#ifndef TRIANGLE_VAO_HPP
#define TRIANGLE_VAO_HPP

#include <GL/glew.h>


class VAO {
public:
    VAO();
    ~VAO();
    void Bind();
    void Unbind();

private:
    GLuint vao_location;
};

#endif //TRIANGLE_VAO_HPP
