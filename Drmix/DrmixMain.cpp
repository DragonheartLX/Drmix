#include <iostream>
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Drmix.hpp"

double framePerSecond = 60;
int winWidth = 1440;
int winHeigh = 1080;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char** argv)
{
    // Spdlog init
    spdlog::set_pattern("[%Y-%m-%d %T]%^[%l]%$ %v");
    printWorkPath();

    // glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* mainWin = glfwCreateWindow(winWidth, winHeigh, "Drmix", NULL, NULL);
    if (mainWin == NULL)
    {
        spdlog::warn("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    };
    glfwMakeContextCurrent(mainWin);
    glfwSetFramebufferSizeCallback(mainWin, framebuffer_size_callback);
    spdlog::info("GLFW window init.");

    // glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::warn("Failed to initialize GLAD");
        return -1;
    };
    spdlog::info("glad init.");

    framebuffer_size_callback(mainWin, winWidth, winHeigh);

    // Get OpenGL Version
    {
        unsigned char* glVersion;
        glVersion = (unsigned char*)glGetString(GL_VERSION);
        spdlog::info("Status: Using GL {0}", (char*)glVersion);
    }

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(mainWin, true);
    ImGui::StyleColorsDark();

    // 需要指定GLSL版本, 也就是shader中的version
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

    double vertices[] =
    {
        -0.5f,  -0.5f,  0.0f,
         0.5f,  -0.5f,  0.0f,
         0.0f,   0.5f,  0.0f
    };
    float clearColor[3] = { 0.2f, 0.3f, 0.3 };
    float color[3] = { 0.09f, 0.09f, 0.43f };

    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader shader("../../../Drmix/res/Shader/Basic.vert", "../../../Drmix/res/Shader/Basic.frag");

    // Main loop
    // Delta time
    double currentTime = glfwGetTime();
    double lastRenderTime = currentTime;
    double time = 0.0f;
    double delta = 0.0f;
    while (!glfwWindowShouldClose(mainWin))
    {
        // Delta time
        time = glfwGetTime();
        delta = time - currentTime;
        currentTime = time;

        // Event
        processInput(mainWin);

        if (1.0f / framePerSecond < time - lastRenderTime)
        {
            lastRenderTime = time;

            // Render
            glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::ColorEdit3("Clear color", clearColor);
            ImGui::ColorEdit3("Color", color);

            shader.enable();

            shader.setUniform4f("color", color[0], color[1], color[2], 1.0f);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(mainWin);
        }


        glfwPollEvents();
    };

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    spdlog::info("Drmix exit.");
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