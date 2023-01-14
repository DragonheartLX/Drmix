#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

using namespace std;

int winWidth = 1280;
int winHeight = 720;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char** argv)
{
    spdlog::set_pattern("[%Y-%m-%d %T]%^[%l]%$ %v");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWin = glfwCreateWindow(winWidth, winHeight, "Drmix", NULL, NULL);
    if (mainWin == NULL)
    {
        spdlog::warn("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    };
    glfwMakeContextCurrent(mainWin);
    glfwSetFramebufferSizeCallback(mainWin, framebuffer_size_callback);
    spdlog::info("GLFW window init.");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::warn("Failed to initialize GLAD");
        return -1;
    };
    spdlog::info("glad init.");
    framebuffer_size_callback(mainWin, winWidth, winHeight);

    while (!glfwWindowShouldClose(mainWin))
    {
        processInput(mainWin);

        glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWin);
        glfwPollEvents();
    };

    spdlog::info("Drmix exit.");
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    spdlog::info("Set viewport to: {0:d}x{1:d}", width, height);
    glViewport(0, 0, width, height);
};