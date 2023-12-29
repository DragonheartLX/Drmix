#include "Core/Core.hpp"
#include "Core/Logger.hpp"

#include <spdlog/spdlog.h>

namespace Drmix
{
	Drmix* Drmix::m_Instance = nullptr;

	Drmix::Drmix():
		mainWindow(NULL)
	{

	}

	Drmix::~Drmix()
	{
		delete m_Instance;
	}

	Drmix* Drmix::getInstance()
	{
		if (m_Instance == nullptr)
			m_Instance = new Drmix();
		return m_Instance;
	}
}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) 
	{
		DRMIX_LOG_WARN("[OpenGL Error] ({0}): {1} {2}:{3}", error, function, file, line);
		return false;
	}
	return true;
}
