#ifndef TRIANGLE_SHADER_HPP
#define TRIANGLE_SHADER_HPP

#include <string>
#include <GL/glew.h>

class ShaderProgram;

class Shader {
    friend class ShaderProgram;
public:
    Shader(std::string source, GLenum type);
    ~Shader();

private:
    GLuint shader_loc;
};


class ShaderProgram {
public:
    ShaderProgram(Shader vertex_shader, Shader fragment_shader);
    ~ShaderProgram();
    void Use();
    void Unuse();
    GLint GetUniformLocation(std::string name);

private:
    GLuint shader_program_loc;
};

#endif //TRIANGLE_SHADER_HPP
