#include "triangle.hpp"
#include "engine.hpp"
#include "loaded_model.hpp"

#include <cmath>

const float PI = 3.1415927f;

//TODO: model mustn't know its coord. VBO must belong to VAO. Think about model matrix. Scene class to rule them all.
//TODO: check if shaders are vertex and fragment. Think about main loop. Engine must not depend on model.
int main() {
    sf::ContextSettings contextSettings(24, 8, 0, 4, 5, sf::ContextSettings::Attribute::Core);
    sf::Window window(sf::VideoMode(1980, 1000), "Triangle", sf::Style::Default, contextSettings);
    Engine engine(window);
    LoadedModel *nanosuit = new LoadedModel(static_cast<GLchar *>("nanosuit/nanosuit.obj"));
    engine.Assign(nanosuit);
    engine.MainLoop();
    return 0;
}