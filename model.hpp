#ifndef TRIANGLE_MODEL_HPP
#define TRIANGLE_MODEL_HPP

#include "shader.hpp"


class Model {
public:
    virtual ~Model() {};
    virtual void Draw(ShaderProgram& shader_program) = 0;
};


#endif //TRIANGLE_MODEL_HPP
