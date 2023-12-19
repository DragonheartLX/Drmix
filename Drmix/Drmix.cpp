#include "Drmix.hpp"

#include <spdlog/spdlog.h>
#include <filesystem>

void printWorkPath()
{
	spdlog::info("WorkPath: {0}", std::filesystem::current_path().string());
}