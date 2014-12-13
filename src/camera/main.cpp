#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImagePlus.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <vector>

using std::cout;
using std::vector;
using std::string;
using std::ifstream;
using std::ios_base;
using std::ios;

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::translate;
using glm::rotate;
using glm::scale;
using glm::value_ptr;
using glm::perspective;
using glm::cross;
using glm::lookAt;
using glm::radians;
using glm::normalize;

const auto WINDOW_WIDTH  = 800;
const auto WINDOW_HEIGHT = 600;
const auto WINDOW_TITLE  = "GL Cook Book - Playing with Camera.";
const auto CENTER_X = static_cast<double>(WINDOW_WIDTH)  / 2.0;
const auto CENTER_Y = static_cast<double>(WINDOW_HEIGHT) / 2.0;
const auto RATIO = static_cast<float>(WINDOW_WIDTH/WINDOW_HEIGHT);

void printErr(int code, const char* desc);
void printShaderStatus(GLuint shader);
string makeString(string path);
GLuint makeMesh(vector<GLfloat> vertices);
GLuint makeShader(GLenum shaderType, string text);
GLuint makeVShader(string path);
GLuint makeFShader(string path);
GLuint makeTexture(string path);
GLuint makeProgram(vector<GLuint> shaders);

class Camera {
public:
    Camera(GLFWwindow* w);

    void update(float delta);
    void updateMovement(float delta);
    void updateLook(float delta);
    void setPosition(vec3 p);
    void setDirection(vec3 d);
    void setMovementSpeed(float s);
    void setSensitivity(float s);
    mat4 getView() const;
private:
    GLFWwindow* window;
    float pitch, yaw;
    float sensitivity, speed;
    vec3 pos, dir, up, right;
    float lastX, lastY;
    bool moved;
};

