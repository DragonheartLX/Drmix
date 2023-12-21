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

    Mixture* currentMixture = nullptr;
    MenuMixture* menu = new MenuMixture(currentMixture);
    currentMixture = menu;
    
    menu->registerMixture<BasicMixture>("Basic Nixture");
    menu->registerMixture<Texture2DMixture>("Texture2D Mixture");

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
            currentMixture->update(time - lastRenderTime);

            // Render
            glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::SetWindowFontScale(1.5f);
            
            if (currentMixture != menu && ImGui::Button("Back to menu"))
            {
                delete currentMixture;
                currentMixture = menu;
            }

            if (currentMixture != menu)
                ImGui::SameLine();
            if (ImGui::Button("Exit"))
                glfwSetWindowShouldClose(mainWin, GLFW_TRUE);

            currentMixture->imguiRender();

            currentMixture->render();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(mainWin);
        }


        glfwPollEvents();
    };

    delete currentMixture;

    if (menu != currentMixture)
        delete menu;
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    spdlog::info("Drmix exit.");
    return 0;
}