#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImagePlus.h>

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

    auto vertexShaderPath = "res/textures/vertex.glsl";
    auto vertexShaderText = makeString(vertexShaderPath);
    auto vertexShader = makeShader(GL_VERTEX_SHADER, vertexShaderText);
    printShaderStatus(vertexShader);

    auto fragmentShaderPath = "res/textures/fragment.glsl";
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

    GLuint tex1;
    glGenTextures(1, &tex1);

    GLuint tex2;
    glGenTextures(1, &tex2);

    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLfloat vertices[] = {
        // Positions    // Colors           // Texture Coords
        0.5f, 0.5f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
        0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
       -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
       -0.5f, 0.5f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    };

    GLuint indices[] = {
        2, 3, 0,
        0, 1, 2
    };

    // Setup Texture Settings
    fipImage container;
    container.load("res/images/container.jpg");
    container.convertTo32Bits();

    fipImage awesomeface;
    awesomeface.load("res/images/awesomeface.png");
    awesomeface.convertTo32Bits();

    auto containerTexData = container.accessPixels();
    auto containerTexWidth = container.getWidth();
    auto containerTexHeight = container.getHeight();


    auto awesomefaceTexData = awesomeface.accessPixels();
    auto awesomefaceTexWidth = awesomeface.getWidth();
    auto awesomefaceTexHeight = awesomeface.getHeight();

    auto minSetting = GL_LINEAR_MIPMAP_LINEAR;
    auto magSetting = GL_LINEAR;

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindTexture(GL_TEXTURE_2D, tex1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, containerTexWidth, containerTexHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, containerTexData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minSetting);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magSetting);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, tex2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, awesomefaceTexWidth, awesomefaceTexHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, awesomefaceTexData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minSetting);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magSetting);
    glGenerateMipmap(GL_TEXTURE_2D);

    auto stride = sizeof(GLfloat) * 7;
    auto vertexOff = (GLvoid*)(0);
    auto colorOff = (GLvoid*)(sizeof(GLfloat) * 2);
    auto texOff = (GLvoid*)(sizeof(GLfloat) * 5);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, vertexOff);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, colorOff);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, texOff);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    container.clear();
    awesomeface.clear();

    while (! glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex2);
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture2"), 1);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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