int main(int argc, char const *argv[])
{
    glfwSetErrorCallback(printErr);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    // OS X has a bug that does not normalise the viewport coordinates
    // at all so only uncomment this when it's really needed since GLFW
    // have a normalized default anyway.
    // glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    auto vshader = makeVShader("res/camera/vertex.glsl");
    printShaderStatus(vshader);

    auto fshader = makeFShader("res/camera/fragment.glsl");
    printShaderStatus(fshader);

    auto program = makeProgram({vshader, fshader});

    vector<GLfloat> vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vector<vec3> cubePositions = {
        vec3( 0.0f, 0.0f, 0.0f),
        vec3( 2.0f, 5.0f,-15.0f),
        vec3(-1.5f,-2.2f,-2.5f),
        vec3(-3.8f,-2.0f,-12.3f),
        vec3( 2.4f,-0.4f,-3.5f),
        vec3(-1.7f, 3.0f,-7.5f),
        vec3( 1.3f,-2.0f,-2.5f),
        vec3( 1.5f, 2.0f,-2.5f),
        vec3( 1.5f, 0.2f,-1.5f),
        vec3(-1.3f, 1.0f,-1.5f)
    };

    auto vao = makeMesh(vertices);
    auto tex1 = makeTexture("res/images/container.jpg");
    auto tex2 = makeTexture("res/images/awesomeface.png");

    glUseProgram(program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glUniform1i(glGetUniformLocation(program, "ourTexture1"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glUniform1i(glGetUniformLocation(program, "ourTexture2"), 1);
    glUseProgram(0);

    auto camera = Camera(window);
    camera.setPosition(vec3(0.0f, 0.0f, 2.0f));

    auto lastFrame = static_cast<float>(glfwGetTime());
    auto deltaTime = 0.0f;

    while (! glfwWindowShouldClose(window))
    {
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, GL_TRUE);

        camera.update(deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);

        auto modelId = glGetUniformLocation(program, "model");
        auto viewId = glGetUniformLocation(program, "view");
        auto projId = glGetUniformLocation(program, "projection");


        auto view = camera.getView();
        auto proj = perspective(45.0f, RATIO, 0.1f, 1000.0f);

        glUniformMatrix4fv(viewId, 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(projId, 1, GL_FALSE, value_ptr(proj));

        glBindVertexArray(vao);
        for (const auto& pos : cubePositions)
        {
            auto model = mat4(1.0f);
            model = translate(model, pos);
            model = rotate(model,
                radians(currentFrame * -55.0f),
                vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelId, 1, GL_FALSE, value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glDeleteShader(vshader);
    glDeleteShader(fshader);
    glDeleteProgram(program);
    glfwTerminate();

    return 0;
}

void printErr(int code, const char* desc)
{
    cout << desc << "\n";
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

GLuint makeVShader(string path)
{
    auto shader = glCreateShader(GL_VERTEX_SHADER);
    auto shaderText = makeString(path);
    auto shaderRawText = shaderText.c_str();
    glShaderSource(shader, 1, &shaderRawText, nullptr);
    glCompileShader(shader);

    return shader;
}

GLuint makeFShader(string path)
{
    auto shader = glCreateShader(GL_FRAGMENT_SHADER);
    auto shaderText = makeString(path);
    auto shaderRawText = shaderText.c_str();
    glShaderSource(shader, 1, &shaderRawText, nullptr);
    glCompileShader(shader);

    return shader;
}

GLuint makeProgram(vector<GLuint> shaders)
{
    GLuint program = glCreateProgram();

    for (auto s : shaders)
        glAttachShader(program, s);

    glLinkProgram(program);

    return program;
}

GLuint makeTexture(string path)
{
    GLuint id;
    glGenTextures(1, &id);

    fipImage image;
    image.load(path.c_str());
    image.convertTo32Bits();

    auto data = image.accessPixels();
    auto w = image.getWidth();
    auto h = image.getHeight();

    auto minSetting = GL_LINEAR_MIPMAP_LINEAR;
    auto magSetting = GL_LINEAR;

    glBindTexture(GL_TEXTURE_2D,id);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_BGRA,GL_UNSIGNED_BYTE,data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minSetting);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magSetting);
    glGenerateMipmap(GL_TEXTURE_2D);

    image.clear();

    return id;
}

GLuint makeMesh(vector<GLfloat> vertices)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    auto vtSize = vertices.size() * sizeof(GLfloat);
    auto vtData = vertices.data();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vtSize, vtData, GL_STATIC_DRAW);

    auto stride = sizeof(GLfloat) * 5;
    auto vertexOff = (GLvoid*)(0);
    auto texOff = (GLvoid*)(sizeof(GLfloat) * 3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, vertexOff);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, texOff);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vao;
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

    file.seekg(0, ios::end);
    auto size = file.tellg();
    string buffer(size, ' ');
    file.seekg(0);
    file.read(&buffer[0], size);
    file.close();
    return buffer;
}

Camera::Camera(GLFWwindow* w)
{
    window      = w;
    pitch       = 0.0f;
    yaw         = -90.0f;
    sensitivity = 5.0f;
    speed       = 3.0f;
    dir         = vec3(0.0f, 0.0f, -1.0f);
    right       = cross(vec3(0.0f, 1.0f, 0.0f), dir);
    up          = cross(dir, right);
    right       = cross(dir, up);
}

void Camera::update(float delta)
{
    updateMovement(delta);
    updateLook(delta);
}

void Camera::updateMovement(float delta)
{
    right = cross(dir, up);
    auto deltaSpeed = speed * delta;

    if(glfwGetKey(window, GLFW_KEY_W))
        pos += deltaSpeed * dir;
    if(glfwGetKey(window, GLFW_KEY_S))
        pos -= deltaSpeed * dir;
    if(glfwGetKey(window, GLFW_KEY_A))
        pos -= glm::normalize(right) * deltaSpeed;
    if(glfwGetKey(window, GLFW_KEY_D))
        pos += glm::normalize(right) * deltaSpeed;
}

void Camera::updateLook(float delta)
{
    auto currentCursorX = CENTER_X;
    auto currentCursorY = CENTER_Y;

    glfwGetCursorPos(window, &currentCursorX, &currentCursorY);

    // glfwGetCursorPos return incorrect value when cursor hasn't moved
    // for the first time. Prevents camera jolting when you move the cursor
    // the first time.
    if (not moved and currentCursorX != 0.0 and currentCursorY != 0.0)
    {
        lastX = currentCursorX;
        lastY = currentCursorY;

        moved = true;
    }

    auto xoffset = currentCursorX - lastX;
    auto yoffset = lastY - currentCursorY;

    lastX = currentCursorX;
    lastY = currentCursorY;

    yaw += sensitivity * delta * xoffset;
    pitch += sensitivity * delta * yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    else if (pitch < -89.0f)
        pitch = -89.0f;

    dir.x = cosf(radians(yaw)) * cosf(radians(pitch));
    dir.y = sinf(radians(pitch));
    dir.z = sinf(radians(yaw)) * cosf(radians(pitch));
    dir = normalize(dir);
}

void Camera::setPosition(vec3 p)
{
    pos = p;
}

void Camera::setDirection(vec3 d)
{
    dir = d;
}

void Camera::setSensitivity(float s)
{
    sensitivity = s;
}

void Camera::setMovementSpeed(float s)
{
    speed = s;
}

mat4 Camera::getView() const
{
    return lookAt(pos, pos + dir, up);
}