#include <GL/glew.h>
#include <GLFW/glfw3.h>

const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 600;
const char* WINDOW_TITLE  = "GL Cook Book - Window Creation";

int main(int argc, char const *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        WINDOW_TITLE, 
        NULL, 
        NULL);

    glfwMakeContextCurrent(window);

    // Give access to modern GL functions
    glewExperimental = GL_TRUE;
    glewInit();

    // Sync the rendering window with GLFW window
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    while (! glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}