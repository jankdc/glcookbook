#pragma once

#ifndef GLC_MODEL_HPP
#define GLC_MODEL_HPP

#include "shader.hpp"

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <unordered_map>

namespace glc {
    struct vtx
    {
        glm::vec3 pos;
        glm::vec3 norm;
        glm::vec2 uv;
    };

    enum class textype
    {
        SPECULAR,
        DIFFUSE
    };

    struct tex
    {
        GLuint id;
        textype type;
    };

    struct mesh
    {
        std::vector<glc::vtx> vertices;
        std::vector<glc::tex> textures;
        std::vector<GLuint> indices;
    };

    class Model
    {
    public:
        explicit Model(std::string path);
        void draw(glc::Shader shader);
    private:
        std::vector<glc::mesh> mMeshes;
        std::unordered_map<std::string, glc::tex> mLoadedTextures;
        std::string mBaseDirectory;

        // Helper Methods
        void processNode(const aiScene* scene, const aiNode* node);
        void processMesh(const aiScene* scene, const aiMesh* mesh);
        std::vector<glc::tex> getTex(const aiMaterial* mat, const aiTextureType type);
    };
}

#endif