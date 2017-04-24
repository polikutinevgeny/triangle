#include "triangle.hpp"
#include <iterator>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


Triangle::Triangle(GLfloat vertices[18], GLfloat x, GLfloat y, GLfloat z)
        : x(x), y(y), z(z), x0(x), y0(y), z0(z) {
    std::copy(vertices, vertices + 18, this->vertices);
    vao.Bind();
    vbo.SetData(sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
    vao.Unbind();
}

void Triangle::Draw() {
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(x, y, z));
    GLint model_loc = shader->GetUniformLocation("model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(shader->GetUniformLocation("FillWhite"), 1);
    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    vao.Unbind();
    glUniform1i(shader->GetUniformLocation("FillWhite"), 0);
}

void Triangle::Load(std::shared_ptr<ShaderProgram> shader_program) {
    shader = shader_program;
    vao.Bind();
    GLuint position = shader->GetAttribLocation("position");
    GLuint color = shader->GetAttribLocation("color");
    vbo.SetAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    vbo.SetAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    vao.Unbind();
}

Line::Line(GLfloat *vertices, GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {
    std::copy(vertices, vertices + 12, this->vertices);
    vao.Bind();
    vbo.SetData(sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
    vao.Unbind();
}

void Line::Load(std::shared_ptr<ShaderProgram> shader_program) {
    shader = shader_program;
    vao.Bind();
    GLuint position = shader->GetAttribLocation("position");
    GLuint color = shader->GetAttribLocation("color");
    vbo.SetAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    vbo.SetAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    vao.Unbind();
}

void Line::Draw() {
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(x, y, z));
    GLint model_loc = shader->GetUniformLocation("model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(shader->GetUniformLocation("FillWhite"), 1);
    vao.Bind();
    glDrawArrays(GL_LINES, 0, 2);
    vao.Unbind();
    glUniform1i(shader->GetUniformLocation("FillWhite"), 0);
}

void LightCube::Draw() {
    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    vao.Unbind();
}

LightCube::LightCube() {
    vertices = new GLfloat[108]{
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,

            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,

            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f
    };
    vao.Bind();
    vbo.SetData(108 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    vao.Unbind();
}

void LightCube::Load(std::shared_ptr<ShaderProgram> shader_program) {
    shader = shader_program;
    vao.Bind();
    GLuint position = shader->GetAttribLocation("position");
    vbo.SetAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    vao.Unbind();
}
