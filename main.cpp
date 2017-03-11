#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utility.hpp"


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
    // VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    // Vertex shader
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexShaderSource = ReadFile("vertex.vert");
    const char* vertexShaderSourceString = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSourceString, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error: vertex shader compilation failed: " << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderSource = ReadFile("fragment.frag");
    const char* fragmentShaderSourceString = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceString, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error: fragment shader compilation failed\n" << infoLog << std::endl;
    }
    // Shader program
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error: shader program linking failed\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
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
        glUseProgram(shaderProgram);
        GLfloat timeValue = clock.getElapsedTime().asSeconds();
        glm::mat4 trans = glm::perspective(
                glm::radians(60.0f), (GLfloat)window.getSize().x / (GLfloat)window.getSize().y, 0.1f, 10.0f);
        trans = glm::translate(trans, glm::vec3(cosf(timeValue), sinf(timeValue), -3.0 + sinf(timeValue)));
        trans = glm::rotate(trans, timeValue, glm::vec3(0.0, 0.0, 1.0));
        GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        GLint timeLoc = glGetUniformLocation(shaderProgram, "time");
        glUniform1f(timeLoc, timeValue);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        window.display();
    }
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    return EXIT_SUCCESS;
}