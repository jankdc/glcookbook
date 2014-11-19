#include <GL/glew.h>
#include <GLFW/glfw3.h>

const auto WINDOW_WIDTH  = 800;
const auto WINDOW_HEIGHT = 600;
const auto WINDOW_TITLE  = "GL Cook Book - Window Creation";

void onKeyChange(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char const *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    auto window = glfwCreateWindow(
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        WINDOW_TITLE, 
        nullptr, 
        nullptr);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, onKeyChange);

    // Give access to modern GL functions
    glewExperimental = GL_TRUE;
    glewInit();

    // Sync the rendering window with GLFW window
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    while (! glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void onKeyChange(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}