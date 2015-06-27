#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

constexpr int Width  = 800;
constexpr int Height = 600;

int main(int argc, char ** argv) {

  glfwSetErrorCallback([](int, const char* desc) {
    std::cerr << "GLFW Error: " << desc << ".\n";
  });

  if (! glfwInit()) return -1;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  auto window = glfwCreateWindow(Width, Height, "GLCookBook", nullptr, nullptr);
  if (! window) {
    std::cerr << "GLFW Error: Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window,
    [](GLFWwindow* window,
     int key,
     int scancode,
     int action,
     int mode) {
      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
      }
    });

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cerr << "GLEW Error: Failed to initialize GLEW\n";
    glfwTerminate();
    return -1;
  }

  /* We can now call OpenGL functions from here... */


  // OpenGL settings
  glViewport(0, 0, Width, Height);

  while (! glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
