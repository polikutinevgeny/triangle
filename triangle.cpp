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
}

void Triangle::Draw(ShaderProgram& shader_program) {
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(x, y, z));
    GLint model_loc = shader_program.GetUniformLocation("model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    vao.Unbind();
}

void Triangle::Load(ShaderProgram &shader_program) {
    vao.Bind();
    vbo.SetData(sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
    GLuint position = shader_program.GetAttribLocation("position");
    GLuint color = shader_program.GetAttribLocation("color");
    vbo.SetAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    vbo.SetAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    vao.Unbind();
}

Line::Line(GLfloat *vertices, GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {
    std::copy(vertices, vertices + 12, this->vertices);
}

void Line::Load(ShaderProgram &shader_program) {
    vao.Bind();
    vbo.SetData(sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
    GLuint position = shader_program.GetAttribLocation("position");
    GLuint color = shader_program.GetAttribLocation("color");
    vbo.SetAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    vbo.SetAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    vao.Unbind();
}

void Line::Draw(ShaderProgram &shader_program) {
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(x, y, z));
    GLint model_loc = shader_program.GetUniformLocation("model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    vao.Bind();
    glDrawArrays(GL_LINES, 0, 2);
    vao.Unbind();
}

void LightCube::Draw(ShaderProgram &shader_program) {
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
    std::string vertex_shader = "#version 450 core\n"
            "layout (location = 0) in vec3 position;\n"
            "\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    gl_Position = projection * view * model * vec4(position, 1.0f);\n"
            "}";
    std::string frag_shader = "#version 330 core\n"
            "out vec4 color;\n"
            "  \n"
            "uniform vec3 lightColor;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    color = vec4(lightColor, 1.0f);\n"
            "}";
    Shader vs(vertex_shader, GL_VERTEX_SHADER);
    Shader fs(frag_shader, GL_FRAGMENT_SHADER);
    sp = new ShaderProgram(vs, fs);
    vao.Bind();
    vbo.SetData(sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
    vbo.SetAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    vao.Unbind();
}

LightCube::~LightCube() {
    delete sp;
}
