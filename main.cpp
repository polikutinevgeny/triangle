#include "triangle.hpp"
#include "engine.hpp"


int main() {
    sf::ContextSettings contextSettings(24, 8, 1, 4, 5, sf::ContextSettings::Attribute::Core);
    sf::Window window(sf::VideoMode(640, 480), "Triangle", sf::Style::Default, contextSettings);
    Engine engine(window);
    GLfloat vertices1[] = {
            -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    GLfloat vertices2[] = {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f
    };
    Triangle* triangle1 = new Triangle(vertices1, 1.0f, 1.0f, -0.1f);
    Triangle* triangle2 = new Triangle(vertices2, -1.0f, -1.0f, -2.0f);
    engine.LoadModel(triangle1);
    engine.LoadModel(triangle2);
    engine.MainLoop();
    return EXIT_SUCCESS;
}