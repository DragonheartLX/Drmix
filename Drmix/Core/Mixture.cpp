#include "Core/Mixture.hpp"
#include "Core/Core.hpp"
#include "Core/Filesystem.hpp"
#include "Core/Logger.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace Drmix
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		DRMIX_LOG_INFO("Set viewport to: {0:d}x{1:d}", width, height);
		glViewport(0, 0, width, height);
	};

	Mixture::Mixture()
	{

	}

	Mixture::~Mixture()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();

		DRMIX_LOG_INFO("{0} exit.", info.name);
	}

	void Mixture::init()
	{
		// Spdlog init
		spdlog::set_pattern("[%Y-%m-%d %T]%^[%l]%$ %v");

		// glfw init
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		Filesystem::setResourcePath(info.resourcePath);
		DRMIX_LOG_INFO("WorkPath: {0}", Filesystem::getWorkDir());
		DRMIX_LOG_INFO("ResourcePath: {0}", Filesystem::getResourcePath());

		Drmix* drmix = Drmix::getInstance();
		drmix->mainWindow = glfwCreateWindow(info.winWidth, info.winHeight, info.name.c_str(), NULL, NULL);

		if (drmix->mainWindow == NULL)
		{
			DRMIX_LOG_ERROR("Failed to create GLFW window");
			glfwTerminate();
			exit(-1);
		}

		glfwMakeContextCurrent(drmix->mainWindow);
		glfwSwapInterval(1); // Enable vsync
		// glfwSetFramebufferSizeCallback(drmix->mainWindow, framebuffer_size_callback);
		DRMIX_LOG_INFO("GLFW window init.");

		// glad init
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			DRMIX_LOG_INFO("Failed to initialize GLAD");
			exit(-1);
		};
		DRMIX_LOG_INFO("glad init.");

		framebuffer_size_callback(drmix->mainWindow, info.winWidth, info.winHeight);

		// Get OpenGL Version
		{
			unsigned char* glVersion;
			glVersion = (unsigned char*)glGetString(GL_VERSION);
			DRMIX_LOG_INFO("Status: Using GL {0}", (char*)glVersion);
		}

		// ImGui init
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigViewportsNoAutoMerge = true;

		ImGui_ImplGlfw_InitForOpenGL(drmix->mainWindow, true);
		ImGui::StyleColorsDark();

		// 需要指定GLSL版本, 也就是shader中的version
		const char* glsl_version = "#version 330";
		ImGui_ImplOpenGL3_Init(glsl_version);

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		DRMIX_LOG_INFO("imGui init.");

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Mixture::run()
	{
		onEnter();

		// Main loop
		// Delta time
		double currentTime = glfwGetTime();
		double lastRenderTime = currentTime;
		double time = 0.0f;
		double delta = 0.0f;
		while (!info.mixtureShouldClose && !(glfwWindowShouldClose(Drmix::getInstance()->mainWindow) == GLFW_TRUE))
		{
			glfwPollEvents();

			// Delta time
			time = glfwGetTime();
			delta = time - currentTime;
			currentTime = time;

			// Event
			update();

			if (1.0f / info.framePerSecond < time - lastRenderTime)
			{
				lastRenderTime = time;
				fixedUpdate(time - lastRenderTime);

				// Render
				GLCall(glClearColor(0.16f, 0.16f, 0.16f, 1.0f));
				GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				if (info.imGuiRender)
				{
					ImGui::Begin(info.name.c_str(), &info.imGuiRender);
					ImGui::SetWindowFontScale(1.6f);
					imGuiRender();
					ImGui::End();
				}

				render();

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				ImGuiIO& io = ImGui::GetIO(); (void)io;

				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					GLFWwindow* backup_current_context = glfwGetCurrentContext();
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
					glfwMakeContextCurrent(backup_current_context);
				};

				glfwSwapBuffers(Drmix::getInstance()->mainWindow);
			}

		};
	}

	void Mixture::release()
	{

	}

	void Mixture::setFramebufferSize(int width, int height)
	{
		info.winWidth = width;
		info.winHeight = height;

		framebuffer_size_callback(Drmix::getInstance()->mainWindow, width, height);
	}

	double Mixture::getTime()
	{
		return glfwGetTime();
	}
}