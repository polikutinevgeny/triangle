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

    DirLight dirlight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f),
                      glm::vec3(0.5f, 0.5f, 0.5f));
    std::vector<PointLight> pointlights;
    std::vector<SpotLight> spotlights;
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    std::vector<glm::vec3> pointLightPositions = {
            glm::vec3(0.7f, 0.2f, 2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f, 2.0f, -12.0f),
            glm::vec3(0.0f, 0.0f, -3.0f)
    };
    for (int i = 0; i < pointLightPositions.size(); ++i) {
        pointlights.push_back(PointLight(pointLightPositions[i], 1.0, 0.09, 0.032, glm::vec3(0.05f, 0.05f, 0.05f),
                                         glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), white_shader));
    }
    spotlights.push_back(
            SpotLight(camera.position, camera.front, 12.5f, 15.0f, 1.0f, 0.09f, 0.032f, glm::vec3(0), glm::vec3(1),
                      glm::vec3(1)));

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

        spotlights[0].position = camera.position;
        spotlights[0].direction = camera.front;

        main_shader->Enable();

        glUniform1i(main_shader->GetUniformLocation("PointLightNum"), static_cast<GLint>(pointlights.size()));
        glUniform1i(main_shader->GetUniformLocation("SpotLightNum"), static_cast<GLint>(spotlights.size()));

        for (int i = 0; i < pointlights.size(); ++i) {
            pointlights[i].Load(main_shader, "pointLights[" + std::to_string(i) + "]");
        }
        for (int i = 0; i < spotlights.size(); ++i) {
            spotlights[i].Load(main_shader, "spotLights[" + std::to_string(i) + "]");
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

        glUniform3f(main_shader->GetUniformLocation("lightColor"), 1, 1, 1);

        for (auto it: objects) {
            it->Draw();
        }

        main_shader->Disable();

        for (int i = 0; i < pointlights.size(); ++i) {
            pointlights[i].Visualize(camera.GetViewMatrix(), projection);
        }

        window.display();
    }
}

Engine::Engine(sf::Window &window) : window(window) {
    window.setActive();
    window.setMouseCursorVisible(false);
    glewInit();
    glViewport(0, 0, window.getSize().x, window.getSize().y);
}
