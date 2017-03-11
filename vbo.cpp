
#include "vbo.hpp"

VBO::VBO() {
    glGenBuffers(1, &vbo_location);
}

VBO::~VBO() {
    glDeleteBuffers(1, &vbo_location);
}

void VBO::SetData(size_t size, void *location, GLenum mode) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_location);
    glBufferData(GL_ARRAY_BUFFER, size, location, mode);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::SetAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                           const GLvoid *pointer) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_location);
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
