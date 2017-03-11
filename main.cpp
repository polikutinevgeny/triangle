#include <string>
#include <cmath>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utility.hpp"
#include "shader.hpp"
#include "vbo.hpp"
#include "vao.hpp"


int main() {
    sf::ContextSettings contextSettings(24, 8, 1, 4, 5, sf::ContextSettings::Attribute::Core);
    sf::Window window(
            sf::VideoMode(640, 480), "Triangle",
            sf::Style::Default, contextSettings);
    window.setActive();
    glewInit();
    glViewport(0, 0, window.getSize().x, window.getSize().y);
    GLfloat vertices[] = {
            -1.0f, -1.0f, -0.5f, 1.0f, 0.0f, 0.0f,
             0.0f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 1.0f
    };
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    VAO vao;
    vao.Bind();
    VBO vbo;
    vbo.SetData(sizeof(vertices), vertices, GL_STATIC_DRAW);
    vbo.SetAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    vbo.SetAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    vao.Unbind();
    Shader vertex_shader(ReadFile("vertex.vert"), GL_VERTEX_SHADER);
    Shader fragment_shader(ReadFile("fragment.frag"), GL_FRAGMENT_SHADER);
    ShaderProgram shader_program(vertex_shader, fragment_shader);
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_program.Use();
        GLfloat timeValue = clock.getElapsedTime().asSeconds();
        glm::mat4 trans = glm::perspective(
                glm::radians(60.0f), (GLfloat)window.getSize().x / (GLfloat)window.getSize().y, 0.1f, 10.0f);
        trans = glm::translate(trans, glm::vec3(cosf(timeValue), sinf(timeValue), -3.0 + sinf(timeValue)));
        trans = glm::rotate(trans, timeValue, glm::vec3(0.0, 0.0, 1.0));
        GLint transformLoc = shader_program.GetUniformLocation("transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        GLint timeLoc = shader_program.GetUniformLocation("time");
        glUniform1f(timeLoc, timeValue);
        vao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        vao.Unbind();
        shader_program.Unuse();
        window.display();
    }
    return EXIT_SUCCESS;
}