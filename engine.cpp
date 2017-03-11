#include "engine.hpp"
#include <string>
#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utility.hpp"
#include "shader.hpp"
#include "triangle.hpp"


void Engine::LoadModel(Model* model) {
    models.push_back(model);
}

Engine::~Engine() {
    for(auto it: models) {
        delete(it);
    }
}

void Engine::MainLoop() {
    Shader vertex_shader(ReadFile("vertex.vert"), GL_VERTEX_SHADER);
    Shader fragment_shader(ReadFile("fragment.frag"), GL_FRAGMENT_SHADER);
    ShaderProgram shader_program(vertex_shader, fragment_shader);
    for (auto it: models) {
        it->Load(shader_program);
    }
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
        for (auto it: models) {
            it->Update(timeValue);
            it->Draw(shader_program);
        }
        shader_program.Unuse();
        window.display();
    }
}

Engine::Engine(sf::Window& window): window(window) {
    window.setActive();
    glewInit();
    glViewport(0, 0, window.getSize().x, window.getSize().y);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
