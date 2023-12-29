#include "Core/Filesystem.hpp"

#include <filesystem>

namespace Drmix
{
	std::string Filesystem::m_ResourcePath = "./";

	std::string Filesystem::getWorkDir()
	{
		return std::filesystem::current_path().string();
	};

	std::string Filesystem::getResourcePath(std::string path)
	{
		return (std::filesystem::current_path() / std::filesystem::path(m_ResourcePath) / std::filesystem::path(path)).string();
	};

	void Filesystem::setResourcePath(std::string path)
	{
		m_ResourcePath = path;
	};
}