#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/Window/Mouse.hpp>
#include "engine.hpp"
#include "utility.hpp"
#include "camera.hpp"
#include "lightsources.hpp"
#include "loaded_model.hpp"
#include "object.hpp"
//#include <freetype2/ft2build.h>
//#include FT_FREETYPE_H


void Engine::Assign(Object *object) {
    objects.push_back(object);
    object->Load(main_shader);
}

Engine::~Engine() {
    for (auto it: objects) {
        delete (it);
    }
    for (auto it: dirlights) {
        delete (it);
    }
    for (auto it: spotlights) {
        delete (it);
    }
    for (auto it: pointlights) {
        delete (it);
    }
}

void Engine::MainLoop() {
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLfloat delta_time = 0.0f;
    GLfloat last_frame = 0.0f;
    GLfloat lastX = window.getSize().x / 2, lastY = window.getSize().y / 2;
    sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
    Camera camera(glm::vec3(-3, 0, -3), glm::vec3(0, 1, 0), 45, 0);
    sf::Clock clock;

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            camera.ProcessKeyboard(BACKWARD, delta_time);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            camera.ProcessKeyboard(LEFT, delta_time);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            camera.ProcessKeyboard(RIGHT, delta_time);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            camera.ProcessKeyboard(UP, delta_time);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
            camera.ProcessKeyboard(DOWN, delta_time);
        }
        GLfloat xoffset = sf::Mouse::getPosition(window).x - lastX;
        GLfloat yoffset = lastY - sf::Mouse::getPosition(window).y;
        lastX = window.getSize().x / 2;
        lastY = window.getSize().y / 2;
        sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
        camera.ProcessMouseMovement(xoffset, yoffset);
        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        flashlight->position = camera.position;
        flashlight->direction = camera.front;

        main_shader->Enable();

        glUniform1i(main_shader->GetUniformLocation("DirLightNum"), static_cast<GLint>(dirlights.size()));
        glUniform1i(main_shader->GetUniformLocation("PointLightNum"), static_cast<GLint>(pointlights.size()));
        glUniform1i(main_shader->GetUniformLocation("SpotLightNum"), static_cast<GLint>(spotlights.size()));

        for (int i = 0; i < dirlights.size(); ++i) {
            dirlights[i]->Load(main_shader, "pointLights[" + std::to_string(i) + "]");
        }
        for (int i = 0; i < pointlights.size(); ++i) {
            pointlights[i]->Load(main_shader, "pointLights[" + std::to_string(i) + "]");
        }
        for (int i = 0; i < spotlights.size(); ++i) {
            spotlights[i]->Load(main_shader, "spotLights[" + std::to_string(i) + "]");
        }

        GLint viewPosLoc = main_shader->GetUniformLocation("viewPos");
        glUniform3f(viewPosLoc, camera.position.x, camera.position.y, camera.position.z);

        glm::mat4 projection =
                glm::perspective(glm::radians(camera.zoom), (GLfloat) window.getSize().x / (GLfloat) window.getSize().y,
                                 0.1f, 100.f);
        GLint view_loc = main_shader->GetUniformLocation("view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        GLint projection_loc = main_shader->GetUniformLocation("projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

        for (auto it: objects) {
            it->Draw();
        }

        main_shader->Disable();

        for (int i = 0; i < pointlights.size(); ++i) {
            pointlights[i]->Visualize(camera.GetViewMatrix(), projection);
        }

        window.display();
    }
}

Engine::Engine(sf::Window &window) : window(window), flashlight(nullptr) {
    window.setActive();
    window.setMouseCursorVisible(false);
    glewInit();
    glViewport(0, 0, window.getSize().x, window.getSize().y);
}

void Engine::AddDirLight(DirLight *source) {
    dirlights.push_back(source);
}

void Engine::AddSpotLight(SpotLight *source) {
    spotlights.push_back(source);
}

void Engine::AddPointLight(PointLight *source) {
    pointlights.push_back(source);
}

void Engine::AddFlashLight(SpotLight *source) {
    spotlights.push_back(source);
    flashlight = source;
}
