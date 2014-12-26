#include "camera.h"
#include "scene.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>
#include <vector>
#include <string>


const auto MATERIALS = std::unordered_map<std::string, glc::Material> {
    {"emerald", glc::Material{
        glm::vec3(0.02150f, 0.17450f, 0.02150f), // ka
        glm::vec3(0.07568f, 0.61424f, 0.07568f), // kd
        glm::vec3(0.63300f, 0.72781f, 0.63300f), // ks
        128.0 * 0.6f // sh
    }},
    {"jade", glc::Material{
        glm::vec3(0.13500f, 0.22250f, 0.15750f), // ka
        glm::vec3(0.54000f, 0.89000f, 0.63000f), // kd
        glm::vec3(0.31622f, 0.31622f, 0.31622f), // ks
        128.0f * 0.1 // sh
    }},
    {"obsidian", glc::Material{
        glm::vec3(0.05375f, 0.05000f, 0.06625f), // ka
        glm::vec3(0.18275f, 0.17000f, 0.22525f), // kd
        glm::vec3(0.33274f, 0.32863f, 0.34643f), // ks
        128.0f * 0.3f // sh
    }},
    {"pearl", glc::Material{
        glm::vec3(0.25000f, 0.20725f, 0.20725f), // ka
        glm::vec3(1.00000f, 0.82900f, 0.82900f), // kd
        glm::vec3(0.29665f, 0.29665f, 0.29665f), // ks
        128.0f * 0.088f // sh
    }},
    {"ruby", glc::Material{
        glm::vec3(0.17450f, 0.01175f, 0.01175f), // ka
        glm::vec3(0.61424f, 0.04136f, 0.04136f), // kd
        glm::vec3(0.72781f, 0.62696f, 0.62696f), // ks
        128.0f * 0.6f // sh
    }},
    {"gold", glc::Material{
        glm::vec3(0.24725f, 0.19950f, 0.07450f), // ka
        glm::vec3(0.75164f, 0.60648f, 0.22648f), // kd
        glm::vec3(0.62828f, 0.55580f, 0.36606f), // ks
        128.0f * 0.4f // sh
    }},
    {"cyan_plastic", glc::Material{
        glm::vec3(0.00000f, 0.10000f, 0.06000f), // ka
        glm::vec3(0.00000f, 0.50981f, 0.50980f), // kd
        glm::vec3(0.50196f, 0.50196f, 0.50196f), // ks
        128.0f * 0.25f // sh
    }},
    {"bronze", glc::Material{
        glm::vec3(0.21250f, 0.12750f, 0.05400f), // ka
        glm::vec3(0.71400f, 0.42840f, 0.18144f), // kd
        glm::vec3(0.39355f, 0.27191f, 0.16672f), // ks
        128.0f * 0.2f // sh
    }},
    {"bronze", glc::Material{
        glm::vec3(0.21250f, 0.12750f, 0.05400f), // ka
        glm::vec3(0.71400f, 0.42840f, 0.18144f), // kd
        glm::vec3(0.39355f, 0.27191f, 0.16672f), // ks
        128.0f * 0.2f // sh
    }}};


