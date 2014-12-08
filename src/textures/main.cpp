#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

using std::string;
using std::ifstream;
using std::ios_base;

const auto WINDOW_WIDTH  = 800;
const auto WINDOW_HEIGHT = 600;
const auto WINDOW_TITLE  = "GL Cook Book - Loading Textures.";

void updateKey(GLFWwindow* window, int key, int code, int action, int mode);
void printShaderStatus(GLuint shader);
GLuint makeShader(GLenum shaderType, string text);
string makeString(string path);

int main(int argc, char const *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // GLEW has a bug for OS X where it still calls some legacy functions.
    // We need this for now to prevent GLFW from crashing when legacy
    // functions are called.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto window = glfwCreateWindow(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        WINDOW_TITLE,
        nullptr,
        nullptr);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, updateKey);

    glewExperimental = GL_TRUE;
    glewInit();

    // OS X has a bug that does not normalise the viewport coordinates
    // at all so only uncomment this when it's really needed since GLFW
    // have a normalized default anyway.
    // glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    auto vertexShaderPath = "res/triangle/vertex.glsl";
    auto vertexShaderText = makeString(vertexShaderPath);
    auto vertexShader = makeShader(GL_VERTEX_SHADER, vertexShaderText);
    printShaderStatus(vertexShader);

    auto fragmentShaderPath = "res/triangle/fragment.glsl";
    auto fragmentShaderText = makeString(fragmentShaderPath);
    auto fragmentShader = makeShader(GL_FRAGMENT_SHADER, fragmentShaderText);
    printShaderStatus(fragmentShader);

    auto shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    GLfloat vertices[] = {
       -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f,  0.5f
    };

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (! glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

void updateKey(GLFWwindow* window, int key, int code, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void printShaderStatus(GLuint shader)
{
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (! success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << infoLog << "\n";
    }
}

GLuint makeShader(GLenum shaderType, string text)
{
    auto shader = glCreateShader(shaderType);
    auto cstring = text.c_str();
    glShaderSource(shader, 1, &cstring, nullptr);
    glCompileShader(shader);
    return shader;
}

string makeString(string path)
{
    ifstream file(path);

    try
    {
        file.exceptions(ifstream::failbit);
    }
    catch (const ios_base::failure& err)
    {
        throw ios_base::failure(path + " is not available.");
    }

    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    string buffer(size, ' ');
    file.seekg(0);
    file.read(&buffer[0], size);
    file.close();
    return buffer;
}