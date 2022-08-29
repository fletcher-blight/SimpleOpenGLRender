#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <fmt/format.h>
#include <array>
#include <chrono>
#include <string_view>
#include <thread>

static void GLFWErrorHandler(int const errorCode, char const* const description) noexcept(false)
{
    throw std::runtime_error(fmt::format("GLFW Error ({}): {}", errorCode, description));
}

static void CompileShader(GLuint const shaderID, std::string_view const shaderCode)
{
    char const* shaderCodePtr = shaderCode.data();
    glShaderSource(shaderID, 1, &shaderCodePtr, nullptr);
    glCompileShader(shaderID);

    GLint result = GL_FALSE;
    int logLength = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength != 0)
    {
        auto errorMessage = std::string(logLength, '\0');
        glGetShaderInfoLog(shaderID, logLength, nullptr, errorMessage.data());
        throw std::runtime_error(fmt::format("Shader Compilation Failed ({}): {}", result, errorMessage));
    }
}

template <size_t N>
static void LinkShaders(GLuint const programID, std::array<GLuint, N> shaderIDs)
{
    for (GLuint const shaderID : shaderIDs)
        glAttachShader(programID, shaderID);

    glLinkProgram(programID);

    GLint result = GL_FALSE;
    int logLength = 0;
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength != 0)
    {
        auto errorMessage = std::string(logLength, '\0');
        glGetProgramInfoLog(programID, logLength, nullptr, errorMessage.data());
        throw std::runtime_error(fmt::format("Program Linking Failed ({}): {}", result, errorMessage));
    }

    for (GLuint const shaderID : shaderIDs) {
        glDetachShader(programID, shaderID);
        glDeleteShader(shaderID);
    }
}

static constexpr std::string_view vertexShaderCode = R"""(
#version 330 core

layout(location = 0) in vec3 modelSpace;
layout(location = 1) in vec3 vertexColour;

out vec3 fragmentColour;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(modelSpace, 1);
    fragmentColour = vertexColour;
}
)""";

static constexpr std::string_view fragmentShaderCode = R"""(
#version 330 core

in vec3 fragmentColour;
out vec3 colour;

void main() {
    colour = fragmentColour;
}
)""";

void run()
{
    glewExperimental = GL_TRUE;
    glfwSetErrorCallback(GLFWErrorHandler);
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    uint16_t const windowWidth = 1920;
    uint16_t const windowHeight = 1080;
    GLFWwindow* const window = glfwCreateWindow(windowWidth, windowHeight, "OpenGLTest", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (GLenum const res = glewInit(); res != GLEW_OK)
        throw std::runtime_error(reinterpret_cast<char const*>(glewGetErrorString(res)));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    GLuint const programID = glCreateProgram();
    GLuint const vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint const fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    CompileShader(vertexShaderID, vertexShaderCode);
    CompileShader(fragmentShaderID, fragmentShaderCode);
    LinkShaders(programID, std::array{ vertexShaderID, fragmentShaderID });

    GLint const mvpID = glGetUniformLocation(programID, "mvp");

    glm::mat4 const view = glm::lookAt(glm::vec3(0, 5, -20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 const projection = glm::perspective(glm::radians(75.f), static_cast<float>(windowWidth) / windowHeight, 0.1f, 100.f);

    constexpr std::array pyramidVertexData = {
        1.f, -1.f, 0.f,
        0.f, -1.f, 1.f,
        0.f, 1.f, 0.f,

        1.f, -1.f, 0.f,
        0.f, -1.f, -1.f,
        0.f, 1.f, 0.f,

        0.f, -1.f, 1.f,
        -1.f, -1.f, 0.f,
        0.f, 1.f, 0.f,

        -1.f, -1.f, 0.f,
        0.f, -1.f, -1.f,
        0.f, 1.f, 0.f,
    };

    constexpr std::array cubeVertexData = {
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    constexpr std::array colourBufferData = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f,
    };

    // Create Buffers

    GLuint cubeVertex;
    glGenBuffers(1, &cubeVertex);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData.data(), GL_STATIC_DRAW);

    GLuint pyramidVertex;
    glGenBuffers(1, &pyramidVertex);
    glBindBuffer(GL_ARRAY_BUFFER, pyramidVertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertexData), pyramidVertexData.data(), GL_STATIC_DRAW);

    GLuint colourBuffer;
    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colourBufferData), colourBufferData.data(), GL_STATIC_DRAW);

    // Draw To Screen

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    float rotationAngle = 0.f;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    while (glfwWindowShouldClose(window) == 0 && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        auto const frameStartTime = std::chrono::system_clock::now();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        rotationAngle += 1.f;
        float const scaleMag = (std::sin(glm::radians(rotationAngle)) * 3.f) + 4;
        float const zMag = (std::cos(glm::radians(rotationAngle)) * 10.f) + 5;

        glm::mat4 const scale = glm::scale(glm::mat4(1.f), glm::vec3(scaleMag, scaleMag, scaleMag));
        glm::mat4 const rotation = glm::rotate(glm::radians(rotationAngle), glm::vec3(0, 1, 0));

        glm::mat4 const cubeTrans = glm::translate(glm::mat4(1.f), glm::vec3(10, 0, zMag));
        glm::mat4 const pyramidTrans = glm::translate(glm::mat4(1.f), glm::vec3(-10, 0, zMag));

        {
            glm::mat4 const mvp = projection * view * (cubeTrans * rotation * scale);

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, cubeVertex);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, cubeVertexData.size() / 3);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }
        {
            glm::mat4 const mvp = projection * view * (pyramidTrans * rotation * scale);

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, pyramidVertex);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, pyramidVertexData.size() / 3);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }

        glfwSwapBuffers(window);

        auto const frameEndTime = std::chrono::system_clock::now();
        auto const frameDuration = frameEndTime - frameStartTime;
        static constexpr auto frameLimit = std::chrono::milliseconds(16);
        if (frameDuration < frameLimit)
            std::this_thread::sleep_for(frameLimit - frameDuration);

        glfwPollEvents();
    }

    glDeleteBuffers(1, &cubeVertex);
    glDeleteBuffers(1, &pyramidVertex);
    glDeleteBuffers(1, &colourBuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &vertexArrayID);
    glfwTerminate();
}

int main()
{
    try {
        run();
        return EXIT_SUCCESS;
    } catch (std::exception const& e) {
        fmt::print(stderr, "Fatal Death: {}\n", e.what());
    }
    return EXIT_FAILURE;
}