#ifndef TRIANGLE_TEXT_HPP
#define TRIANGLE_TEXT_HPP

#include <freetype2/ft2build.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <memory>
#include FT_FREETYPE_H
#include "shader.hpp"


struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};


class Printer {
public:
    Printer(std::shared_ptr<ShaderProgram> shader);

    ~Printer();

    void LoadFont(std::string path);

    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
    std::map<GLchar, Character> characters;
    std::shared_ptr<ShaderProgram> shader;
private:
    GLuint VAO, VBO;
};


#endif //TRIANGLE_TEXT_HPP
