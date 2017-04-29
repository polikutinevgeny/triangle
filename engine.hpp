#ifndef TRIANGLE_ENGINE_HPP
#define TRIANGLE_ENGINE_HPP

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include "model.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "lightsources.hpp"


class Engine {
public:
    Engine(sf::Window& window);

    void Assign(Object *object);
    void MainLoop();
    ~Engine();

    void AddDirLight(DirLight *source);

    void AddSpotLight(SpotLight *source);

    void AddPointLight(PointLight *source);

    void AddFlashLight(SpotLight *source);

    std::shared_ptr<ShaderProgram> main_shader;
    std::shared_ptr<ShaderProgram> white_shader;
    std::shared_ptr<ShaderProgram> text_shader;

private:
    SpotLight *flashlight;
    std::vector<DirLight *> dirlights;
    std::vector<SpotLight *> spotlights;
    std::vector<PointLight *> pointlights;
    std::vector<Object *> objects;
    sf::Window& window;
};


#endif //TRIANGLE_ENGINE_HPP
