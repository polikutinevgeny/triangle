#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/Window/Mouse.hpp>
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
    GLfloat delta_time = 0.0f;
    GLfloat last_frame = 0.0f;
    GLfloat lastX = window.getSize().x / 2, lastY = window.getSize().y / 2;
    sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
    Camera camera(glm::vec3(-3, 0, -3), glm::vec3(0, 1, 0), 45, 0);
    sf::Clock clock;
    while (window.isOpen()) {
        GLfloat time_value = clock.getElapsedTime().asSeconds();
        delta_time = time_value - last_frame;
        last_frame = time_value;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                camera.ProcessMouseScroll(event.mouseWheel.delta * 0.1f);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            camera.ProcessKeyboard(FORWARD, delta_time);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            camera.ProcessKeyboard(BACKWARD, delta_time);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            camera.ProcessKeyboard(LEFT, delta_time);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            camera.ProcessKeyboard(RIGHT, delta_time);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            camera.ProcessKeyboard(UP, delta_time);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
            camera.ProcessKeyboard(DOWN, delta_time);
        }
        GLfloat xoffset = sf::Mouse::getPosition(window).x - lastX;
        GLfloat yoffset = lastY - sf::Mouse::getPosition(window).y;
        lastX = window.getSize().x / 2;
        lastY = window.getSize().y / 2;
        sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
        camera.ProcessMouseMovement(xoffset, yoffset);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_program.Enable();
        glm::mat4 projection =
                glm::perspective(glm::radians(camera.zoom), (GLfloat)window.getSize().x / (GLfloat)window.getSize().y, 0.1f, 100.f);
        GLint view_loc = shader_program.GetUniformLocation("view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        GLint projection_loc = shader_program.GetUniformLocation("projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
        for (auto it: models) {
            it->Update(time_value);
            it->Draw(shader_program);
        }
        shader_program.Disable();
        window.display();
    }
}

Engine::Engine(sf::Window& window): window(window) {
    window.setActive();
    window.setMouseCursorVisible(false);
    glewInit();
    glViewport(0, 0, window.getSize().x, window.getSize().y);
//    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
