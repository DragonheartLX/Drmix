#include <exception>
#include <spdlog/spdlog.h>

#include "Instance.h"

int main(int argc, char** argv)
{
    // _putenv_s("VK_ADD_LAYER_PATH", VK_VALIDATIONLAYERS);
    
    spdlog::set_pattern("%^[%H:%M:%S][%l] %v%$");

    spdlog::info("Logger init.");
    dm::DMInstance* instance = new dm::DMInstance();

    try
    {
        instance->init();
        spdlog::info("Instance init.");

        instance->mainLoop();
        instance->cleanup();
    } catch(const std::exception& e)
    {
        spdlog::error("Error: {0}", e.what());
        delete instance;

        spdlog::error("Exit.");
        return EXIT_FAILURE;
    }

    delete instance;
    spdlog::info("Exit.");
    return EXIT_SUCCESS;
}