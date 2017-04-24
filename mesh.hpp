#ifndef TRIANGLE_MESH_HPP
#define TRIANGLE_MESH_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "shader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture {
    GLuint id;
    std::string type;
    aiString path;
};


class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    GLfloat shininess;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures,
         GLfloat shininess);


    void Load(std::shared_ptr<ShaderProgram> shader_program);

    void Draw();

    std::shared_ptr<ShaderProgram> shader;

private:
    GLuint VAO, VBO, EBO;

    void setupMesh();
};

#endif //TRIANGLE_MESH_HPP
