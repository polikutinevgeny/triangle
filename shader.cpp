#include <stdexcept>
#include "shader.hpp"

Shader::Shader(std::string source, GLenum type) {
    shader_loc = glCreateShader(type);
    const char* source_str = source.c_str();
    glShaderSource(shader_loc, 1, &source_str, NULL);
    glCompileShader(shader_loc);
    GLint success;
    GLchar info_log[512];
    glGetShaderiv(shader_loc, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader_loc, 512, NULL, info_log);
        throw std::runtime_error(std::string("Error: shader compilation failed\n") + std::string(info_log));
    }
}

Shader::~Shader() {
    glDeleteShader(shader_loc);
}

ShaderProgram::ShaderProgram(Shader vertex_shader, Shader fragment_shader) {
    shader_program_loc = glCreateProgram();
    glAttachShader(shader_program_loc, vertex_shader.shader_loc);
    glAttachShader(shader_program_loc, fragment_shader.shader_loc);
    glLinkProgram(shader_program_loc);
    GLint success;
    GLchar info_log[512];
    glGetProgramiv(shader_program_loc, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader_program_loc, 512, NULL, info_log);
        throw std::runtime_error(std::string("Error: shader program linking failed\n") + std::string(info_log));
    }
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(shader_program_loc);
}

void ShaderProgram::Use() {
    glUseProgram(shader_program_loc);
}

void ShaderProgram::Unuse() {
    glUseProgram(0);
}

GLint ShaderProgram::GetUniformLocation(std::string name) {
    return glGetUniformLocation(shader_program_loc, name.c_str());;
}
