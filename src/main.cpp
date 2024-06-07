#include "Drmix.h"
#include "Logger.h"
#include <cstdlib>
#include <exception>

int main() 
{
    DrmixApplication app;

    try 
    {
        app.run();
    } 
    catch(const std::exception& e) 
    {
        Logger::error(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}