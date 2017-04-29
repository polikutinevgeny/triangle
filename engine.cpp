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
#include <map>
#include "text.hpp"


//Freetype test


std::map<GLchar, Character> Characters;
GLuint VAO, VBO;

void RenderText(std::shared_ptr<ShaderProgram> shader, std::string text, GLfloat x, GLfloat y, GLfloat scale,
                glm::vec3 color) {
    shader->Enable();
    glUniform3f(shader->GetUniformLocation("textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];
        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        GLfloat vertices[6][4] = {
                {xpos,     ypos + h, 0.0, 0.0},
                {xpos,     ypos,     0.0, 1.0},
                {xpos + w, ypos,     1.0, 1.0},

                {xpos,     ypos + h, 0.0, 0.0},
                {xpos + w, ypos,     1.0, 1.0},
                {xpos + w, ypos + h, 1.0, 0.0}
        };
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


//f_t


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
    // Freetype test
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    FT_Face face;
    if (FT_New_Face(ft, "/usr/share/fonts/TTF/comic.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Freetype test



    glEnable(GL_CULL_FACE);
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

        if (flashlight) {
            flashlight->position = camera.position;
            flashlight->direction = camera.front;
        }

        main_shader->Enable();

        glUniform1i(main_shader->GetUniformLocation("DirLightNum"), static_cast<GLint>(dirlights.size()));
        glUniform1i(main_shader->GetUniformLocation("PointLightNum"), static_cast<GLint>(pointlights.size()));
        glUniform1i(main_shader->GetUniformLocation("SpotLightNum"), static_cast<GLint>(spotlights.size()));

        for (int i = 0; i < dirlights.size(); ++i) {
            dirlights[i]->Load(main_shader, "dirLights[" + std::to_string(i) + "]");
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

        //Freetype test
        text_shader->Enable();
        projection = glm::ortho(0.0f, static_cast<GLfloat>(window.getSize().x), 0.0f,
                                static_cast<GLfloat>(window.getSize().y));
        glUniformMatrix4fv(text_shader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
        RenderText(text_shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        text_shader->Disable();
        //Freetype test

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