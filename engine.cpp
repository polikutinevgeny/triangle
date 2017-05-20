#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <SFML/Window/Mouse.hpp>
#include "engine.hpp"
#include "utility.hpp"
#include "camera.hpp"
#include "lightsources.hpp"
#include "loaded_model.hpp"
#include "object.hpp"
#include <map>
#include "text.hpp"


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
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLfloat delta_time = 0.0f;
    GLfloat last_frame = 0.0f;
    GLfloat lastX = window.getSize().x / 2, lastY = window.getSize().y / 2;
    sf::Mouse::setPosition(
            sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2),
            window);
    Camera camera(glm::vec3(-3, 0, -3), glm::vec3(0, 1, 0), 45, 0);
    sf::Clock clock;
    Printer printer(text_shader);
//    printer.LoadFont("/usr/share/fonts/TTF/Monoton-Regular.ttf");
//    printer.LoadFont("/usr/share/fonts/TTF/EuphoriaScript-Regular.ttf");
    printer.LoadFont("/usr/share/fonts/TTF/arial.ttf");


    bool editing_pl = true;
    bool editing_sl = false;
    int selected_pointlight = 0;
    int selected_spotlight = 0;
    if (flashlight) {
        flashlight->on = false;
    }

    while (window.isOpen()) {
        GLfloat time_value = clock.getElapsedTime().asSeconds();
        delta_time = time_value - last_frame;
        last_frame = time_value;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if ((event.type == sf::Event::KeyPressed) &&
                (event.key.code == sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                camera.ProcessMouseScroll(event.mouseWheel.delta * 0.1f);
            }
            if (event.type == sf::Event::KeyPressed) {
                if (editing_pl) {
                    if (event.key.code == sf::Keyboard::LBracket) {
                        selected_pointlight = std::max(0, std::min(
                                selected_pointlight - 1,
                                static_cast<int>(pointlights.size() - 1)));
                    }
                    if (event.key.code == sf::Keyboard::RBracket) {
                        selected_pointlight = std::max(0, std::min(
                                selected_pointlight + 1,
                                static_cast<int>(pointlights.size() - 1)));
                    }
                }
                if (editing_sl) {
                    if (event.key.code == sf::Keyboard::LBracket) {
                        selected_spotlight = std::max(0, std::min(
                                selected_spotlight - 1,
                                static_cast<int>(spotlights.size() - 1)));
                    }
                    if (event.key.code == sf::Keyboard::RBracket) {
                        selected_spotlight = std::max(0, std::min(
                                selected_spotlight + 1,
                                static_cast<int>(spotlights.size() - 1)));
                    }
                }
                if (event.key.code == sf::Keyboard::F && flashlight) {
                    flashlight->on = !flashlight->on;
                }
                if (event.key.code == sf::Keyboard::P) {
                    editing_pl = editing_sl;
                    editing_sl = !editing_sl;
                }
                if (event.key.code == sf::Keyboard::Slash) {
                    if (editing_pl &&
                        selected_pointlight < pointlights.size()) {
                        pointlights[selected_pointlight]->on = !pointlights[selected_pointlight]->on;
                    }
                    if (editing_sl && selected_spotlight < spotlights.size()) {
                        spotlights[selected_spotlight]->on = !spotlights[selected_spotlight]->on;
                    }
                }
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
        if (editing_pl && selected_pointlight < pointlights.size()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                pointlights[selected_pointlight]->position.x += 1 * delta_time;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                pointlights[selected_pointlight]->position.x -= 1 * delta_time;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                pointlights[selected_pointlight]->position.z += 1 * delta_time;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                pointlights[selected_pointlight]->position.z -= 1 * delta_time;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
                pointlights[selected_pointlight]->position.y += 1 * delta_time;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
                pointlights[selected_pointlight]->position.y -= 1 * delta_time;
            }
        }
        if (editing_sl && selected_spotlight < spotlights.size()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                spotlights[selected_spotlight]->position.x += 1 * delta_time;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                spotlights[selected_spotlight]->position.x -= 1 * delta_time;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                spotlights[selected_spotlight]->position.z += 1 * delta_time;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                spotlights[selected_spotlight]->position.z -= 1 * delta_time;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
                spotlights[selected_spotlight]->position.y += 1 * delta_time;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
                spotlights[selected_spotlight]->position.y -= 1 * delta_time;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma)) {
                spotlights[selected_spotlight]->direction =
                        glm::rotate(spotlights[selected_spotlight]->direction,
                                    delta_time, glm::vec3(0, 1, 0));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period)) {
                spotlights[selected_spotlight]->direction =
                        glm::rotate(spotlights[selected_spotlight]->direction,
                                    -delta_time, glm::vec3(0, 1, 0));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
                spotlights[selected_spotlight]->direction =
                        glm::rotate(spotlights[selected_spotlight]->direction,
                                    delta_time, glm::cross(
                                        spotlights[selected_spotlight]->direction,
                                        glm::vec3(0, 1, 0)));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                spotlights[selected_spotlight]->direction =
                        glm::rotate(spotlights[selected_spotlight]->direction,
                                    -delta_time, glm::cross(
                                        spotlights[selected_spotlight]->direction,
                                        glm::vec3(0, 1, 0)));
            }
        }
        GLfloat xoffset = sf::Mouse::getPosition(window).x - lastX;
        GLfloat yoffset = lastY - sf::Mouse::getPosition(window).y;
        lastX = window.getSize().x / 2;
        lastY = window.getSize().y / 2;
        sf::Mouse::setPosition(
                sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2),
                window);
        camera.ProcessMouseMovement(xoffset, yoffset);
        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (flashlight) {
            flashlight->position = camera.position;
            flashlight->direction = camera.front;
        }

        main_shader->Enable();

        glUniform1i(main_shader->GetUniformLocation("DirLightNum"),
                    static_cast<GLint>(dirlights.size()));
        glUniform1i(main_shader->GetUniformLocation("PointLightNum"),
                    static_cast<GLint>(pointlights.size()));
        glUniform1i(main_shader->GetUniformLocation("SpotLightNum"),
                    static_cast<GLint>(spotlights.size()));

        for (int i = 0; i < dirlights.size(); ++i) {
            dirlights[i]->Load(main_shader,
                               "dirLights[" + std::to_string(i) + "]");
        }
        for (int i = 0; i < pointlights.size(); ++i) {
            pointlights[i]->Load(main_shader,
                                 "pointLights[" + std::to_string(i) + "]");
        }
        for (int i = 0; i < spotlights.size(); ++i) {
            spotlights[i]->Load(main_shader,
                                "spotLights[" + std::to_string(i) + "]");
        }

        GLint viewPosLoc = main_shader->GetUniformLocation("viewPos");
        glUniform3f(viewPosLoc, camera.position.x, camera.position.y,
                    camera.position.z);

        glm::mat4 projection =
                glm::perspective(glm::radians(camera.zoom),
                                 (GLfloat) window.getSize().x /
                                 (GLfloat) window.getSize().y,
                                 0.01f, 100.f);
        GLint view_loc = main_shader->GetUniformLocation("view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE,
                           glm::value_ptr(camera.GetViewMatrix()));
        GLint projection_loc = main_shader->GetUniformLocation("projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE,
                           glm::value_ptr(projection));

        for (auto it: objects) {
            it->Draw();
        }

        main_shader->Disable();

        for (int i = 0; i < pointlights.size(); ++i) {
            pointlights[i]->Visualize(camera.GetViewMatrix(), projection);
        }

        for (int i = 0; i < spotlights.size(); ++i) {
            spotlights[i]->Visualize(camera.GetViewMatrix(), projection);
        }

        text_shader->Enable();
        projection = glm::ortho(0.0f, static_cast<GLfloat>(window.getSize().x),
                                0.0f,
                                static_cast<GLfloat>(window.getSize().y));
        glUniformMatrix4fv(text_shader->GetUniformLocation("projection"), 1,
                           GL_FALSE, glm::value_ptr(projection));
        if (editing_pl) {
            printer.RenderText("Selected pointlight #" +
                               std::to_string(selected_pointlight),
                               25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        } else if (editing_sl) {
            printer.RenderText(
                    "Selected spotlight #" + std::to_string(selected_spotlight),
                    25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        }
        text_shader->Disable();

        window.display();
    }
}

Engine::Engine(sf::Window &window) : window(window), flashlight(nullptr) {
    window.setActive();
    window.setMouseCursorVisible(false);
    glewInit();
    glViewport(0, 0, window.getSize().x, window.getSize().y);
    flashlight = nullptr;
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