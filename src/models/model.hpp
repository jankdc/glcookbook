#pragma once

#ifndef GLC_MODEL_HPP
#define GLC_MODEL_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>

#include <vector>
#include <string>
#include <unordered_map>

namespace glc {
    class Shader;

    struct Vex
    {
        glm::vec3 pos;
        glm::vec3 norm;
        glm::vec2 uv;
    };

    enum class TexType
    {
        SPEC,
        DIFF
    };

    struct Tex
    {
        GLuint id;
        TexType type;
    };

    class Mesh
    {
    public:
        explicit
        Mesh(const std::vector<glc::Vex>& vertices,
             const std::vector<glc::Tex>& textures,
             const std::vector<GLuint>& indices);

        void draw(glc::Shader* shader);
    private:
        std::vector<glc::Vex> mVertices;
        std::vector<glc::Tex> mTextures;
        std::vector<GLuint> mIndices;
        GLuint mVao, mVbo, mEbo;
    };

    class Model
    {
    public:
        explicit Model(std::string path);
        void draw(glc::Shader* shader);
    private:
        std::vector<glc::Mesh> mMeshes;
        std::unordered_map<std::string, glc::Tex> mLoadedTextures;
        std::string mBaseDirectory;

        // Helper Methods
        void processNode(const aiScene* scene, const aiNode* node);
        void processMesh(const aiScene* scene, const aiMesh* mesh);
        std::vector<glc::Tex> getTex(const aiMaterial* mat, const aiTextureType type);
    };
}

#endif
