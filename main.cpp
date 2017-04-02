#include "triangle.hpp"
#include "engine.hpp"

#include <cmath>

const float PI = 3.1415927f;

//TODO: model mustn't know its coord. VBO must belong to VAO. Think about model matrix. Scene class to rule them all.
//TODO: check if shaders are vertex and fragment. Think about main loop. Engine must not depend on model.
int main() {
    sf::ContextSettings contextSettings(24, 8, 0, 4, 5, sf::ContextSettings::Attribute::Core);
    sf::Window window(sf::VideoMode(1980, 1000), "Triangle", sf::Style::Default, contextSettings);
    Engine engine(window);
//    GLfloat vertices1[] = {
//            -1.0.f, -1.0.f, 0.0.f, 1.0.f, 0.0.f, 0.0.f,
//             0.0.f,  1.0.f, 0.0.f, 0.0.f, 1.0.f, 0.0.f,
//             1.0.f, -1.0.f, 0.0.f, 0.0.f, 0.0.f, 1.0.f
//    };
//    GLfloat vertices2[] = {
//            -1.0.f, -1.0.f, 0.0.f, 0.0.f, 0.0.f, 1.0.f,
//             0.0.f,  1.0.f, 0.0.f, 0.0.f, 1.0.f, 0.0.f,
//             1.0.f, -1.0.f, 0.0.f, 1.0.f, 0.0.f, 0.0.f
//    };
//    Triangle* triangle1 = new Triangle(vertices1, 1.0.f, 1.0.f, 2.0.f);
//    Triangle* triangle2 = new Triangle(vertices2, -1.0.f, -1.0.f, -2.0.f);
//    engine.Assign(triangle1);
//    engine.Assign(triangle2);
    GLfloat vertices1[] = {
            1, -std::sqrt(3.f) / 2, 0, 1, 0, 0,
            std::cos(PI * 2 / 3), -std::sqrt(3.f) / 2, std::sin(PI * 2 / 3), 1, 0, 0,
            std::cos(-PI * 2 / 3), -std::sqrt(3.f) / 2, std::sin(-PI * 2 / 3), 1, 0, 0,
    };
    GLfloat vertices2[] = {
            0, std::sqrt(3.f) / 2, 0, 0, 1, 0,
            std::cos(PI * 2 / 3), -std::sqrt(3.f) / 2, std::sin(PI * 2 / 3), 0, 1, 0,
            std::cos(-PI * 2 / 3), -std::sqrt(3.f) / 2, std::sin(-PI * 2 / 3), 0, 1, 0,
    };
    GLfloat vertices3[] = {
            1, -std::sqrt(3.f) / 2, 0, 0, 0, 1,
            0, std::sqrt(3.f) / 2, 0, 0, 0, 1,
            std::cos(-PI * 2 / 3), -std::sqrt(3.f) / 2, std::sin(-PI * 2 / 3), 0, 0, 1,
    };
    GLfloat vertices4[] = {
            1, -std::sqrt(3.f) / 2, 0, 1, 0, 1,
            std::cos(PI * 2 / 3), -std::sqrt(3.f) / 2, std::sin(PI * 2 / 3), 1, 0, 1,
            0, std::sqrt(3.f) / 2, 0, 1, 0, 1,
    };
    Triangle* triangle1 = new Triangle(vertices1, 0.f, 0.f, 0.f);
    Triangle* triangle2 = new Triangle(vertices2, 0.f, 0.f, 0.f);
    Triangle* triangle3 = new Triangle(vertices3, 0.f, 0.f, 0.f);
    Triangle* triangle4 = new Triangle(vertices4, 0.f, 0.f, 0.f);
    engine.Assign(triangle1);
    engine.Assign(triangle2);
    engine.Assign(triangle3);
    engine.Assign(triangle4);
    GLfloat vertices_x[] = {
            -10, 0, 0, 0, 0, 0,
            10, 0, 0, 0, 0, 0,
    };
    GLfloat vertices_z[] = {
            0, 0, -10, 0, 0, 0,
            0, 0, 10, 0, 0, 0,
    };
    for (int i = -10; i <= 10; ++i) {
        engine.Assign(new Line(vertices_x, 0, 0, i));
        engine.Assign(new Line(vertices_z, i, 0, 0));
    }
    engine.MainLoop();
    return 0;
}