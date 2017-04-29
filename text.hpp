#ifndef TRIANGLE_TEXT_HPP
#define TRIANGLE_TEXT_HPP

#include <freetype2/ft2build.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include FT_FREETYPE_H
#include "vao.hpp"
#include "vbo.hpp"


struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};


class Font {
public:
    std::map<GLchar, Character> characters;
private:
    VAO vao;
    VBO vbo;
};


#endif //TRIANGLE_TEXT_HPP
