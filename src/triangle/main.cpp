#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

const auto WINDOW_WIDTH  = 800;
const auto WINDOW_HEIGHT = 600;
const auto WINDOW_TITLE  = "GL Cook Book - Creating a Triangle";

void onKeyUpdate(GLFWwindow* window, int key, int code, int action, int mode);

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
    glfwSetKeyCallback(window, onKeyUpdate);

    // Give access to modern GL functions
    glewExperimental = GL_TRUE;
    glewInit();

    auto vertexShaderSource   = "";
    auto fragmentShaderSource = "";

    std::ifstream file;

    file.open("res/triangle/vertex.glsl");
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        auto size = file.tellg();
        std::string buffer(size, ' ');
        file.seekg(0);
        file.read(&buffer[0], size);
        vertexShaderSource = buffer.c_str();
    } else {
        std::cout << "[error]: Vertex shader file is not available.\n";
        glfwTerminate();
        return -1;
    }
    file.close();

    file.open("res/triangle/fragment.glsl");
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        auto size = file.tellg();
        std::string buffer(size, ' ');
        file.seekg(0);
        file.read(&buffer[0], size);
        fragmentShaderSource = buffer.c_str();
    } else {
        std::cout << "[error]: Fragment shader file is not available.\n";
        glfwTerminate();
        return -1;
    }
    file.close();

    GLfloat vertices[] = {
       -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f,  0.5f
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    {
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (! success) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cout << "[error]: Vertex shader failed to compile.\n";
            std::cout << infoLog << "\n";
            glfwTerminate();
            return -1;
        }
    }

    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    {
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if (! success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cout << "[error]: Fragment shader failed to compile.\n";
            std::cout << infoLog << "\n";
            glfwTerminate();
            return -1;
        }
    }

    auto shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    {
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

        if (! success) {
            glGetShaderInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cout << "[error]: Shader program failed to compile.\n";
            std::cout << infoLog << "\n";
            glfwTerminate();
            return -1;
        }
    }

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
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glfwSwapBuffers(window);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

void onKeyUpdate(GLFWwindow* window, int key, int code, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
