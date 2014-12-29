#pragma once

#ifndef GLC_SCENE_H
#define GLC_SCENE_H


#include "common.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

struct GLFWwindow;

namespace glc {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texcoord;
    };

    struct Texture {
        GLuint id;
        std::string type;
        std::string path;
    };

    class ExMesh {
    public:
        ExMesh(std::vector<Vertex> vertices,
             std::vector<GLuint> indices,
             std::vector<Texture> textures)
        : m_vertices(vertices),
          m_indices(indices),
          m_textures(textures)
        {
        }

        void setup()
        {
            glGenVertexArrays(1, &m_vao);
            glGenBuffers(1, &m_vbo);
            glGenBuffers(1, &m_ebo);

            glBindVertexArray(m_vao);

            auto vbytesize = m_vertices.size() * sizeof(Vertex);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, vbytesize, m_vertices.data(), GL_STATIC_DRAW);

            auto ibytesize = m_indices.size() * sizeof(GLuint);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibytesize, m_indices.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                (GLvoid*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                (GLvoid*)offsetof(Vertex, normal));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                (GLvoid*)offsetof(Vertex, texcoord));

            glBindVertexArray(0);
        }

        void draw(GLuint shader)
        {
            GLuint diff = 1;
            GLuint spec = 1;

            for (GLuint i = 0; i < m_textures.size(); i++) {
                auto tex = m_textures[i];
                auto str = std::stringstream();
                auto name = m_textures[i].type;

                if (name == "texture_diffuse") {
                    str << diff++;
                }

                if (name == "texture_specular") {
                    str << spec++;
                }

                auto num = str.str();
                auto matId = glGetUniformLocation(shader, ("Material."+name+num).c_str());

                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
                glUniform1f(matId, i);
            }
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(m_vao);
            glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    private:
        std::vector<Vertex> m_vertices;
        std::vector<GLuint> m_indices;
        std::vector<Texture> m_textures;
        GLuint m_vao, m_vbo, m_ebo;
    };

    class Model {
    public:
        void draw(GLuint shader) {
            for (auto& m : m_meshes) {
                m.draw(shader);
            }
        }

        void load(std::string path)
        {
            Assimp::Importer import;
            auto scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

            if (! scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || ! scene->mRootNode) {
                std::cout << "[Assimp Error]: " << import.GetErrorString() << '\n';
                return;
            }

            m_directory = path.substr(0, path.find_last_of('/'));

            processNode(scene->mRootNode, scene);
        }

        void processNode(aiNode* node, const aiScene* scene)
        {
            for (GLuint i = 0; i < node->mNumMeshes; i++) {
                auto mesh = scene->mMeshes[node->mMeshes[i]];
                auto customMesh = processMesh(mesh, scene);
                customMesh.setup();
                m_meshes.emplace_back(customMesh);
            }

            for (GLuint i = 0; i < node->mNumChildren; i++) {
                processNode(node->mChildren[i], scene);
            }
        }

        ExMesh processMesh(aiMesh* mesh, const aiScene* scene)
        {
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;
            std::vector<Texture> textures;

            for (GLuint i = 0; i < mesh->mNumVertices; i++) {
                Vertex vertex;

                vertex.position = glm::vec3(
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z);

                vertex.normal = glm::vec3(
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z);

                if (mesh->mTextureCoords[0]) {
                    vertex.texcoord = glm::vec2(
                        mesh->mTextureCoords[0][i].x,
                        mesh->mTextureCoords[0][i].y);
                }
                else {
                    vertex.texcoord = glm::vec2(0.0f, 0.0f);
                }

                vertices.emplace_back(vertex);
            }

            for (GLuint i = 0; i < mesh->mNumFaces; ++i) {
                auto face = mesh->mFaces[i];
                for (GLuint j = 0; j < face.mNumIndices; j++) {
                    indices.emplace_back(face.mIndices[j]);
                }
            }

            auto mat = scene->mMaterials[mesh->mMaterialIndex];
            auto diffMaps = loadMatTex(mat, aiTextureType_DIFFUSE, "texture_diffuse");
            auto specMaps = loadMatTex(mat, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), diffMaps.begin(), diffMaps.end());
            textures.insert(textures.end(), specMaps.begin(), specMaps.end());

            return {vertices, indices, textures};
        }

        std::vector<Texture> loadMatTex(aiMaterial* mat, aiTextureType type, std::string typeName) {
            std::vector<Texture> textures;

            for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
                aiString str;
                mat->GetTexture(type, i, &str);

                bool skip = false;
                for (auto loadedTex : m_loadedTextures) {
                    if (loadedTex.path == str.C_Str()) {
                        skip = true;
                        textures.emplace_back(loadedTex);
                        break;
                    }
                }

                if (skip) {
                    continue;
                }

                Texture tex;
                tex.id = glc::makeTexture(m_directory + '/' + str.C_Str());
                tex.type = typeName;
                tex.path = str.C_Str();
                textures.emplace_back(tex);
                m_loadedTextures.emplace_back(tex);
            }

            return textures;
        }
    private:
        std::vector<ExMesh> m_meshes;
        std::vector<Texture> m_loadedTextures;
        std::string m_directory;
    };

    struct Mesh {
        GLuint id;
        size_t size;
    };

    struct Light {
        glm::vec3 pos;
        glm::vec3 ka;
        glm::vec3 kd;
        glm::vec3 ks;
    };

    struct Material {
        glm::vec3 ka;
        glm::vec3 kd;
        glm::vec3 ks;
        float sh;
    };

    struct Cube {
        glc::Material material;
        glm::vec3 position;
    };

    enum class SceneType {
        BASIC,
        BIO,
        NANO
    };

    class BasicScene {
    public:
        BasicScene(GLFWwindow* window,
            std::unordered_map<std::string, glc::Mesh> meshes,
            std::unordered_map<std::string, GLuint> shaders,
            std::unordered_map<std::string, GLuint> textures);
        void setup();
        void update(float diftime);
        void draw();
    private:
        GLFWwindow* m_window;
        glc::Camera m_camera;
        std::unordered_map<std::string, glc::Mesh> m_meshes;
        std::unordered_map<std::string, GLuint> m_shaders;
        std::unordered_map<std::string, GLuint> m_textures;
        glc::Light m_light;
        glm::mat4 m_projection;
    };

    class BioScene {
    public:
        BioScene(GLFWwindow* window,
            std::unordered_map<std::string, glc::Mesh> meshes,
            std::unordered_map<std::string, GLuint> shaders,
            std::unordered_map<std::string, GLuint> textures);
        void setup();
        void update(float diftime);
        void draw();
    private:
        GLFWwindow* m_window;
        glc::Camera m_camera;
        std::unordered_map<std::string, glc::Mesh> m_meshes;
        std::unordered_map<std::string, GLuint> m_shaders;
        std::unordered_map<std::string, GLuint> m_textures;
        glc::Light m_light;
        glm::mat4 m_projection;
    };

    class NanoScene {
    public:
        NanoScene(GLFWwindow* window,
            std::unordered_map<std::string, glc::Mesh> meshes,
            std::unordered_map<std::string, GLuint> shaders,
            std::unordered_map<std::string, GLuint> textures);
        void setup();
        void update(float diftime);
        void draw();
    private:
        GLFWwindow* m_window;
        glc::Camera m_camera;
        std::unordered_map<std::string, glc::Mesh> m_meshes;
        std::unordered_map<std::string, GLuint> m_shaders;
        std::unordered_map<std::string, GLuint> m_textures;
        Model m_nanosuit;
        glc::Light m_light;
        glm::mat4 m_projection;
    };

}

#endif