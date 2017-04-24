#include "triangle.hpp"
#include "engine.hpp"
#include "loaded_model.hpp"
#include "utility.hpp"

#include <cmath>

const float PI = 3.1415927f;

//TODO: model mustn't know its coord. VBO must belong to VAO. Think about model matrix. Scene class to rule them all.
//TODO: check if shaders are vertex and fragment. Think about main loop. Engine must not depend on model.
int main() {
    sf::ContextSettings contextSettings(24, 8, 0, 4, 5, sf::ContextSettings::Attribute::Core);
    sf::Window window(sf::VideoMode(1980, 1000), "Triangle", sf::Style::Default, contextSettings);
    Engine engine(window);
    engine.main_shader = std::make_shared<ShaderProgram>(
            Shader(ReadFile("vertex.vert"), GL_VERTEX_SHADER),
            Shader(ReadFile("fragment.frag"), GL_FRAGMENT_SHADER));
    engine.white_shader = std::make_shared<ShaderProgram>(
            Shader(ReadFile("white.vert"), GL_VERTEX_SHADER),
            Shader(ReadFile("white.frag"), GL_FRAGMENT_SHADER));
    LoadedModel *nanosuit = new LoadedModel(static_cast<GLchar *>("nanosuit/nanosuit.obj"));
    engine.Assign(
            new Object(nanosuit, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 180.f, 0.f), glm::vec3(0.3f, 0.3f, 0.3f)));
    LoadedModel *cyborg = new LoadedModel(static_cast<GLchar *>("cyborg/cyborg.obj"));
    engine.Assign(
            new Object(cyborg, glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.f, 180.f, 0.f), glm::vec3(1.2f, 1.2f, 1.2f)));

    GLfloat vertices_x[] = {
            -10, 0, 0, 1, 0, 0,
            10, 0, 0, 0, 0, 1,
    };
    GLfloat vertices_z[] = {
            0, 0, -10, 0, 0, 1,
            0, 0, 10, 1, 0, 0,
    };
    for (int i = -10; i <= 10; ++i) {
        engine.Assign(new Object(new Line(vertices_x, 0, -std::sqrt(3.f) / 2, i), glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)));
        engine.Assign(new Object(new Line(vertices_z, i, -std::sqrt(3.f) / 2, 0), glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)));
    }

    DirLight dirlight(glm::vec3(-0.f, -1.0f, -0.f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.6f, 0.6f, 0.6f),
                      glm::vec3(0.5f, 0.5f, 0.5f));

    std::vector<glm::vec3> pointLightPositions = {
            glm::vec3(0.7f, 0.2f, 2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f, 2.0f, -12.0f),
            glm::vec3(0.0f, 0.0f, -3.0f)
    };

    for (int i = 0; i < pointLightPositions.size() - 2; ++i) {
        engine.AddPointLight(new PointLight(pointLightPositions[i], 1.0, 0.09, 0.032, glm::vec3(0.2f, 0.2f, 0.2f),
                                            glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f),
                                            engine.white_shader));
    }

    engine.AddPointLight(new PointLight(pointLightPositions[2], 1.0, 0.09, 0.032, glm::vec3(0.f, 0.2f, 0.f),
                                        glm::vec3(0.f, 0.8f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f), engine.white_shader));

    engine.AddPointLight(new PointLight(pointLightPositions[3], 1.0, 0.09, 0.032, glm::vec3(0.2f, 0.f, 0.f),
                                        glm::vec3(0.8f, 0.f, 0.f), glm::vec3(1.0f, 0.0f, 0.0f), engine.white_shader));

    engine.AddFlashLight(
            new SpotLight(glm::vec3(0), glm::vec3(0), 12.5f, 15.0f, 1.0f, 0.09f, 0.032f, glm::vec3(0), glm::vec3(1),
                          glm::vec3(1)));

//    LoadedModel* cube = new LoadedModel(static_cast<GLchar *>("cube.ply"));
//        engine.Assign(
//            new Object(cube, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)));
//    LoadedModel *planet = new LoadedModel(static_cast<GLchar *>("planet/planet.obj"));
//    engine.Assign(new Object(planet, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)));
//    LoadedModel *rock = new LoadedModel(static_cast<GLchar *>("rock/rock.obj"));
//    engine.Assign(rock);
    engine.MainLoop();
    return 0;
}