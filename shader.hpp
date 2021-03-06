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
    ShaderProgram(const Shader &vertex_shader, const Shader &fragment_shader);

    ShaderProgram(const ShaderProgram &other) = delete;

    ShaderProgram &operator=(const ShaderProgram &) = delete;
    ~ShaderProgram();
    void Enable();
    void Disable();

    GLuint GetUniformLocation(std::string name) const;

    GLuint GetAttribLocation(std::string name) const;

private:
    GLuint shader_program_loc;
};

#endif //TRIANGLE_SHADER_HPP
