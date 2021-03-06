#ifndef TRIANGLE_LOADED_MODEL_HPP
#define TRIANGLE_LOADED_MODEL_HPP

#include "mesh.hpp"
#include "model.hpp"


class LoadedModel : public Model {
public:
    LoadedModel(GLchar *path, bool use_textures = true) {
        this->loadModel(path);
        this->use_textures = use_textures;
    }

    ~LoadedModel();

    void Load(std::shared_ptr<ShaderProgram> shader_program);

    void Draw() override;

private:
    bool use_textures;
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                              std::string typeName);
};

#endif //TRIANGLE_LOADED_MODEL_HPP