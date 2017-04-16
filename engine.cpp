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
    GLfloat delta_time = 0.0f;
    GLfloat last_frame = 0.0f;
    GLfloat lastX = window.getSize().x / 2, lastY = window.getSize().y / 2;
    sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
    Camera camera(glm::vec3(-3, 0, -3), glm::vec3(0, 1, 0), 45, 0);
    sf::Clock clock;

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Lighting test
    glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f, 0.2f, 2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f, 2.0f, -12.0f),
            glm::vec3(0.0f, 0.0f, -3.0f)
    };
    //Lighting test

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
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_program.Enable();
        //Lighting test
        glUniform1i(shader_program.GetUniformLocation("PointLightNum"), 4);
        glUniform1i(shader_program.GetUniformLocation("SpotLightNum"), 1);

        GLint viewPosLoc = shader_program.GetUniformLocation("viewPos");
        glUniform3f(viewPosLoc, camera.position.x, camera.position.y, camera.position.z);
        glUniform1f(shader_program.GetUniformLocation("material.shininess"), 16.0f);

        glUniform3f(shader_program.GetUniformLocation("dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(shader_program.GetUniformLocation("dirLight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(shader_program.GetUniformLocation("dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
        glUniform3f(shader_program.GetUniformLocation("dirLight.specular"), 0.5f, 0.5f, 0.5f);
        // Point light 1
        glUniform3f(shader_program.GetUniformLocation("pointLights[0].position"), pointLightPositions[0].x,
                    pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(shader_program.GetUniformLocation("pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(shader_program.GetUniformLocation("pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(shader_program.GetUniformLocation("pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(shader_program.GetUniformLocation("pointLights[0].constant"), 1.0f);
        glUniform1f(shader_program.GetUniformLocation("pointLights[0].linear"), 0.09);
        glUniform1f(shader_program.GetUniformLocation("pointLights[0].quadratic"), 0.032);
        // Point light 2
        glUniform3f(shader_program.GetUniformLocation("pointLights[1].position"), pointLightPositions[1].x,
                    pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(shader_program.GetUniformLocation("pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(shader_program.GetUniformLocation("pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(shader_program.GetUniformLocation("pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(shader_program.GetUniformLocation("pointLights[1].constant"), 1.0f);
        glUniform1f(shader_program.GetUniformLocation("pointLights[1].linear"), 0.09);
        glUniform1f(shader_program.GetUniformLocation("pointLights[1].quadratic"), 0.032);
        // Point light 3
        glUniform3f(shader_program.GetUniformLocation("pointLights[2].position"), pointLightPositions[2].x,
                    pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(shader_program.GetUniformLocation("pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(shader_program.GetUniformLocation("pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(shader_program.GetUniformLocation("pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(shader_program.GetUniformLocation("pointLights[2].constant"), 1.0f);
        glUniform1f(shader_program.GetUniformLocation("pointLights[2].linear"), 0.09);
        glUniform1f(shader_program.GetUniformLocation("pointLights[2].quadratic"), 0.032);
        // Point light 4
        glUniform3f(shader_program.GetUniformLocation("pointLights[3].position"), pointLightPositions[3].x,
                    pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(shader_program.GetUniformLocation("pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(shader_program.GetUniformLocation("pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(shader_program.GetUniformLocation("pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(shader_program.GetUniformLocation("pointLights[3].constant"), 1.0f);
        glUniform1f(shader_program.GetUniformLocation("pointLights[3].linear"), 0.09);
        glUniform1f(shader_program.GetUniformLocation("pointLights[3].quadratic"), 0.032);
        // SpotLight (tied to camera)
        glUniform3f(shader_program.GetUniformLocation("spotLights[0].position"), camera.position.x, camera.position.y,
                    camera.position.z);
        glUniform3f(shader_program.GetUniformLocation("spotLights[0].direction"), camera.front.x, camera.front.y,
                    camera.front.z);
        glUniform3f(shader_program.GetUniformLocation("spotLights[0].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(shader_program.GetUniformLocation("spotLights[0].diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(shader_program.GetUniformLocation("spotLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(shader_program.GetUniformLocation("spotLights[0].constant"), 1.0f);
        glUniform1f(shader_program.GetUniformLocation("spotLights[0].linear"), 0.09);
        glUniform1f(shader_program.GetUniformLocation("spotLights[0].quadratic"), 0.032);
        glUniform1f(shader_program.GetUniformLocation("spotLights[0].cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(shader_program.GetUniformLocation("spotLights[0].outerCutOff"), glm::cos(glm::radians(15.0f)));
        //Lighting test
        glm::mat4 projection =
                glm::perspective(glm::radians(camera.zoom), (GLfloat)window.getSize().x / (GLfloat)window.getSize().y, 0.1f, 100.f);
        GLint view_loc = shader_program.GetUniformLocation("view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        GLint projection_loc = shader_program.GetUniformLocation("projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        model = glm::rotate(model, (glm::mediump_float) 180, glm::vec3(0.f, 1.f, 0.f));
        glUniformMatrix4fv(shader_program.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

        for (auto it: models) {
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
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
