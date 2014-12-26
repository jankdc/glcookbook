#include "camera.h"
#include "common.h"
#include "texture.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <unordered_map>


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
const auto WINDOW_TITLE  = "GL Cook Book - Playing with Light Casters.";
const auto RATIO = static_cast<float>(WINDOW_WIDTH/WINDOW_HEIGHT);


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


const auto MATERIALS = std::unordered_map<std::string, Material> {
    {"emerald", Material{
        glm::vec3(0.02150f, 0.17450f, 0.02150f), // ka
        glm::vec3(0.07568f, 0.61424f, 0.07568f), // kd
        glm::vec3(0.63300f, 0.72781f, 0.63300f), // ks
        128.0 * 0.6f // sh
    }},
    {"jade", Material{
        glm::vec3(0.13500f, 0.22250f, 0.15750f), // ka
        glm::vec3(0.54000f, 0.89000f, 0.63000f), // kd
        glm::vec3(0.31622f, 0.31622f, 0.31622f), // ks
        128.0f * 0.1 // sh
    }},
    {"obsidian", Material{
        glm::vec3(0.05375f, 0.05000f, 0.06625f), // ka
        glm::vec3(0.18275f, 0.17000f, 0.22525f), // kd
        glm::vec3(0.33274f, 0.32863f, 0.34643f), // ks
        128.0f * 0.3f // sh
    }},
    {"pearl", Material{
        glm::vec3(0.25000f, 0.20725f, 0.20725f), // ka
        glm::vec3(1.00000f, 0.82900f, 0.82900f), // kd
        glm::vec3(0.29665f, 0.29665f, 0.29665f), // ks
        128.0f * 0.088f // sh
    }},
    {"ruby", Material{
        glm::vec3(0.17450f, 0.01175f, 0.01175f), // ka
        glm::vec3(0.61424f, 0.04136f, 0.04136f), // kd
        glm::vec3(0.72781f, 0.62696f, 0.62696f), // ks
        128.0f * 0.6f // sh
    }},
    {"gold", Material{
        glm::vec3(0.24725f, 0.19950f, 0.07450f), // ka
        glm::vec3(0.75164f, 0.60648f, 0.22648f), // kd
        glm::vec3(0.62828f, 0.55580f, 0.36606f), // ks
        128.0f * 0.4f // sh
    }},
    {"cyan_plastic", Material{
        glm::vec3(0.00000f, 0.10000f, 0.06000f), // ka
        glm::vec3(0.00000f, 0.50981f, 0.50980f), // kd
        glm::vec3(0.50196f, 0.50196f, 0.50196f), // ks
        128.0f * 0.25f // sh
    }},
    {"bronze", Material{
        glm::vec3(0.21250f, 0.12750f, 0.05400f), // ka
        glm::vec3(0.71400f, 0.42840f, 0.18144f), // kd
        glm::vec3(0.39355f, 0.27191f, 0.16672f), // ks
        128.0f * 0.2f // sh
    }},
    {"bronze", Material{
        glm::vec3(0.21250f, 0.12750f, 0.05400f), // ka
        glm::vec3(0.71400f, 0.42840f, 0.18144f), // kd
        glm::vec3(0.39355f, 0.27191f, 0.16672f), // ks
        128.0f * 0.2f // sh
    }}};


