#include "vao.hpp"


VAO::VAO() {
    glGenVertexArrays(1, &vao_location);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &vao_location);
}

void VAO::Bind() {
    glBindVertexArray(vao_location);
}

void VAO::Unbind() {
    glBindVertexArray(0);
}
