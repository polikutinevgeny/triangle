#ifndef TRIANGLE_VBO_HPP
#define TRIANGLE_VBO_HPP

#include <GL/glew.h>

class VBO {
public:
    VBO();
    ~VBO();
    void SetData(size_t size, void* location, GLenum mode);
    void SetAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized,
                          GLsizei stride, const GLvoid* pointer);

private:
    GLuint vbo_location;

};

#endif //TRIANGLE_VBO_HPP
