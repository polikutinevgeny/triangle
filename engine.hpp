#ifndef TRIANGLE_ENGINE_HPP
#define TRIANGLE_ENGINE_HPP

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include "model.hpp"
#include "shader.hpp"
#include "object.hpp"


class Engine {
public:
    Engine(sf::Window& window);

    void Assign(Object *object);
    void MainLoop();
    ~Engine();

    std::shared_ptr<ShaderProgram> main_shader;
    std::shared_ptr<ShaderProgram> white_shader;

private:
    std::vector<Object *> objects;
    sf::Window& window;
};


#endif //TRIANGLE_ENGINE_HPP
