//
// Created by abdoe on 15/06/2025.
//

#ifndef MODEL_H
#define MODEL_H
#include "learn_opengl_utils/mesh.h"
#include "learn_opengl_utils/shader.h"
#include "third_party/stb_image/stb_image.h"

#include "third_party/assimp/include/assimp/Importer.hpp"
#include "third_party/assimp/include/assimp/postprocess.h"
#include "third_party/assimp/include/assimp/scene.h"
#include <unordered_map>

using namespace std;

class Model {
public:
  Model &operator=(const Model &) = delete;
  Model(const Model &) = delete;

  explicit Model(const string &path) { LoadModel(path); }

  void Draw(Shader &shader) const {
    for (auto &mesh : meshes_)
      mesh->Draw(shader);
  }

private:
  void LoadModel(const string &path) {
    Assimp::Importer importer;
    string local_path = LocalPaths::getLocalPath(path);
    const aiScene *scene = importer.ReadFile(local_path, aiProcess_Triangulate |
                                                             aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
      throw runtime_error("ERROR::ASSIMP::" +
                          string(importer.GetErrorString()));
    }

    directory_ = local_path.substr(0, local_path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
  }

  void ProcessNode(aiNode *node, const aiScene *scene) {
    // process all the nodes meshes
    for (auto i = 0; i < node->mNumMeshes; i++) {
      auto meshIdx = node->mMeshes[i];
      aiMesh *mesh = scene->mMeshes[meshIdx];
      meshes_.push_back(ProcessMesh(mesh, scene));
    }

    // repeat for all children
    for (auto i = 0; i < node->mNumChildren; i++)
      ProcessNode(node->mChildren[i], scene);
  }

  Mesh *ProcessMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<uint32_t> indices;
    vector<Texture> textures;

    for (auto i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex{};
      // process vertex positions
      vertex.position = FromAiVector3DToGlmV3(mesh->mVertices[i]);
      vertex.normal = FromAiVector3DToGlmV3(mesh->mNormals[i]);

      vertex.tex_coords = glm::vec2(0.0f, 0.0f);
      if (mesh->mTextureCoords[0])
        vertex.tex_coords = FromAiVector3DToGlmV2(mesh->mTextureCoords[0][i]);

      vertices.push_back(vertex);
    }
    // process indices
    for (auto i = 0; i < mesh->mNumFaces; i++) {
      const aiFace &face = mesh->mFaces[i];
      for (auto j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }

    // process material
    if (mesh->mMaterialIndex >= 0) {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
      vector<Texture> diffuse_maps = LoadMaterialTextures(
          material, aiTextureType_DIFFUSE, "texture_diffuse");
      textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

      vector<Texture> specular_maps = LoadMaterialTextures(
          material, aiTextureType_SPECULAR, "texture_specular");
      textures.insert(textures.end(), specular_maps.begin(),
                      specular_maps.end());
    }

    // TODO: probably we should use unique ptrs ?
    return new Mesh(vertices, indices, textures);
  }

  static glm::vec3 FromAiVector3DToGlmV3(const aiVector3D &vector) {
    return {vector.x, vector.y, vector.z};
  }

  static glm::vec2 FromAiVector3DToGlmV2(const aiVector3D &vector) {
    return {vector.x, vector.y};
  }

  vector<Texture> LoadMaterialTextures(const aiMaterial *mat,
                                       const aiTextureType type,
                                       const string &type_name) {
    vector<Texture> textures;
    for (auto i = 0; i < mat->GetTextureCount(type); i++) {
      aiString tex_file_name;
      mat->GetTexture(type, i, &tex_file_name);

      // check if the texture is already loaded
      if (textures_loaded.find(tex_file_name.C_Str()) !=
          textures_loaded.end()) {
        textures.push_back(textures_loaded[tex_file_name.C_Str()]);
      } else {
        Texture texture;
        texture.id = TextureFromFile(tex_file_name.C_Str(), directory_);
        texture.type = type_name;
        texture.path = tex_file_name.C_Str();
        textures.push_back(texture);
        textures_loaded.insert({tex_file_name.C_Str(), texture});
      }
    }

    return textures;
  }

  static uint32_t TextureFromFile(const char *name, const string &dir) {
    unsigned int texture_id;
    string path = dir + "/" + string(name);
    glGenTextures(1, &texture_id);

    int width, height, nr_components;
    unsigned char *data = stbi_load(LocalPaths::getLocalPath(path).c_str(),
                                    &width, &height, &nr_components, 0);
    if (data) {
      GLenum format = GL_RED;
      if (nr_components == 1)
        format = GL_RED;
      else if (nr_components == 3)
        format = GL_RGB;
      else if (nr_components == 4)
        format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, texture_id);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
    } else {
      const std::string err_message =
          "Texture failed to load at path " + path + "\n";
      stbi_image_free(data);
      throw std::runtime_error(err_message);
    }

    return texture_id;
  }

public:
  // model data
  unordered_map<string, Texture>
      textures_loaded; // store currently loaded textures to avoid
                       // reloading them
  vector<Mesh *> meshes_;
  string directory_;
};

#endif // MODEL_H
