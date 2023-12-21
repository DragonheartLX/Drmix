#pragma once

#include "Shader/Shader.hpp"
#include "Mixture/Mixture.hpp"
#include "Mixture/BasicMixture.hpp"
#include "Mixture/Texture2DMixture.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void printWorkPath();

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);