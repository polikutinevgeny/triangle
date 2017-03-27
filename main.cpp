#include "triangle.hpp"
#include "engine.hpp"
//TODO: model mustn't know its coord. VBO must belong to VAO. Think about model matrix. Scene class to rule them all.
//TODO: check if shaders are vertex and fragment. Think about main loop. Engine must not depend on model.
int main() {
    sf::ContextSettings contextSettings(24, 8, 0, 4, 5, sf::ContextSettings::Attribute::Core);
    sf::Window window(sf::VideoMode(1980, 1000), "Triangle", sf::Style::Default, contextSettings);
    Engine engine(window);
    GLfloat vertices1[] = {
            -1.0f, -4.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    GLfloat vertices2[] = {
            -4.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f
    };
    Triangle* triangle1 = new Triangle(vertices1, 1.0f, 1.0f, 2.0f);
    Triangle* triangle2 = new Triangle(vertices2, -1.0f, -1.0f, -2.0f);
    engine.Assign(triangle1);
    engine.Assign(triangle2);
    engine.MainLoop();
    return 0;
}