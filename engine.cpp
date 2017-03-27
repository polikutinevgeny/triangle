#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "engine.hpp"
#include "utility.hpp"
#include "camera.hpp"


void Engine::Assign(Model *model) {
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
    Camera camera(glm::vec3(0, 0, 5.f), glm::vec3(0, 0, 0));
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
        shader_program.Enable();
        GLfloat timeValue = clock.getElapsedTime().asSeconds();
        camera.MoveTo(glm::vec3(sin(timeValue) * 5, 0, cos(timeValue) * 5));
//        camera.LookAt(glm::vec3(sin(timeValue) * 3, 0, 0));
        glm::mat4 projection =
                glm::perspective(45.0f, (GLfloat)window.getSize().x / (GLfloat)window.getSize().y, 0.1f, 100.f);
        GLint view_loc = shader_program.GetUniformLocation("view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera.View()));
        GLint projection_loc = shader_program.GetUniformLocation("projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
        for (auto it: models) {
            it->Update(timeValue);
            it->Draw(shader_program);
        }
        shader_program.Disable();
        window.display();
    }
}

Engine::Engine(sf::Window& window): window(window) {
    window.setActive();
    glewInit();
    glViewport(0, 0, window.getSize().x, window.getSize().y);
//    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
