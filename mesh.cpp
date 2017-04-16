#include <sstream>
#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) :
        vertices(vertices), indices(indices), textures(textures) {
    this->setupMesh();
}

void Mesh::Draw(ShaderProgram &shader) {
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    GLuint normalNr = 1;
//    GLuint heightNr = 1;
//    for(GLuint i = 0; i < this->textures.size(); i++) {
//        glActiveTexture(GL_TEXTURE0 + i);
//        std::stringstream ss;
//        std::string number;
//        std::string name = this->textures[i].type;
//        if(name == "texture_diffuse")
//            ss << diffuseNr++;
//        else if(name == "texture_specular")
//            ss << specularNr++;
//        else if(name == "texture_normal")
//            ss << normalNr++;
////        else if(name == "texture_height")
////            ss << heightNr++;
//        number = ss.str();
//        glUniform1i(shader.GetUniformLocation((name + number).c_str()), i);
//        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
//    }
    glActiveTexture(GL_TEXTURE0 + 0);
    glUniform1i(shader.GetUniformLocation("material.diffuse"), 0);
    glBindTexture(GL_TEXTURE_2D, this->textures[0].id);
    glActiveTexture(GL_TEXTURE0 + 1);
    glUniform1i(shader.GetUniformLocation("material.specular"), 1);
    glBindTexture(GL_TEXTURE_2D, this->textures[1].id);
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    for (GLuint i = 0; i < this->textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, TexCoords));
//    glEnableVertexAttribArray(3);
//    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
//    glEnableVertexAttribArray(4);
//    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Bitangent));
    glBindVertexArray(0);
}
