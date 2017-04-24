#ifndef TRIANGLE_MODEL_HPP
#define TRIANGLE_MODEL_HPP

#include "shader.hpp"
#include <memory>


class Model {
public:
    virtual ~Model() {};

    virtual void Load(std::shared_ptr<ShaderProgram> shader_program) = 0;

    virtual void Draw() = 0;

    std::shared_ptr<ShaderProgram> shader;
};


#endif //TRIANGLE_MODEL_HPP
