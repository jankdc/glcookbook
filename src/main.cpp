#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

constexpr int WIDTH  = 800;
constexpr int HEIGHT = 600;
constexpr const char * TITLE = "An OpenGL Cookbook.";

std::string getFileContents(std::string path);
GLuint makeShader(std::string path, GLenum type);
GLuint makeProgram(std::vector<GLuint> shaders);

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

  auto window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
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

  /* NOTE: We can now call OpenGL functions from here. */


  // OpenGL settings
  glViewport(0, 0, WIDTH, HEIGHT);

  // This array form 3 sets of X, Y, Z vertex coordinates, forming a triangle.
  GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  // Sets up our vertex array object (VAO) in order to store the state
  // of our buffer and vertex attribute bindings, in order to reduce the amount
  // of GL calls later when we draw.
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  const auto vs = makeShader("data/shaders/simple_vs.glsl", GL_VERTEX_SHADER);
  const auto fs = makeShader("data/shaders/simple_fs.glsl", GL_FRAGMENT_SHADER);
  const auto program = makeProgram({vs, fs});

  while (! glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // Reset the buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  // Cleanup code.
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteProgram(program);
  glfwTerminate();
  return 0;
}


std::string getFileContents(std::string path) {
  std::ifstream file(path);
  file.exceptions(std::ifstream::failbit);
  file.seekg(0, std::ios::end);
  std::string contents(file.tellg(), ' ');
  file.seekg(0, std::ios::beg);
  file.read(&contents.front(), contents.size());
  file.close();
  return contents;
}

GLuint makeShader(std::string path, GLenum type) {
  const auto shaderSrc    = getFileContents(path);
  const auto shaderSrcId  = glCreateShader(type);
  const auto shaderSrcRaw = shaderSrc.c_str();
  glShaderSource(shaderSrcId, 1, &shaderSrcRaw, nullptr);
  glCompileShader(shaderSrcId);
  return shaderSrcId;
}

GLuint makeProgram(std::vector<GLuint> shaders) {
  const auto program = glCreateProgram();

  for (auto s : shaders) {
    glAttachShader(program, s);
  }

  glLinkProgram(program);

  // Once we've linked the shaders to the program, we no longer need shader
  // sources anymore so we delete them here.
  for (auto s : shaders) {
    glDeleteShader(s);
  }

  return program;
}
