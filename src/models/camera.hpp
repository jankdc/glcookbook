#pragma once

#ifndef GLC_CAMERA_HPP
#define GLC_CAMERA_HPP

#include <glm/glm.hpp>

struct GLFWwindow;

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
        GLFWwindow* mWindow;
        float mPitch;
        float mYaw;
        float mSensitivity;
        float mSpeed;
        glm::vec3 mPosition;
        glm::vec3 mDirection;
        glm::vec3 mUp;
        glm::vec3 mWorldUp;
        glm::vec3 mRight;
        double mLastX;
        double mLastY;
        bool mMoved;
    };

}

#endif
