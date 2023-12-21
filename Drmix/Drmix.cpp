#include "Drmix.hpp"

#include <spdlog/spdlog.h>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void printWorkPath()
{
	spdlog::info("WorkPath: {0}", std::filesystem::current_path().string());
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    spdlog::info("Set viewport to: {0:d}x{1:d}", width, height);
    glViewport(0, 0, width, height);
};