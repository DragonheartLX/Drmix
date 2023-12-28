#include "Core/Mixture.hpp"
#include "Core/Core.hpp"

#include <filesystem>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace Drmix
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		spdlog::info("Set viewport to: {0:d}x{1:d}", width, height);
		glViewport(0, 0, width, height);
	};

	Mixture::Mixture():
		m_MixtureShouldClose(false),
		m_imGuiRender(false)
	{

	}

	Mixture::~Mixture()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();

		spdlog::info("{0} exit.", info.name);
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

		spdlog::info("WorkPath: {0}", std::filesystem::current_path().string());

		Drmix* drmix = Drmix::getInstance();
		drmix->mainWindow = glfwCreateWindow(info.winWidth, info.winHeight, info.name.c_str(), NULL, NULL);
		drmix->mainRenderer = new OpenGL::Renderer();

		if (drmix->mainWindow == NULL)
		{
			spdlog::error("Failed to create GLFW window");
			glfwTerminate();
			exit(-1);
		}

		glfwMakeContextCurrent(drmix->mainWindow);
		glfwSwapInterval(1); // Enable vsync
		// glfwSetFramebufferSizeCallback(drmix->mainWindow, framebuffer_size_callback);
		spdlog::info("GLFW window init.");

		// glad init
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			spdlog::warn("Failed to initialize GLAD");
			exit(-1);
		};
		spdlog::info("glad init.");

		framebuffer_size_callback(drmix->mainWindow, info.winWidth, info.winHeight);

		// Get OpenGL Version
		{
			unsigned char* glVersion;
			glVersion = (unsigned char*)glGetString(GL_VERSION);
			spdlog::info("Status: Using GL {0}", (char*)glVersion);
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
		spdlog::info("imGui init.");

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
		while (!m_MixtureShouldClose && !(glfwWindowShouldClose(Drmix::getInstance()->mainWindow) == GLFW_TRUE))
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

				if (m_imGuiRender)
				{
					ImGui::Begin(info.name.c_str(), &m_imGuiRender);
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

	void Mixture::render()
	{
		render(objects);
	};

	void Mixture::render(std::unordered_map<std::string, Node*> nodes)
	{
		if (nodes.empty())
			return;

		for (std::pair childNode: nodes)
		{
			if (childNode.second->getChildren().empty())
			{
				childNode.second->render();
			}
			else 
			{
				render(childNode.second->getChildren());
			}
		}
	};

	void Mixture::setFramebufferSize(int width, int height)
	{
		info.winWidth = width;
		info.winHeight = height;

		framebuffer_size_callback(Drmix::getInstance()->mainWindow, width, height);
	}

	void Mixture::setMixtureShouldClose(bool status)
	{
		m_MixtureShouldClose = status;
	}

	void Mixture::setRenderImGui(bool status)
	{
		m_imGuiRender = status;
	}

	double Mixture::getTime()
	{
		return glfwGetTime();
	}
}