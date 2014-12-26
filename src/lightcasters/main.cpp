#include "camera.h"
#include "common.h"
#include "scene.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <unordered_map>


const auto WINDOW_WIDTH  = 800;
const auto WINDOW_HEIGHT = 600;
const auto WINDOW_TITLE  = "GL Cook Book - Playing with Light Casters.";

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


    // CUBE SHADER
    auto cubeVShader = glc::makeVShader("res/lightcasters/object_v.glsl");
    auto cubeFShader = glc::makeFShader("res/lightcasters/object_f.glsl");
    auto cubeShader = glc::makeProgram({cubeVShader, cubeFShader});
    glc::printShaderStatus(cubeVShader);
    glc::printShaderStatus(cubeFShader);
    glDeleteShader(cubeFShader);
    glDeleteShader(cubeVShader);


    // LIGHT SOURCE SHADER
    auto lampVShader = glc::makeVShader("res/lightcasters/lamp_v.glsl");
    auto lampFShader = glc::makeFShader("res/lightcasters/lamp_f.glsl");
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


    auto basicScene = glc::BasicScene(window, meshes, shaders, textures);
    basicScene.setup();


    /*
     __  __          _____ _   _   _      ____   ____  _____
    |  \/  |   /\   |_   _| \ | | | |    / __ \ / __ \|  __ \
    | \  / |  /  \    | | |  \| | | |   | |  | | |  | | |__) |
    | |\/| | / /\ \   | | | . ` | | |   | |  | | |  | |  ___/
    | |  | |/ ____ \ _| |_| |\  | | |___| |__| | |__| | |
    |_|  |_/_/    \_\_____|_| \_| |______\____/ \____/|_|

    */

    while (! glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        basicScene.update();
    }


    // CLEANUP
    glDeleteProgram(cubeShader);
    glDeleteProgram(lampShader);
    glfwTerminate();


    return 0;
}

