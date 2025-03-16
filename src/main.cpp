#include <iostream>
#include <exception>

#include "Instance.h"

int main(int argc, char** argv)
{
    dm::DMInstance* instance = new dm::DMInstance();

    try
    {
        instance->init();
        instance->mainLoop();
        instance->cleanup();
    } catch(std::exception e)
    {
        std::cerr << e.what() << std::endl;
        delete instance;
        return EXIT_FAILURE;
    }

    delete instance;

    return EXIT_SUCCESS;
}