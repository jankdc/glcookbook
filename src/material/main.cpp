#include "camera.h"
#include "common.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

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
    Material  material;
    glm::vec3 position;
};


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
    /*
     _____ _   _ _____ _______
    |_   _| \ | |_   _|__   __|
      | | |  \| | | |    | |
      | | | . ` | | |    | |
     _| |_| |\  |_| |_   | |
    |_____|_| \_|_____|  |_|

    */


    glfwSetErrorCallback(glc::printErr);
    glfwInit();


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
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

    /*
     _____  ______  _____  ____  _    _ _____   _____ ______  _____
    |  __ \|  ____|/ ____|/ __ \| |  | |  __ \ / ____|  ____|/ ____|
    | |__) | |__  | (___ | |  | | |  | | |__) | |    | |__  | (___
    |  _  /|  __|  \___ \| |  | | |  | |  _  /| |    |  __|  \___ \
    | | \ \| |____ ____) | |__| | |__| | | \ \| |____| |____ ____) |
    |_|  \_\______|_____/ \____/ \____/|_|  \_\\_____|______|_____/

    */


    // CUBE MESH VAO
    auto cubeMeshId = glc::makeMesh(VERTICES);


    // CUBE SHADER
    auto objectVShader = glc::makeVShader("res/material/object_v.glsl");
    auto objectFShader = glc::makeFShader("res/material/object_f.glsl");
    auto objectShader = glc::makeProgram({objectVShader, objectFShader});
    glc::printShaderStatus(objectVShader);
    glc::printShaderStatus(objectFShader);
    glDeleteShader(objectFShader);
    glDeleteShader(objectVShader);


    // LIGHT SOURCE SHADER
    auto lampVShader = glc::makeVShader("res/material/lamp_v.glsl");
    auto lampFShader = glc::makeFShader("res/material/lamp_f.glsl");
    auto lampShader = glc::makeProgram({lampVShader, lampFShader});
    glc::printShaderStatus(lampVShader);
    glc::printShaderStatus(lampFShader);
    glDeleteShader(lampVShader);
    glDeleteShader(lampFShader);


    // CAMERA OBJECT
    auto camera = glc::Camera(window);
    camera.setPosition(glm::vec3(0.5f, 0.0f, 5.0f));


    // LIGHT SOURCE
    auto light = Light();
    light.ka  = glm::vec3(1.0f);
    light.kd  = glm::vec3(1.0f);
    light.ks  = glm::vec3(1.0f);
    light.pos = glm::vec3(1.2f, 1.0f, 2.0f);


    /*
     __  __          _____ _   _   _      ____   ____  _____
    |  \/  |   /\   |_   _| \ | | | |    / __ \ / __ \|  __ \
    | \  / |  /  \    | | |  \| | | |   | |  | | |  | | |__) |
    | |\/| | / /\ \   | | | . ` | | |   | |  | | |  | |  ___/
    | |  | |/ ____ \ _| |_| |\  | | |___| |__| | |__| | |
    |_|  |_/_/    \_\_____|_| \_| |______\____/ \____/|_|

    */


    // TIME
    auto oldTime = static_cast<float>(glfwGetTime());
    auto newTime = static_cast<float>(glfwGetTime());
    auto delta = 0.0f;

    while (! glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        newTime = static_cast<float>(glfwGetTime());
        delta   = newTime-oldTime;
        oldTime = newTime;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        camera.update(delta);

        light.pos.x = 1.0f + sinf(newTime) * 2.0f;
        light.pos.y = sinf(newTime / 2.0f) * 1.0f;

        auto view = camera.generateMat();
        auto projection = glm::perspective(45.0f, RATIO, 0.1f, 1000.0f);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(objectShader);

        {
            auto matKaId = glGetUniformLocation(objectShader, "Material.ka");
            auto matKdId = glGetUniformLocation(objectShader, "Material.kd");
            auto matKsId = glGetUniformLocation(objectShader, "Material.ks");
            auto matShineId = glGetUniformLocation(objectShader, "Material.a");
            auto lightKaId = glGetUniformLocation(objectShader, "Light.ka");
            auto lightKdId = glGetUniformLocation(objectShader, "Light.kd");
            auto lightKsId = glGetUniformLocation(objectShader, "Light.ks");
            auto lightPosId = glGetUniformLocation(objectShader, "Light.position");
            auto modelId = glGetUniformLocation(objectShader, "Model");
            auto viewId  = glGetUniformLocation(objectShader, "View");
            auto projectionId = glGetUniformLocation(objectShader, "Projection");
            auto normalMatId  = glGetUniformLocation(objectShader, "Normal");
            auto cameraPosId  = glGetUniformLocation(objectShader, "CameraPosition");
            auto cameraPos = camera.getPosition();
            glUniform3f(lightPosId, light.pos.x, light.pos.y, light.pos.z);
            glUniform3f(lightKaId, light.ka.r, light.ka.g, light.ka.b);
            glUniform3f(lightKdId, light.kd.r, light.kd.g, light.kd.b);
            glUniform3f(lightKsId, 1.0f, 1.0f, 1.0f);


            auto model = glm::mat4(1.0f);
            auto modelRotationAngle = glm::radians(newTime * -55.0f);
            auto modelRotationAxis = glm::vec3(1.0f, 0.3f, 0.5f);
            model = glm::translate(model, glm::vec3(0.0f));
            model = glm::rotate(model, modelRotationAngle, modelRotationAxis);
            auto normalMat = glm::mat3(glm::transpose(glm::inverse(model)));


            glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix3fv(normalMatId, 1, GL_FALSE, glm::value_ptr(normalMat));
            glUniform3f(cameraPosId, cameraPos.x, cameraPos.y, cameraPos.z);


            glUniform3f(matKaId, 0.0f, 0.1f, 0.06f);
            glUniform3f(matKdId, 0.0f, 0.50980392f, 0.50980392f);
            glUniform3f(matKsId, 0.50196078f, 0.50196078f, 0.50196078f);
            glUniform1f(matShineId, 64.0f);


            glBindVertexArray(cubeMeshId);
            glDrawArrays(GL_TRIANGLES, 0, VERTICES.size());
            glBindVertexArray(0);
        }

        glUseProgram(lampShader);

        {
            auto modelId = glGetUniformLocation(lampShader, "Model");
            auto viewId = glGetUniformLocation(lampShader, "View");
            auto projectionId = glGetUniformLocation(lampShader, "Projection");

            auto model = glm::mat4(1.0f);
            model = glm::translate(model, light.pos);
            model = glm::scale(model, glm::vec3(0.2f));

            glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(cubeMeshId);
            glDrawArrays(GL_TRIANGLES, 0, VERTICES.size());
            glBindVertexArray(0);
        }

        glUseProgram(0);

        glfwSwapBuffers(window);
    }


    // CLEANUP
    glDeleteProgram(objectShader);
    glDeleteProgram(lampShader);
    glfwTerminate();


    return 0;
}

