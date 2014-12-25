#include "camera.h"
#include "common.h"

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
const auto WINDOW_TITLE  = "GL Cook Book - Playing with Light Maps.";
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
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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


    // CUBE MESH VAO + TEXTURE
    auto cubeMeshId = glc::makeMesh(VERTICES);
    auto cubeMeshTex = glc::makeTexture("res/images/box.png");


    // CUBE SHADER
    auto objectVShader = glc::makeVShader("res/lightmaps/object_v.glsl");
    auto objectFShader = glc::makeFShader("res/lightmaps/object_f.glsl");
    auto objectShader = glc::makeProgram({objectVShader, objectFShader});
    glc::printShaderStatus(objectVShader);
    glc::printShaderStatus(objectFShader);
    glDeleteShader(objectFShader);
    glDeleteShader(objectVShader);


    // LIGHT SOURCE SHADER
    auto lampVShader = glc::makeVShader("res/lightmaps/lamp_v.glsl");
    auto lampFShader = glc::makeFShader("res/lightmaps/lamp_f.glsl");
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
    light.ka  = glm::vec3(0.5f);
    light.kd  = glm::vec3(1.0f);
    light.ks  = glm::vec3(1.0f);
    light.pos = glm::vec3(1.2f, 1.0f, 2.0f);


    /*
     __  __       _______ ______ _____  _____          _
    |  \/  |   /\|__   __|  ____|  __ \|_   _|   /\   | |
    | \  / |  /  \  | |  | |__  | |__) | | |    /  \  | |
    | |\/| | / /\ \ | |  |  __| |  _  /  | |   / /\ \ | |
    | |  | |/ ____ \| |  | |____| | \ \ _| |_ / ____ \| |____
    |_|  |_/_/    \_\_|  |______|_|  \_\_____/_/    \_\______|

    */

    std::unordered_map<std::string, Material> materials;
    materials.emplace("emerald", Material{
        glm::vec3(0.02150f, 0.17450f, 0.02150f), // ka
        glm::vec3(0.07568f, 0.61424f, 0.07568f), // kd
        glm::vec3(0.63300f, 0.72781f, 0.63300f), // ks
        128.0 * 0.6f // sh
    });
    materials.emplace("jade", Material{
        glm::vec3(0.13500f, 0.22250f, 0.15750f), // ka
        glm::vec3(0.54000f, 0.89000f, 0.63000f), // kd
        glm::vec3(0.31622f, 0.31622f, 0.31622f), // ks
        128.0f * 0.1 // sh
    });
    materials.emplace("obsidian", Material{
        glm::vec3(0.05375f, 0.05000f, 0.06625f), // ka
        glm::vec3(0.18275f, 0.17000f, 0.22525f), // kd
        glm::vec3(0.33274f, 0.32863f, 0.34643f), // ks
        128.0f * 0.3f // sh
    });
    materials.emplace("pearl", Material{
        glm::vec3(0.25000f, 0.20725f, 0.20725f), // ka
        glm::vec3(1.00000f, 0.82900f, 0.82900f), // kd
        glm::vec3(0.29665f, 0.29665f, 0.29665f), // ks
        128.0f * 0.088f // sh
    });
    materials.emplace("ruby", Material{
        glm::vec3(0.17450f, 0.01175f, 0.01175f), // ka
        glm::vec3(0.61424f, 0.04136f, 0.04136f), // kd
        glm::vec3(0.72781f, 0.62696f, 0.62696f), // ks
        128.0f * 0.6f // sh
    });
    materials.emplace("gold", Material{
        glm::vec3(0.24725f, 0.19950f, 0.07450f), // ka
        glm::vec3(0.75164f, 0.60648f, 0.22648f), // kd
        glm::vec3(0.62828f, 0.55580f, 0.36606f), // ks
        128.0f * 0.4f // sh
    });
    materials.emplace("cyan_plastic", Material{
        glm::vec3(0.00000f, 0.10000f, 0.06000f), // ka
        glm::vec3(0.00000f, 0.50981f, 0.50980f), // kd
        glm::vec3(0.50196f, 0.50196f, 0.50196f), // ks
        128.0f * 0.25f // sh
    });
    materials.emplace("bronze", Material{
        glm::vec3(0.21250f, 0.12750f, 0.05400f), // ka
        glm::vec3(0.71400f, 0.42840f, 0.18144f), // kd
        glm::vec3(0.39355f, 0.27191f, 0.16672f), // ks
        128.0f * 0.2f // sh
    });
    materials.emplace("bronze", Material{
        glm::vec3(0.21250f, 0.12750f, 0.05400f), // ka
        glm::vec3(0.71400f, 0.42840f, 0.18144f), // kd
        glm::vec3(0.39355f, 0.27191f, 0.16672f), // ks
        128.0f * 0.2f // sh
    });


    /*
      _____ _    _ ____  ______  _____
     / ____| |  | |  _ \|  ____|/ ____|
    | |    | |  | | |_) | |__  | (___
    | |    | |  | |  _ <|  __|  \___ \
    | |____| |__| | |_) | |____ ____) |
     \_____|\____/|____/|______|_____/

    */

    auto cubes = std::vector<Cube>{
        { materials.at("cyan_plastic"), glm::vec3( 0.0f, 0.0f, 0.0f)  },
        { materials.at("emerald"),      glm::vec3( 2.0f, 5.0f,-15.0f) },
        { materials.at("ruby"),         glm::vec3(-1.5f,-2.2f,-2.5f)  },
        { materials.at("emerald"),      glm::vec3(-3.8f,-2.0f,-12.3f) },
        { materials.at("pearl"),        glm::vec3( 2.4f,-0.4f,-3.5f)  },
        { materials.at("pearl"),        glm::vec3(-1.7f, 3.0f,-7.5f)  },
        { materials.at("obsidian"),     glm::vec3( 1.3f,-2.0f,-2.5f)  },
        { materials.at("gold"),         glm::vec3( 1.5f, 2.0f,-2.5f)  },
        { materials.at("jade"),         glm::vec3( 1.5f, 0.2f,-1.5f)  },
        { materials.at("bronze"),       glm::vec3(-1.3f, 1.0f,-1.5f)  }
    };


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


            for (auto cube : cubes) {
                auto material = cube.material;
                auto position = cube.position;
                auto model = glm::mat4(1.0f);
                auto modelRotationAngle = glm::radians(newTime * -55.0f);
                auto modelRotationAxis = glm::vec3(1.0f, 0.3f, 0.5f);
                model = glm::translate(model, position);
                model = glm::rotate(model, modelRotationAngle, modelRotationAxis);
                auto normalMat = glm::mat3(glm::transpose(glm::inverse(model)));

                glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));
                glUniformMatrix3fv(normalMatId, 1, GL_FALSE, glm::value_ptr(normalMat));
                glUniform3f(cameraPosId, cameraPos.x, cameraPos.y, cameraPos.z);


                glUniform1f(matKdId, 0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cubeMeshTex);


                glUniform3f(matKsId, material.ks.r, material.ks.g, material.ks.b);
                glUniform1f(matShineId, material.sh);


                glBindVertexArray(cubeMeshId);
                glDrawArrays(GL_TRIANGLES, 0, VERTICES.size());
                glBindVertexArray(0);
            }
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

