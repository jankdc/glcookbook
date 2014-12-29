#pragma once

#ifndef GLC_SCENE_HPP
#define GLC_SCENE_HPP

#include "camera.hpp"
#include "shader.hpp"
#include "model.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace glc {

    struct Light
    {
        glm::vec3 pos;
        glm::vec3 ka;
        glm::vec3 kd;
        glm::vec3 ks;
    };

    struct Material
    {
        glm::vec3 ka;
        glm::vec3 kd;
        glm::vec3 ks;
        GLfloat sh;
    };

    struct Cube
    {
        glc::Material material;
        glm::vec3 position;
    };

    class Scene
    {
    public:
        Scene(GLFWwindow* window);
        void setup();
        void update(float diftime);
        void draw();
    private:
        GLFWwindow* mWindow;
        glc::Camera mCamera;
        glc::Shader mPhong;
        glc::Light  mLight;
        glc::Model  mNanoSuit;
    };

}

#endif
