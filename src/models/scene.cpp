#include "camera.hpp"
#include "scene.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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



glc::Scene::Scene(GLFWwindow* window)
: mWindow(window),
  mCamera(window),
  mPhong({"res/models/phong-vt.glsl", "res/models/phong-fm.glsl"}),
  mLight(),
  mNanoSuit("res/images/nano/nanosuit.obj")
{
    mLight.ka = glm::vec3(0.1f);
    mLight.kd = glm::vec3(1.0f);
    mLight.ks = glm::vec3(1.0f);
    mLight.pos = glm::vec3(1.2f, 1.0f, 2.0f);
    mCamera.setPosition(glm::vec3(0.5f, 0.0f, 5.0f));
}

void glc::Scene::update(float diftime)
{
    mCamera.update(diftime);
    mLight.pos.x = 1.0f + sinf(glfwGetTime()) * 2.0f;
    mLight.pos.y = sinf(glfwGetTime() / 2.0f) * 1.0f;
}

void glc::Scene::draw()
{
    auto width = 0, height = 0;
    glfwGetWindowSize(mWindow, &width, &height);
    auto ratio = static_cast<float>(width)/static_cast<float>(height);

    auto view = mCamera.generateMat();
    auto model = glm::mat4(1.0f);
    auto normal = glm::mat3(glm::transpose(glm::inverse(model)));
    auto projection = glm::perspective(45.0f, ratio, 0.1f, 1000.0f);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    mPhong.use();
    mPhong.setUniform("SLight.position", mCamera.getPosition());
    mPhong.setUniform("SLight.spotDir", mCamera.getDirection());
    mPhong.setUniform("SLight.ka", glm::vec3(0.0f));
    mPhong.setUniform("SLight.kd", glm::vec3(1.0f));
    mPhong.setUniform("SLight.ks", glm::vec3(1.0f));
    mPhong.setUniform("SLight.cutInAngle", glm::cos(glm::radians(12.5f)));
    mPhong.setUniform("SLight.cutOffAngle", glm::cos(glm::radians(17.5f)));
    mPhong.setUniform("DLight.ka", glm::vec3(0.1f));
    mPhong.setUniform("DLight.kd", glm::vec3(0.1f));
    mPhong.setUniform("DLight.ks", glm::vec3(0.1f));
    mPhong.setUniform("DLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    mPhong.setUniform("PLight.position", mLight.pos);
    mPhong.setUniform("PLight.ka", mLight.ka);
    mPhong.setUniform("PLight.kd", mLight.kd);
    mPhong.setUniform("PLight.ks", glm::vec3(1.0f));
    mPhong.setUniform("PLight.kc", 1.0f);
    mPhong.setUniform("PLight.kl", 0.09f);
    mPhong.setUniform("PLight.kq", 0.032f);
    mPhong.setUniform("Material.a", 64.0f);
    mPhong.setUniform("Model", model);
    mPhong.setUniform("View", view);
    mPhong.setUniform("Projection", projection);
    mPhong.setUniform("Normal", normal);
    mPhong.setUniform("CameraPosition", mCamera.getPosition());

    mNanoSuit.draw(&mPhong);

    glUseProgram(0);
}
