#pragma once

#ifndef GLC_SCENE_H
#define GLC_SCENE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include <string>

struct GLFWwindow;

namespace glc {

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
        BIO
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

}

#endif