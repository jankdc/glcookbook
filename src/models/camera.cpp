#include "camera.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

glc::Camera::Camera(GLFWwindow* window)
: mWindow(window),
  mPitch(0.0f),
  mYaw(-90.0f),
  mSensitivity(5.0f),
  mSpeed(3.0f),
  mPosition(0.0f),
  mDirection(0.0f, 0.0f, -1.0f),
  mUp(0.0f, 1.0f, 0.0f),
  mWorldUp(0.0f, 1.0f, 0.0f),
  mRight(glm::normalize(glm::cross(mWorldUp, mDirection))),
  mLastX(0.0),
  mLastY(0.0),
  mMoved(false)
{

}

void glc::Camera::update(float delta)
{
    updateMovement(delta);
    updateLook(delta);
}

void glc::Camera::updateMovement(float delta)
{
    auto deltaSpeed = mSpeed * delta;

    if(glfwGetKey(mWindow, GLFW_KEY_W))
        mPosition += glm::normalize(mDirection) * deltaSpeed;
    if(glfwGetKey(mWindow, GLFW_KEY_S))
        mPosition -= glm::normalize(mDirection) * deltaSpeed;
    if(glfwGetKey(mWindow, GLFW_KEY_A))
        mPosition -= glm::normalize(mRight) * deltaSpeed;
    if(glfwGetKey(mWindow, GLFW_KEY_D))
        mPosition += glm::normalize(mRight) * deltaSpeed;
}

void glc::Camera::updateLook(float delta)
{
    auto currentCursorX = 0.0;
    auto currentCursorY = 0.0;

    glfwGetCursorPos(mWindow, &currentCursorX, &currentCursorY);

    // glfwGetCursorPos return incorrect value when cursor hasn't moved
    // for the first time. Prevents camera jolting when you move the cursor
    // the first time.
    if (not mMoved and currentCursorX != 0.0 and currentCursorY != 0.0)
    {
        mLastX = currentCursorX;
        mLastY = currentCursorY;

        mMoved = true;
    }

    auto xoffset = currentCursorX - mLastX;
    auto yoffset = mLastY - currentCursorY;

    mLastX = currentCursorX;
    mLastY = currentCursorY;

    mYaw += mSensitivity * delta * xoffset;
    mPitch += mSensitivity * delta * yoffset;

    if (mPitch > 89.0f)
        mPitch = 89.0f;
    if (mPitch < -89.0f)
        mPitch = -89.0f;

    auto newDir = glm::vec3(0.0f);
    newDir.x = cosf(glm::radians(mYaw)) * cosf(glm::radians(mPitch));
    newDir.y = sinf(glm::radians(mPitch));
    newDir.z = sinf(glm::radians(mYaw)) * cosf(glm::radians(mPitch));

    mDirection = glm::normalize(newDir);
    mRight = glm::normalize(glm::cross(mDirection, mWorldUp));
    mUp = glm::normalize(glm::cross(mDirection, mRight));
}

void glc::Camera::setPosition(glm::vec3 position)
{
    mPosition = position;
}

void glc::Camera::setDirection(glm::vec3 direction)
{
    mDirection = direction;
}

void glc::Camera::setSensitivity(float sensitivity)
{
    mSensitivity = sensitivity;
}

void glc::Camera::setMovementSpeed(float speed)
{
    mSpeed = speed;
}

glm::vec3 glc::Camera::getPosition() const
{
    return mPosition;
}

glm::vec3 glc::Camera::getDirection() const
{
    return mDirection;
}

glm::mat4 glc::Camera::generateMat() const
{
    return glm::lookAt(mPosition, mPosition + mDirection, mWorldUp);
}
