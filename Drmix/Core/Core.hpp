#pragma once
#include "Core/Mixture.hpp"
#include "Core/Assert.hpp"
#include "OpenGL/Renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLCall(x) \
	GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

namespace Drmix
{
	class Drmix
	{
	public:
		Drmix();
		~Drmix();

		static Drmix* getInstance();
		GLFWwindow* mainWindow;
		OpenGL::Renderer* mainRenderer;
	private:
		static Drmix* m_Instance;
	};
}