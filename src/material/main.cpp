#include "camera.h"
#include "common.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

const auto WINDOW_WIDTH  = 800;
const auto WINDOW_HEIGHT = 600;
const auto WINDOW_TITLE  = "GL Cook Book - Playing with Material.";
const auto RATIO = static_cast<float>(WINDOW_WIDTH/WINDOW_HEIGHT);
const auto VERTICES = std::vector<GLfloat> {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


int main(int argc, char const *argv[])
{
    glfwSetErrorCallback(glc::printErr);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // GLEW has a bug for OS X where it still calls some legacy functions.
    // We need this for now to prevent GLFW from crashing when legacy
    // functions are called.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto window = glfwCreateWindow(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        WINDOW_TITLE,
        nullptr,
        nullptr);

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    // OS X has a bug that does not normalise the viewport coordinates
    // at all so only uncomment this when it's really needed since GLFW
    // have a normalized default anyway.
    // glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    auto objectVShader = glc::makeVShader("res/material/object_v.glsl");
    auto objectFShader = glc::makeFShader("res/material/object_f.glsl");
    auto objectShader = glc::makeProgram({objectVShader, objectFShader});
    auto objectVao = glc::makeMesh(VERTICES);
    glc::printShaderStatus(objectVShader);
    glc::printShaderStatus(objectFShader);

    auto lampVShader = glc::makeVShader("res/material/lamp_v.glsl");
    auto lampFShader = glc::makeFShader("res/material/lamp_f.glsl");
    auto lampShader = glc::makeProgram({lampVShader, lampFShader});
    auto lampVao = glc::makeMesh(VERTICES);
    glc::printShaderStatus(lampVShader);
    glc::printShaderStatus(lampFShader);

    auto camera = glc::Camera(window);
    camera.setPosition(glm::vec3(0.5f, 0.0f, 5.0f));

    auto lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

    auto lastFrame = static_cast<float>(glfwGetTime());
    auto deltaTime = 0.0f;

    while (! glfwWindowShouldClose(window))
    {
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, GL_TRUE);

        camera.update(deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = camera.generateMat();
        auto projection = glm::perspective(45.0f, RATIO, 0.1f, 1000.0f);

        lightPos.x = 1.0f + sinf(currentFrame) * 2.0f;
        lightPos.y = sinf(currentFrame / 2.0f) * 1.0f;

        glUseProgram(objectShader);

        {
            auto matAmbientId = glGetUniformLocation(objectShader, "material.ka");
            auto matDiffuseId = glGetUniformLocation(objectShader, "material.kd");
            auto matSpecularId = glGetUniformLocation(objectShader, "material.ks");
            auto matShineId = glGetUniformLocation(objectShader, "material.a");

            auto lightAmbientId = glGetUniformLocation(objectShader, "light.ka");
            auto lightDiffuseId = glGetUniformLocation(objectShader, "light.kd");
            auto lightSpecularId = glGetUniformLocation(objectShader, "light.ks");
            auto lightPosId = glGetUniformLocation(objectShader, "light.pos");

            auto modelNormalId = glGetUniformLocation(objectShader, "modelNormal");
            auto modelId = glGetUniformLocation(objectShader, "model");
            auto viewId = glGetUniformLocation(objectShader, "view");
            auto viewPosId = glGetUniformLocation(objectShader, "viewPos");
            auto projectionId = glGetUniformLocation(objectShader, "projection");

            auto model = glm::mat4(1.0f);
            auto modelRotationAngle = glm::radians(currentFrame * -55.0f);
            auto modelRotationAxis = glm::vec3(1.0f, 0.3f, 0.5f);
            auto modelNormal = glm::mat3(glm::transpose(glm::inverse(model)));
            model = glm::translate(model, glm::vec3(0.0f));
            model = glm::rotate(model, modelRotationAngle, modelRotationAxis);

            auto viewPos = camera.getPosition();

            auto lightColor = glm::vec3(1.0f);
            auto lightDiffuseColor = glm::vec3(1.0f);
            auto lightAmbientColor = glm::vec3(1.0f);

            glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix3fv(modelNormalId, 1, GL_FALSE, glm::value_ptr(modelNormal));
            glUniform3f(viewPosId, viewPos.x, viewPos.y, viewPos.z);

            // Material
            glUniform3f(matAmbientId, 0.0f, 0.1f, 0.06f);
            glUniform3f(matDiffuseId, 0.0f, 0.50980392f, 0.50980392f);
            glUniform3f(matSpecularId, 0.50196078f, 0.50196078f, 0.50196078f);
            glUniform1f(matShineId, 64.0f);

            // Light
            glUniform3f(lightPosId, lightPos.x, lightPos.y, lightPos.z);
            glUniform3f(lightAmbientId, lightAmbientColor.r, lightAmbientColor.g, lightAmbientColor.b);
            glUniform3f(lightDiffuseId, lightDiffuseColor.r, lightDiffuseColor.g, lightDiffuseColor.b);
            glUniform3f(lightSpecularId, 1.0f, 1.0f, 1.0f);

            glBindVertexArray(objectVao);
            glDrawArrays(GL_TRIANGLES, 0, VERTICES.size());
            glBindVertexArray(0);
        }

        glUseProgram(lampShader);

        {
            auto modelId = glGetUniformLocation(lampShader, "model");
            auto viewId = glGetUniformLocation(lampShader, "view");
            auto projectionId = glGetUniformLocation(lampShader, "projection");

            auto model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.2f));

            glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(lampVao);
            glDrawArrays(GL_TRIANGLES, 0, VERTICES.size());
            glBindVertexArray(0);
        }

        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glDeleteShader(objectVShader);
    glDeleteShader(objectFShader);
    glDeleteProgram(objectShader);

    glDeleteShader(lampVShader);
    glDeleteShader(lampFShader);
    glDeleteProgram(lampShader);

    glfwTerminate();

    return 0;
}

