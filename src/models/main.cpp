#include "camera.hpp"
#include "common.hpp"
#include "shader.hpp"
#include "scene.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <regex>


const auto WINDOW_WIDTH  = 800;
const auto WINDOW_HEIGHT = 600;
const auto WINDOW_TITLE  = "GL Cook Book - Playing with Models.";

const auto VERTICES = std::vector<GLfloat> {
    // Positions          // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f};


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


    // CUBE MESH VAO + TEXTURE
    auto cubeMeshId = glc::makeMesh(VERTICES);
    auto cubeMeshTex = glc::makeTexture("res/images/box.png");
    auto cubeMeshSpec = glc::makeTexture("res/images/box_specular.png");


    // auto phongShader = glc::Shader
    // {{
    //     "res/shaders/phong-vt.glsl",
    //     "res/shaders/phong-ft.glsl"
    // }};

    // auto basicShader = glc::Shader
    // {{
    //     "res/shaders/basic-vt.glsl",
    //     "res/shaders/basic-ft.glsl"
    // }};


    // CUBE SHADER
    auto cubeVShader = glc::makeVShader("res/models/object_v.glsl");
    auto cubeFShader = glc::makeFShader("res/models/object_f.glsl");
    auto cubeShader = glc::makeProgram({cubeVShader, cubeFShader});
    glc::printShaderStatus(cubeVShader);
    glc::printShaderStatus(cubeFShader);
    glDeleteShader(cubeFShader);
    glDeleteShader(cubeVShader);


    // LIGHT SOURCE SHADER
    auto lampVShader = glc::makeVShader("res/models/lamp_v.glsl");
    auto lampFShader = glc::makeFShader("res/models/lamp_f.glsl");
    auto lampShader = glc::makeProgram({lampVShader, lampFShader});
    glc::printShaderStatus(lampVShader);
    glc::printShaderStatus(lampFShader);
    glDeleteShader(lampVShader);
    glDeleteShader(lampFShader);


    auto meshes = std::unordered_map<std::string, glc::Mesh>
    {
        { "cube", {cubeMeshId, VERTICES.size()} }
    };


    auto shaders = std::unordered_map<std::string, GLuint>
    {
        { "cube", cubeShader },
        { "lamp", lampShader }
    };


    auto textures = std::unordered_map<std::string, GLuint>
    {
        { "box",         cubeMeshTex  },
        { "boxSpecular", cubeMeshSpec }
    };


    auto scene01 = glc::BasicScene(window, meshes, shaders, textures);
    scene01.setup();

    auto scene02 = glc::BioScene(window, meshes, shaders, textures);
    scene02.setup();

    auto scene03 = glc::NanoScene(window, meshes, shaders, textures);
    scene03.setup();

    auto currentScene = glc::SceneType::BASIC;

    /*
     __  __          _____ _   _   _      ____   ____  _____
    |  \/  |   /\   |_   _| \ | | | |    / __ \ / __ \|  __ \
    | \  / |  /  \    | | |  \| | | |   | |  | | |  | | |__) |
    | |\/| | / /\ \   | | | . ` | | |   | |  | | |  | |  ___/
    | |  | |/ ____ \ _| |_| |\  | | |___| |__| | |__| | |
    |_|  |_/_/    \_\_____|_| \_| |______\____/ \____/|_|

    */

    auto newtime = 0.0f;
    auto oldtime = 0.0f;
    auto diftime = 0.0f;

    while (! glfwWindowShouldClose(window))
    {
        newtime = static_cast<float>(glfwGetTime());
        diftime = glm::max(newtime - oldtime, 0.0f);
        oldtime = newtime;

        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (glfwGetKey(window, GLFW_KEY_B)) {
            currentScene = glc::SceneType::BASIC;
        }

        if (glfwGetKey(window, GLFW_KEY_O)) {
            currentScene = glc::SceneType::BIO;
        }

        if (glfwGetKey(window, GLFW_KEY_N)) {
            currentScene = glc::SceneType::NANO;
        }

        scene01.update(diftime);
        scene02.update(diftime);
        scene03.update(diftime);

        switch (currentScene) {
            case glc::SceneType::BASIC:
                scene01.draw();
                break;
            case glc::SceneType::BIO:
                scene02.draw();
                break;
            case glc::SceneType::NANO:
                scene03.draw();
                break;
        }

        glfwSwapBuffers(window);
    }


    // CLEANUP
    glDeleteProgram(cubeShader);
    glDeleteProgram(lampShader);
    glfwTerminate();


    return 0;
}