const auto CUBES = std::vector<glc::Cube>{
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


glc::BasicScene::BasicScene(GLFWwindow* window,
    std::unordered_map<std::string, glc::Mesh> meshes,
    std::unordered_map<std::string, GLuint> shaders,
    std::unordered_map<std::string, GLuint> textures)
: m_window(window),
  m_camera(window),
  m_meshes(meshes),
  m_shaders(shaders),
  m_textures(textures)
{

}

void glc::BasicScene::setup()
{
    m_light.ka  = glm::vec3(0.1f);
    m_light.kd  = glm::vec3(1.0f);
    m_light.ks  = glm::vec3(1.0f);
    m_light.pos = glm::vec3(1.2f, 1.0f, 2.0f);

    m_camera.setPosition(glm::vec3(0.5f, 0.0f, 5.0f));
}

void glc::BasicScene::update()
{
    auto newTime = static_cast<float>(glfwGetTime());
    m_delta = glm::max(newTime - m_oldTime, 0.0f);
    m_oldTime = newTime;

    m_camera.update(m_delta);
    m_light.pos.x = 1.0f + sinf(newTime) * 2.0f;
    m_light.pos.y = sinf(newTime / 2.0f) * 1.0f;

    auto view = m_camera.generateMat();
    auto width = 0, height = 0;
    glfwGetWindowSize(m_window, &width, &height);
    auto projection = glm::perspective(
        45.0f,
        static_cast<float>(width)/static_cast<float>(height),
        0.1f,
        1000.0f);

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto cubeShader = m_shaders.at("cube");
    glUseProgram(cubeShader);

    {
        auto matKdId = glGetUniformLocation(cubeShader, "Material.kd");
        auto matKsId = glGetUniformLocation(cubeShader, "Material.ks");
        auto matShineId = glGetUniformLocation(cubeShader, "Material.a");

        auto dlightKaId = glGetUniformLocation(cubeShader, "DLight.ka");
        auto dlightKdId = glGetUniformLocation(cubeShader, "DLight.kd");
        auto dlightKsId = glGetUniformLocation(cubeShader, "DLight.ks");
        auto dlightDirId = glGetUniformLocation(cubeShader, "DLight.direction");

        auto plightPosId = glGetUniformLocation(cubeShader, "PLight.position");
        auto plightKaId = glGetUniformLocation(cubeShader, "PLight.ka");
        auto plightKdId = glGetUniformLocation(cubeShader, "PLight.kd");
        auto plightKsId = glGetUniformLocation(cubeShader, "PLight.ks");
        auto plightKcId = glGetUniformLocation(cubeShader, "PLight.kc");
        auto plightKlId = glGetUniformLocation(cubeShader, "PLight.kl");
        auto plightKqId = glGetUniformLocation(cubeShader, "PLight.kq");

        auto slightKaId = glGetUniformLocation(cubeShader, "SLight.ka");
        auto slightKdId = glGetUniformLocation(cubeShader, "SLight.kd");
        auto slightKsId = glGetUniformLocation(cubeShader, "SLight.ks");
        auto slightDirId = glGetUniformLocation(cubeShader, "SLight.spotDir");
        auto slightPosId = glGetUniformLocation(cubeShader, "SLight.position");
        auto slightCutOffId = glGetUniformLocation(cubeShader, "SLight.cutOffAngle");
        auto slightCutInId = glGetUniformLocation(cubeShader, "SLight.cutInAngle");

        auto modelId = glGetUniformLocation(cubeShader, "Model");
        auto viewId  = glGetUniformLocation(cubeShader, "View");
        auto projectionId = glGetUniformLocation(cubeShader, "Projection");
        auto normalMatId  = glGetUniformLocation(cubeShader, "Normal");
        auto cameraPosId  = glGetUniformLocation(cubeShader, "CameraPosition");
        auto cameraPos = m_camera.getPosition();
        auto cameraDir = m_camera.getDirection();

        glUniform3f(dlightDirId, -0.2f, -1.0f, -0.3f);
        glUniform3f(dlightKaId, 0.1f, 0.1f, 0.1f);
        glUniform3f(dlightKdId, m_light.kd.r, m_light.kd.g, m_light.kd.b);
        glUniform3f(dlightKsId, 1.0f, 1.0f, 1.0f);

        glUniform3f(plightPosId, m_light.pos.x, m_light.pos.y, m_light.pos.z);
        glUniform3f(plightKaId, m_light.ka.r, m_light.ka.g, m_light.ka.b);
        glUniform3f(plightKdId, m_light.kd.r, m_light.kd.g, m_light.kd.b);
        glUniform3f(plightKsId, 1.0f, 1.0f, 1.0f);
        glUniform1f(plightKcId, 1.0f);
        glUniform1f(plightKlId, 0.09f);
        glUniform1f(plightKqId, 0.032f);

        glUniform3f(slightPosId, cameraPos.x, cameraPos.y, cameraPos.z);
        glUniform3f(slightDirId, cameraDir.x, cameraDir.y, cameraDir.z);
        glUniform3f(slightKaId, m_light.ka.r, m_light.ka.g, m_light.ka.b);
        glUniform3f(slightKdId, m_light.kd.r, m_light.kd.g, m_light.kd.b);
        glUniform3f(slightKsId, 1.0f, 1.0f, 1.0f);
        glUniform1f(slightCutInId, glm::cos(glm::radians(12.5f)));
        glUniform1f(slightCutOffId, glm::cos(glm::radians(17.5f)));

        glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(cameraPosId, cameraPos.x, cameraPos.y, cameraPos.z);

        glUniform1f(matKdId, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textures.at("box"));

        glUniform1f(matKsId, 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textures.at("boxSpecular"));

        glUniform1f(matShineId, 64.0f);

        glBindVertexArray(m_meshes.at("cube").id);
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
            glDrawArrays(GL_TRIANGLES, 0, m_meshes.at("cube").size);
        }
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    auto lampShader = m_shaders.at("lamp");
    glUseProgram(lampShader);

    {
        auto modelId = glGetUniformLocation(lampShader, "Model");
        auto viewId = glGetUniformLocation(lampShader, "View");
        auto projectionId = glGetUniformLocation(lampShader, "Projection");

        auto model = glm::mat4(1.0f);
        model = glm::translate(model, m_light.pos);
        model = glm::scale(model, glm::vec3(0.2f));

        glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(m_meshes.at("cube").id);
        glDrawArrays(GL_TRIANGLES, 0, m_meshes.at("cube").size);
        glBindVertexArray(0);
    }

    glUseProgram(0);

    glfwSwapBuffers(m_window);
}