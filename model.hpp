#ifndef TRIANGLE_MODEL_HPP
#define TRIANGLE_MODEL_HPP

#include "shader.hpp"


class Model {
public:
    virtual ~Model() {};
    virtual void Load(ShaderProgram& shader_program) = 0;
    virtual void Draw(ShaderProgram& shader_program) = 0;
    virtual void Update(float time) = 0;
};


#endif //TRIANGLE_MODEL_HPP
