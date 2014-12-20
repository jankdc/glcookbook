#include "camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glc::Camera::Camera(GLFWwindow* window)
{
    m_window = window;
    m_pitch = 0.0f;
    m_yaw = -90.0f;
    m_sensitivity = 5.0f;
    m_speed = 3.0f;
    m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
    m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    m_right = glm::normalize(glm::cross(m_worldUp, m_direction));
    m_moved = GL_FALSE;
}

void glc::Camera::update(float delta)
{
    updateMovement(delta);
    updateLook(delta);
}

void glc::Camera::updateMovement(float delta)
{
    auto deltaSpeed = m_speed * delta;

    if(glfwGetKey(m_window, GLFW_KEY_W))
        m_position += glm::normalize(m_direction) * deltaSpeed;
    if(glfwGetKey(m_window, GLFW_KEY_S))
        m_position -= glm::normalize(m_direction) * deltaSpeed;
    if(glfwGetKey(m_window, GLFW_KEY_A))
        m_position -= glm::normalize(m_right) * deltaSpeed;
    if(glfwGetKey(m_window, GLFW_KEY_D))
        m_position += glm::normalize(m_right) * deltaSpeed;
}

void glc::Camera::updateLook(float delta)
{
    auto currentCursorX = 0.0;
    auto currentCursorY = 0.0;

    glfwGetCursorPos(m_window, &currentCursorX, &currentCursorY);

    // glfwGetCursorPos return incorrect value when cursor hasn't moved
    // for the first time. Prevents camera jolting when you move the cursor
    // the first time.
    if (not m_moved and currentCursorX != 0.0 and currentCursorY != 0.0)
    {
        m_lastX = currentCursorX;
        m_lastY = currentCursorY;

        m_moved = true;
    }

    auto xoffset = currentCursorX - m_lastX;
    auto yoffset = m_lastY - currentCursorY;

    m_lastX = currentCursorX;
    m_lastY = currentCursorY;

    m_yaw += m_sensitivity * delta * xoffset;
    m_pitch += m_sensitivity * delta * yoffset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    auto newDir = glm::vec3(0.0f);
    newDir.x = cosf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch));
    newDir.y = sinf(glm::radians(m_pitch));
    newDir.z = sinf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch));

    m_direction = glm::normalize(newDir);
    m_right = glm::normalize(glm::cross(m_direction, m_worldUp));
    m_up = glm::normalize(glm::cross(m_direction, m_right));
}

void glc::Camera::setPosition(glm::vec3 position)
{
    m_position = position;
}

void glc::Camera::setDirection(glm::vec3 direction)
{
    m_direction = direction;
}

void glc::Camera::setSensitivity(float sensitivity)
{
    m_sensitivity = sensitivity;
}

void glc::Camera::setMovementSpeed(float speed)
{
    m_speed = speed;
}

glm::mat4 glc::Camera::generateMat() const
{
    return glm::lookAt(m_position, m_position + m_direction, m_worldUp);
}