#pragma once

#include <cstdint>

struct GLFWwindow;

const uint32_t WIDTH = 1280;
const uint32_t HEIGHT = 720;

class DrmixApplication 
{
public:
    void run();
private:
    GLFWwindow* window;

    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();
};