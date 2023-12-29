#pragma once
#include <string>

namespace Drmix
{
	class Filesystem
	{
	public:
		static std::string getWorkDir();
		static std::string getResourcePath(std::string path = "");

		static void setResourcePath(std::string path);
	private:
		static std::string m_ResourcePath;
	};
}