const auto CUBES = std::vector<Cube>{
    { MATERIALS.at("cyan_plastic"), glm::vec3( 0.0f, 0.0f, 0.0f)  },
    { MATERIALS.at("emerald"),      glm::vec3( 2.0f, 5.0f,-15.0f) },
    { MATERIALS.at("ruby"),         glm::vec3(-1.5f,-2.2f,-2.5f)  },
    { MATERIALS.at("emerald"),      glm::vec3(-3.8f,-2.0f,-12.3f) },
    { MATERIALS.at("pearl"),        glm::vec3( 2.4f,-0.4f,-3.5f)  },
    { MATERIALS.at("pearl"),        glm::vec3(-1.7f, 3.0f,-7.5f)  },
    { MATERIALS.at("obsidian"),     glm::vec3( 1.3f,-2.0f,-2.5f)  },
    { MATERIALS.at("gold"),         glm::vec3( 1.5f, 2.0f,-2.5f)  },
    { MATERIALS.at("jade"),         glm::vec3( 1.5f, 0.2f,-1.5f)  },
    { MATERIALS.at("bronze"),       glm::vec3(-1.3f, 1.0f,-1.5f)  }};


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

    auto texLoader = glc::TexLoader();


    // CUBE MESH VAO + TEXTURE
    auto cubeMeshId = glc::makeMesh(VERTICES);
    auto cubeMeshTex = texLoader.load("res/images/box.png");
    auto cubeMeshSpec = texLoader.load("res/images/box_specular.png");


    // CUBE SHADER
    auto objectVShader = glc::makeVShader("res/lightcasters/object_v.glsl");
    auto objectFShader = glc::makeFShader("res/lightcasters/object_f.glsl");
    auto objectShader = glc::makeProgram({objectVShader, objectFShader});
    glc::printShaderStatus(objectVShader);
    glc::printShaderStatus(objectFShader);
    glDeleteShader(objectFShader);
    glDeleteShader(objectVShader);


    // LIGHT SOURCE SHADER
    auto lampVShader = glc::makeVShader("res/lightcasters/lamp_v.glsl");
    auto lampFShader = glc::makeFShader("res/lightcasters/lamp_f.glsl");
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
    light.ka  = glm::vec3(0.1f);
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

        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(objectShader);

        {
            auto matKdId = glGetUniformLocation(objectShader, "Material.kd");
            auto matKsId = glGetUniformLocation(objectShader, "Material.ks");
            auto matShineId = glGetUniformLocation(objectShader, "Material.a");

            auto dlightKaId = glGetUniformLocation(objectShader, "DLight.ka");
            auto dlightKdId = glGetUniformLocation(objectShader, "DLight.kd");
            auto dlightKsId = glGetUniformLocation(objectShader, "DLight.ks");
            auto dlightDirId = glGetUniformLocation(objectShader, "DLight.direction");

            auto plightPosId = glGetUniformLocation(objectShader, "PLight.position");
            auto plightKaId = glGetUniformLocation(objectShader, "PLight.ka");
            auto plightKdId = glGetUniformLocation(objectShader, "PLight.kd");
            auto plightKsId = glGetUniformLocation(objectShader, "PLight.ks");
            auto plightKcId = glGetUniformLocation(objectShader, "PLight.kc");
            auto plightKlId = glGetUniformLocation(objectShader, "PLight.kl");
            auto plightKqId = glGetUniformLocation(objectShader, "PLight.kq");

            auto slightKaId = glGetUniformLocation(objectShader, "SLight.ka");
            auto slightKdId = glGetUniformLocation(objectShader, "SLight.kd");
            auto slightKsId = glGetUniformLocation(objectShader, "SLight.ks");
            auto slightDirId = glGetUniformLocation(objectShader, "SLight.spotDir");
            auto slightPosId = glGetUniformLocation(objectShader, "SLight.position");
            auto slightCutOffId = glGetUniformLocation(objectShader, "SLight.cutOffAngle");
            auto slightCutInId = glGetUniformLocation(objectShader, "SLight.cutInAngle");

            auto modelId = glGetUniformLocation(objectShader, "Model");
            auto viewId  = glGetUniformLocation(objectShader, "View");
            auto projectionId = glGetUniformLocation(objectShader, "Projection");
            auto normalMatId  = glGetUniformLocation(objectShader, "Normal");
            auto cameraPosId  = glGetUniformLocation(objectShader, "CameraPosition");
            auto cameraPos = camera.getPosition();
            auto cameraDir = camera.getDirection();

            glUniform3f(dlightDirId, -0.2f, -1.0f, -0.3f);
            glUniform3f(dlightKaId, 0.1f, 0.1f, 0.1f);
            glUniform3f(dlightKdId, light.kd.r, light.kd.g, light.kd.b);
            glUniform3f(dlightKsId, 1.0f, 1.0f, 1.0f);

            glUniform3f(plightPosId, light.pos.x, light.pos.y, light.pos.z);
            glUniform3f(plightKaId, light.ka.r, light.ka.g, light.ka.b);
            glUniform3f(plightKdId, light.kd.r, light.kd.g, light.kd.b);
            glUniform3f(plightKsId, 1.0f, 1.0f, 1.0f);
            glUniform1f(plightKcId, 1.0f);
            glUniform1f(plightKlId, 0.09f);
            glUniform1f(plightKqId, 0.032f);

            glUniform3f(slightPosId, cameraPos.x, cameraPos.y, cameraPos.z);
            glUniform3f(slightDirId, cameraDir.x, cameraDir.y, cameraDir.z);
            glUniform3f(slightKaId, light.ka.r, light.ka.g, light.ka.b);
            glUniform3f(slightKdId, light.kd.r, light.kd.g, light.kd.b);
            glUniform3f(slightKsId, 1.0f, 1.0f, 1.0f);
            glUniform1f(slightCutInId, glm::cos(glm::radians(12.5f)));
            glUniform1f(slightCutOffId, glm::cos(glm::radians(17.5f)));

            glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));
            glUniform3f(cameraPosId, cameraPos.x, cameraPos.y, cameraPos.z);

            glUniform1f(matKdId, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cubeMeshTex);

            glUniform1f(matKsId, 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, cubeMeshSpec);

            glUniform1f(matShineId, 64.0f);

            glBindVertexArray(cubeMeshId);
            for (auto cube : CUBES) {
                auto position = cube.position;
                auto model = glm::mat4(1.0f);
                auto modelRotationAngle = glm::radians(-55.0f);
                auto modelRotationAxis = glm::vec3(1.0f, 0.3f, 0.5f);
                model = glm::translate(model, position);
                model = glm::rotate(model, modelRotationAngle, modelRotationAxis);
                auto normalMat = glm::mat3(glm::transpose(glm::inverse(model)));
                glUniformMatrix3fv(normalMatId, 1, GL_FALSE, glm::value_ptr(normalMat));
                glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, VERTICES.size());
            }
            glBindVertexArray(0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
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

