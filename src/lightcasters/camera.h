#pragma once

#ifndef GLC_CAMERA_H
#define GLC_CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace glc {

    class Camera {
    public:
        Camera(GLFWwindow* window);

        void update(float delta);
        void updateMovement(float delta);
        void updateLook(float delta);
        void setPosition(glm::vec3 position);
        void setDirection(glm::vec3 direction);
        void setMovementSpeed(float speed);
        void setSensitivity(float sensitivity);
        glm::vec3 getPosition() const;
        glm::vec3 getDirection() const;
        glm::mat4 generateMat() const;
    private:
        GLFWwindow* m_window;
        GLfloat m_pitch;
        GLfloat m_yaw;
        GLfloat m_sensitivity;
        GLfloat m_speed;
        glm::vec3 m_position;
        glm::vec3 m_direction;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;
        GLfloat m_lastX;
        GLfloat m_lastY;
        GLboolean m_moved;
    };

}

#endif