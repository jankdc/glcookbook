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
const auto WINDOW_TITLE  = "GL Cook Book - Playing with Color.";
const auto RATIO = static_cast<float>(WINDOW_WIDTH/WINDOW_HEIGHT);
const auto VERTICES = std::vector<GLfloat> {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

    auto vshader = glc::makeVShader("res/color/model_v.glsl");
    glc::printShaderStatus(vshader);

    auto fshader = glc::makeFShader("res/color/model_f.glsl");
    glc::printShaderStatus(fshader);

    auto program = glc::makeProgram({vshader, fshader});

    auto camera = glc::Camera(window);
    camera.setPosition(glm::vec3(0.0f, 0.0f, 2.0f));

    auto vao = glc::makeMesh(VERTICES);
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

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);

        auto modelId = glGetUniformLocation(program, "model");
        auto viewId = glGetUniformLocation(program, "view");
        auto projectionId = glGetUniformLocation(program, "projection");

        auto view = camera.generateMat();
        auto projection = glm::perspective(45.0f, RATIO, 0.1f, 1000.0f);
        auto model = glm::mat4(1.0f);
        auto modelRotationAngle = glm::radians(currentFrame * -55.0f);
        auto modelRotationAxis = glm::vec3(1.0f, 0.3f, 0.5f);
        model = glm::translate(model, glm::vec3(0.0f));
        model = glm::rotate(model, modelRotationAngle, modelRotationAxis);

        glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, VERTICES.size());
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glDeleteShader(vshader);
    glDeleteShader(fshader);
    glDeleteProgram(program);
    glfwTerminate();

    return 0;
}

