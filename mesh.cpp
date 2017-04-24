#include <sstream>
#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures,
           GLfloat shininess) :
        vertices(vertices), indices(indices), textures(textures), shininess(shininess) {
    this->setupMesh();
}

void Mesh::Draw() {
    glUniform1f(shader->GetUniformLocation("material.shininess"), shininess);
    glUniform1ui(shader->GetUniformLocation("UseNormalMap"), 0);
    for (GLuint i = 0; i < this->textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string name = this->textures[i].type;
        if (name == "texture_diffuse") {
            glUniform1i(shader->GetUniformLocation("material.diffuse"), i);
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        } else if (name == "texture_specular") {
            glUniform1i(shader->GetUniformLocation("material.specular"), i);
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        } else if (name == "texture_normal") {
            glUniform1ui(shader->GetUniformLocation("UseNormalMap"), 1);
            glUniform1i(shader->GetUniformLocation("material.normal"), i);
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        } else if (name == "texture_height") {
            glUniform1i(shader->GetUniformLocation("material.height"), i);
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        }
    }
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, this->indices.data());
    glBindVertexArray(0);
    for (GLuint i = 0; i < this->textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::Load(std::shared_ptr<ShaderProgram> shader_program) {
    shader = shader_program;
    GLuint pos;
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    pos = shader->GetAttribLocation("position");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) 0);
    pos = shader->GetAttribLocation("normal");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, Normal));
    pos = shader->GetAttribLocation("texCoords");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, TexCoords));
    pos = shader->GetAttribLocation("tangent");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, Tangent));
    pos = shader->GetAttribLocation("bitangent");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, Bitangent));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
