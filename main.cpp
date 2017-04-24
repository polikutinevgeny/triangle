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
//    engine.main_shader = std::make_shared<ShaderProgram>(
//            Shader(ReadFile("vertex.vert"), GL_VERTEX_SHADER),
//            Shader(ReadFile("fragment.frag"), GL_FRAGMENT_SHADER));
    engine.main_shader = std::make_shared<ShaderProgram>(
            Shader(ReadFile("white.vert"), GL_VERTEX_SHADER),
            Shader(ReadFile("white.frag"), GL_FRAGMENT_SHADER));
    engine.white_shader = std::make_shared<ShaderProgram>(
            Shader(ReadFile("white.vert"), GL_VERTEX_SHADER),
            Shader(ReadFile("white.frag"), GL_FRAGMENT_SHADER));
    LoadedModel *nanosuit = new LoadedModel(static_cast<GLchar *>("nanosuit/nanosuit.obj"));
    engine.Assign(
            new Object(nanosuit, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 180.f, 0.f), glm::vec3(0.3f, 0.3f, 0.3f)));
    LoadedModel *cyborg = new LoadedModel(static_cast<GLchar *>("cyborg/cyborg.obj"));
    engine.Assign(
            new Object(cyborg, glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.f, 180.f, 0.f), glm::vec3(1.2f, 1.2f, 1.2f)));

    GLfloat vertices1[] = {
            1, -std::sqrt(3.f) / 2, 0, 1, 0, 0,
            std::cos(PI * 2 / 3), -std::sqrt(3.f) / 2, std::sin(PI * 2 / 3), 1, 0, 0,
            std::cos(-PI * 2 / 3), -std::sqrt(3.f) / 2, std::sin(-PI * 2 / 3), 1, 0, 0,
    };
//    engine.Assign(new Object(new Triangle(vertices1, 0, 0, 0), glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.f, 180.f, 0.f), glm::vec3(1.2f, 1.2f, 1.2f)));
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