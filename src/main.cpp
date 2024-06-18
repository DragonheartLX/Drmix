#include "Drmix.h"
#include "Logger.h"
#include <cstdlib>
#include <exception>

int main() 
{
    DrmixApplication app;

    Logger::info("Drmix Start");

    try 
    {
        app.run();
    } 
    catch(const std::exception& e) 
    {
        Logger::error(e.what());
        return EXIT_FAILURE;
    }

    Logger::info("Drmix Exit");

    return EXIT_SUCCESS;
}