#pragma once

struct GLFWwindow;

namespace dm
{
    class DMInstance
    {
    public:
        void init();
        void mainLoop();
        void cleanup();
    private:
        GLFWwindow* m_Window = nullptr;
    };
}