#include "ForestView.h"

bool App::initialize_GLFW()
{
	if (!glfwInit()) return false;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return true;
}

bool App::create_window()
{
	window = glfwCreateWindow(900, 700, "ForestView", nullptr, nullptr);
	if (window == nullptr) 
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	return true;
}

bool App::initialize_GLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return false;
	}
	return true;
}

void App::initialize_ImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.BackendRendererName = "Task++";
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void App::render()
{
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark background
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
}

void App::clean_up()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void App::run_loop()
{
	if (!initialize_GLFW())
	{
		throw std::runtime_error("Failed to initialize GLFW.");
	}

	if (!create_window())
	{
		throw std::runtime_error("Failed to create GLFW Window / Context.");
	}

	if (!initialize_GLAD())
	{
		throw std::runtime_error("Failed to initialize GLAD pointer loader.");
	}

	initialize_ImGui();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		on_update();

		render();
	}
}
