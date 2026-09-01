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
	glfwSetWindowSizeLimits(window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
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

	default_font = io.Fonts->AddFontFromFileTTF("assets/fonts/NotoSans-SemiBold.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	arabic_font = io.Fonts->AddFontFromFileTTF("assets/fonts/NotoSansArabic-SemiBold.ttf", 16.0f, nullptr);



	switch (m_activeTheme)
	{
	case Theme::DARK_MODE:
	{
		dark_mode();
		break;
	}
		
	case Theme::DARK_MODE_V2:
	{
		dark_mode_v2();
		break;
	}
	case Theme::LIGHT_MODE:
	{
		light_mode();
		break;
	}
	case Theme::TEST: 
	{
		test_mode();
		break;
	}
	default:
		break;
	}
	
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

void App::spacing_and_padding_and_stuff(ImGuiStyle& style)
{
	style.WindowPadding = ImVec2(10, 10);
	style.FramePadding = ImVec2(8, 5);
	style.ItemSpacing = ImVec2(6, 6);
	style.ItemInnerSpacing = ImVec2(6, 4);
	style.CellPadding = ImVec2(8, 6);
	style.IndentSpacing = 18.0f;
	style.ScrollbarSize = 13.0f;
	style.GrabMinSize = 10.0f;

	style.WindowRounding = 0.0f;
	style.FrameRounding = 5.0f;
	style.ScrollbarRounding = 5.0f;
	style.GrabRounding = 5.0f;
	style.TabRounding = 4.0f;
	style.PopupRounding = 4.0f;
	style.ChildRounding = 3.0f;
}

void App::dark_mode()
{
	ImGuiStyle& style = ImGui::GetStyle();

	spacing_and_padding_and_stuff(style);

	// Steel Blue Accent
	const ImVec4 Accent = ImVec4(0.38f, 0.60f, 0.95f, 1.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.48f, 0.52f, 0.60f, 1.00f);

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.07843138f, 0.08627451f, 0.10196079f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09411765f, 0.10196079f, 0.11764706f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07843138f, 0.08627451f, 0.10196079f, 1.00f);

	style.Colors[ImGuiCol_Border] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.11372549f, 0.12549020f, 0.15294118f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.17647059f, 0.19215687f, 0.22352941f, 1.00f);

	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04705882f, 0.05490196f, 0.07058824f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.04705882f, 0.05490196f, 0.07058824f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07843138f, 0.08627451f, 0.10196079f, 1.00f);

	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803922f, 0.10588235f, 0.12156863f, 1.00f);

	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.04705882f, 0.05490196f, 0.07058824f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.17647059f, 0.19215687f, 0.22352941f, 1.00f);

	style.Colors[ImGuiCol_CheckMark] = Accent;
	style.Colors[ImGuiCol_SliderGrab] = Accent;
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.48f, 0.70f, 1.00f, 1.00f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.18039216f, 0.18823530f, 0.19607843f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.20000000f, 0.21568628f, 0.23921569f, 1.00f);

	style.Colors[ImGuiCol_Header] = ImVec4(0.14117648f, 0.16470589f, 0.20784314f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.17647059f, 0.20000000f, 0.24313726f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.20000000f, 0.22745098f, 0.27450982f, 1.00f);

	style.Colors[ImGuiCol_Separator] = ImVec4(0.12941177f, 0.14901961f, 0.19215687f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = Accent;
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.48f, 0.70f, 1.00f, 1.00f);

	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.18f, 0.20f, 0.24f, 0.70f);
	style.Colors[ImGuiCol_ResizeGripHovered] = Accent;
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.48f, 0.70f, 1.00f, 1.00f);

	style.Colors[ImGuiCol_Tab] = ImVec4(0.07843138f, 0.08627451f, 0.10196079f, 1.00f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.14500000f, 0.16000000f, 0.19000000f, 1.00f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.16000000f, 0.18000000f, 0.22000000f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.07843138f, 0.08627451f, 0.10196079f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.12000000f, 0.14000000f, 0.17000000f, 1.00f);

	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.55f, 0.60f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = Accent;
	style.Colors[ImGuiCol_PlotHistogram] = Accent;
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.48f, 0.70f, 1.00f, 1.00f);

	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.04705882f, 0.05490196f, 0.07058824f, 1.00f);
	style.Colors[ImGuiCol_TableBorderStrong] = style.Colors[ImGuiCol_Border];
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);

	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09f, 0.10f, 0.12f, 0.45f);

	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.38f, 0.60f, 0.95f, 0.30f);

	style.Colors[ImGuiCol_DragDropTarget] = Accent;
	style.Colors[ImGuiCol_NavHighlight] = Accent;
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.75f, 0.82f, 1.00f, 0.80f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.55f);
}

void App::dark_mode_v2() 
{
	ImGuiStyle& style = ImGui::GetStyle();

	spacing_and_padding_and_stuff(style);

	ImVec4* colors = ImGui::GetStyle().Colors;

	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.96f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.58f, 0.58f, 0.60f, 1.00f);

	colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.12f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.13f, 0.14f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.15f, 1.00f);

	colors[ImGuiCol_Border] = ImVec4(0.24f, 0.24f, 0.25f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

	colors[ImGuiCol_FrameBg] = ImVec4(0.17f, 0.17f, 0.18f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);

	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);

	colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);

	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.28f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.37f, 0.37f, 0.38f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.48f, 0.48f, 0.49f, 1.00f);

	colors[ImGuiCol_CheckMark] = ImVec4(0.35f, 0.62f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.62f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.48f, 0.72f, 1.00f, 1.00f);

	colors[ImGuiCol_Button] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.24f, 0.25f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.30f, 0.31f, 1.00f);

	colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.24f, 0.25f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.30f, 0.31f, 1.00f);

	colors[ImGuiCol_Separator] = ImVec4(0.26f, 0.26f, 0.27f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.35f, 0.36f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.45f, 0.45f, 0.46f, 1.00f);

	colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.29f, 0.70f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.40f, 0.41f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.52f, 0.52f, 0.53f, 1.00f);

	colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.22f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);

	colors[ImGuiCol_PlotLines] = ImVec4(0.70f, 0.70f, 0.72f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.35f, 0.62f, 1.00f, 1.00f);

	colors[ImGuiCol_PlotHistogram] = ImVec4(0.85f, 0.78f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.95f, 0.88f, 0.45f, 1.00f);

	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.24f, 0.24f, 0.25f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.14f, 0.14f, 0.15f, 0.55f);

	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.35f, 0.62f, 1.00f, 0.35f);

	colors[ImGuiCol_DragDropTarget] = ImVec4(0.35f, 0.62f, 1.00f, 1.00f);

	colors[ImGuiCol_NavHighlight] = ImVec4(0.35f, 0.62f, 1.00f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.45f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
}

void App::light_mode()
{
	ImGuiStyle& style = ImGui::GetStyle();

	spacing_and_padding_and_stuff(style);

	// Steel Blue Accent
	const ImVec4 Accent = ImVec4(0.38f, 0.60f, 0.95f, 1.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.54f, 0.60f, 1.00f);

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.98f, 0.98f, 0.99f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

	style.Colors[ImGuiCol_Border] = ImVec4(0.78f, 0.80f, 0.84f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.91f, 0.93f, 0.96f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.86f, 0.89f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.82f, 0.86f, 0.92f, 1.00f);

	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.90f, 0.92f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.90f, 0.92f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);

	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.93f, 0.94f, 0.96f, 1.00f);

	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.90f, 0.91f, 0.93f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.72f, 0.75f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.62f, 0.66f, 0.72f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.52f, 0.57f, 0.64f, 1.00f);

	style.Colors[ImGuiCol_CheckMark] = Accent;
	style.Colors[ImGuiCol_SliderGrab] = Accent;
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.28f, 0.52f, 0.90f, 1.00f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.89f, 0.91f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.87f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.79f, 0.83f, 0.89f, 1.00f);

	style.Colors[ImGuiCol_Header] = ImVec4(0.88f, 0.91f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.81f, 0.86f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.74f, 0.81f, 0.92f, 1.00f);

	style.Colors[ImGuiCol_Separator] = ImVec4(0.78f, 0.80f, 0.84f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = Accent;
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.28f, 0.52f, 0.90f, 1.00f);

	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.72f, 0.75f, 0.80f, 0.70f);
	style.Colors[ImGuiCol_ResizeGripHovered] = Accent;
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.28f, 0.52f, 0.90f, 1.00f);

	style.Colors[ImGuiCol_Tab] = ImVec4(0.90f, 0.92f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.88f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.80f, 0.85f, 0.93f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.92f, 0.94f, 0.97f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.86f, 0.89f, 0.95f, 1.00f);

	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.45f, 0.50f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = Accent;
	style.Colors[ImGuiCol_PlotHistogram] = Accent;
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.28f, 0.52f, 0.90f, 1.00f);

	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.90f, 0.92f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_TableBorderStrong] = style.Colors[ImGuiCol_Border];
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.86f, 0.88f, 0.91f, 1.00f);

	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.95f, 0.96f, 0.98f, 0.60f);

	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.38f, 0.60f, 0.95f, 0.30f);

	style.Colors[ImGuiCol_DragDropTarget] = Accent;
	style.Colors[ImGuiCol_NavHighlight] = Accent;
	style.Colors[ImGuiCol_NavWindowingHighlight] = Accent;
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.12f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
}

void App::test_mode()
{
	ImGuiStyle& style = ImGui::GetStyle();

	spacing_and_padding_and_stuff(style);

}

void App::change_theme(const Theme& theme)
{
	if (theme == m_activeTheme) return;
	m_activeTheme = theme;
	switch (m_activeTheme)
	{
	case Theme::DARK_MODE:
	{
		dark_mode();
		break;
	}

	case Theme::DARK_MODE_V2:
	{
		dark_mode_v2();
		break;
	}
	case Theme::LIGHT_MODE:
	{
		light_mode();
		break;
	}
	case Theme::TEST: 
	{
		test_mode();
		break;
	}
	default:
		break;
	}
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

	on_start();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();

		on_update();

		render();
	}

	on_shutdown();
}

void ForestView::render_msg()
{
	if (!tabs[current_tab]->message.empty())
	{
		if (tabs[current_tab]->message == "Task added successfuly") ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), tabs[current_tab]->message.c_str());
		else if (tabs[current_tab]->message == "Task edtied successfuly")  ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), tabs[current_tab]->message.c_str());
		else if (tabs[current_tab]->message == "Added dependencies successfuly") ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), tabs[current_tab]->message.c_str());
		else if (tabs[current_tab]->message == "Removed dependencies successfuly") ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), tabs[current_tab]->message.c_str());
		else ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), tabs[current_tab]->message.c_str());
	}
}

void ForestView::on_start()
{
	M_texturesManager.load_images();
	tabs[current_tab]->fileExplorer = std::make_unique<Explorer>(M_texturesManager);
	tabs[current_tab]->entries = tabs[current_tab]->fileExplorer->get_entries();

	tabs[current_tab]->t.iniThisPc("THIS PC");
	tabs[current_tab]->t.fillThisPcWithDrives(tabs[current_tab]->fileExplorer->get_partitions());

}

// Main update function (each frame gets called)
void ForestView::on_update()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(800.0f, 600.0f), ImVec2(FLT_MAX, FLT_MAX));
	if (ImGui::Begin("ForestView"))
	{
		refocus_reload();

		tab_bar();

		search_bar();

		tool_bar();

		float footerHeight = 40.0f + ImGui::GetStyle().FramePadding.y * 2.0f;
		float remainingHeight = ImGui::GetContentRegionAvail().y - footerHeight;
		if (remainingHeight < 0.0f)
		{
			remainingHeight = 0.0f;
		}

		folders_tree(remainingHeight);

		ImGui::SameLine();

		files_navigation_panel(remainingHeight);

		stats_bar();

		if (show_icons_slider) icons_size_slider();

		if (!tasks.empty())
		{
			for (size_t i = 0; i < tasks.size(); i++)
			{
				if (tasks[i]->task_type == TaskType::COPY_PASTE)
				{
					if (!tasks[i]->finished)
					{
						ImGui::SetNextWindowSizeConstraints(ImVec2(350.0f, 0.0f), ImVec2(600.0f, FLT_MAX));

						if (ImGui::Begin("Copying", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
						{
							const int files_count_progress = tasks[i]->filesCountProgress.load();
							const int files_total_count = tasks[i]->totalfilesCount;

							// Some path info (From "Path1" to "Path2") =====================================================
							ImGui::Text(std::format("Coping {} items from {} to {}", files_total_count, tasks[i]->sourceName, tasks[i]->destinationName).c_str());
							// ==============================================================================================

							ImGui::Separator();

							const float progressPercentage = tasks[i]->sizeProgress / tasks[i]->totalSize;

							// Percecntage completed =======================================================================
							ImGui::Text("%s", std::format("{:.0f}% Completed", progressPercentage * 100).c_str());
							// =============================================================================================

							const double size_progress = tasks[i]->sizeProgress;
							const double size_total = tasks[i]->totalSize;

							fileSize data = tabs[current_tab]->fileExplorer->decide_file_size_format(size_progress);
							fileSize data2 = tabs[current_tab]->fileExplorer->decide_file_size_format(size_total);

							// Number bytes (KB,MB,GB) copied =====================================================
							ImGui::Text(std::format("Copied {:.2f} {} / {:.2f} {}", data.size, data.label, data2.size, data2.label).c_str());

							// Number of files copied ============================================================
							ImGui::Text(std::format("Copied {} Files / {} Files", files_count_progress, files_total_count).c_str());


							// Speed calculations and UI ==========================================================
							tasks[i]->end = std::chrono::steady_clock::now();

							double duration = std::chrono::duration<double>(tasks[i]->end - tasks[i]->start).count();

							if (duration >= 0.5)
							{
								double bytesInThisSliceOfTime = size_progress - tasks[i]->sizeProgressTemp;

								tasks[i]->currentSpeed = (bytesInThisSliceOfTime / 1048576.0) / duration;

								// To reset after every 0.5 seconds so the speed will be acuurate not just accumulate over time 
								tasks[i]->sizeProgressTemp = size_progress;
								tasks[i]->start = tasks[i]->end;
							}

							ImGui::Text(std::format("Speed: {:.2f} MB/S", tasks[i]->currentSpeed).c_str());

							// Progress Bar ===================================================
							ImGui::Separator();
							ImGui::ProgressBar(progressPercentage, ImVec2(500.00f, 50.0f), "");
							ImGui::Separator();
							// ================================================================

							const fileSize data3 = tabs[current_tab]->fileExplorer->decide_file_size_format(size_total - size_progress);

							// Number of remaining files to copy =============================
							ImGui::Text(std::format("Items Remaining: {} ({:.2f} {})", files_total_count - files_count_progress, data3.size, data3.label).c_str());
							// ===============================================================

							ImGui::SameLine();

							ImGui::Text("   |   ");

							ImGui::SameLine();

							// Canceling Button =============================================
							ImGui::PushID(i);
							if (tasks[i]->is_closing)
							{
								ImGui::Text("Canceling....");
								tasks[i]->cancel_request = true;
							}
							else
							{
								if (ImGui::Button("X"))
								{
									tasks[i]->is_closing = true;
								}
							}
							ImGui::PopID();


							if (i + 1 != tasks.size()) ImGui::Separator();


						}
						ImGui::End();
					}
					else if (tasks[i]->finished)
					{
						if (tasks[i]->worker.joinable())
						{
							tasks[i]->worker.join();
						}
						tasks.erase(tasks.begin() + i);
						if (i > 0) i--;
						else i = 0;
						tabs[current_tab]->tree_need_update = true;
						tabs[current_tab]->entries_need_update = true;
					}
				}
				else if (tasks[i]->task_type == TaskType::_DELETE)
				{
					if (!tasks[i]->finished)
					{
						ImGui::SetNextWindowSizeConstraints(ImVec2(350.0f, 0.0f), ImVec2(600.0f, FLT_MAX));
						if (ImGui::Begin("Deleting", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
						{
							ImGui::Text(std::format("Deleting {}", tasks[i]->sourceName).c_str());
						}
						ImGui::End();
					}
					else if (tasks[i]->finished)
					{
						if (tasks[i]->worker.joinable())
						{
							tasks[i]->worker.join();
						}
						tasks.erase(tasks.begin() + i);
						if (i > 0) i--;
						else i = 0;
						tabs[current_tab]->tree_need_update = true;
						tabs[current_tab]->entries_need_update = true;
					}

				}

			}

		}

		// Empty Folders task UI
		if (m_emptyFoldersTask != nullptr)
		{
			if (!m_emptyFoldersTask->cancel_request)
			{
				is_searching_for_empty_folders = true;
				if (ImGui::Begin(std::format("Empty Folders in {}", m_emptyFoldersTask->sourceName).c_str()))
				{
					// Tool Bar for the empty folders panel
					if (ImGui::Button("Cancel"))
					{
						m_emptyFoldersTask->cancel_request = true;
					}

					ImGui::SameLine();

					ImGui::Text(std::format(" | Number of Empty Folders: {}", (int)m_emptyFolders.size()).c_str());

					ImGui::Separator();

					if (!m_emptyFoldersTask->finished)
					{
						ImGui::Text("Searching For Empty Folders...");
						ImGui::Separator();
					}

					if (ImGui::BeginTable("EmptyFoldersTable", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
					{
						ImGui::TableSetupColumn("Folder Path");
						ImGui::TableHeadersRow();

						ImGuiListClipper EmptyFoldersClipper;

						EmptyFoldersClipper.Begin((int)m_emptyFolders.size());

						while (EmptyFoldersClipper.Step()) 
						{
							for (size_t j = EmptyFoldersClipper.DisplayStart; j < EmptyFoldersClipper.DisplayEnd; j++)
							{
								ImGui::PushID(j);
								
								ImGui::TableNextRow();
								
								ImGui::TableNextColumn();

								ImGui::Selectable(m_emptyFolders[j].string().c_str());

								handle_entries_mouse_click(fs::directory_entry(m_emptyFolders[j]));
								ImGui::PopID();
							}
						}

						ImGui::EndTable();
					}
				}
				ImGui::End();
			}
			else if (m_emptyFoldersTask->finished)
			{
				is_searching_for_empty_folders = false;
				if (m_emptyFoldersTask->worker.joinable())
				{
					m_emptyFoldersTask->worker.join();
				}
				m_emptyFoldersTask = nullptr;
				m_emptyFolders.clear();
				tabs[current_tab]->selected_path.clear();
			}
		}

		// Duplicate Files UI
		if (m_duplicateFilesTask != nullptr)
		{
			if (m_duplicateFilesTask->cancel_request == false) 
			{
				is_searching_for_duplicates_files = true;

				if (ImGui::Begin(std::format("Finding Duplicates in {}", m_duplicateFilesTask->sourceName).c_str() )) 
				{
					// Duplicates tool bar ==========================================================================
					if (ImGui::Button("Cancel")) 
					{
						m_duplicateFilesTask->cancel_request = true;
					}

					ImGui::SameLine();

					ImGui::Text(std::format(" | Number of Duplicates: {}", (int)m_duplicateFiles.size()).c_str());
					
					if (!m_duplicateFilesTask->finished) 
					{
						ImGui::Separator();
						ImGui::Text("Searching for duplicate files...");
					}
					ImGui::Separator();
				
					// Rendering the Duplicates =====================================================================
					if (ImGui::BeginTable("DuplicatesTaskTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
					{
						ImGui::TableSetupColumn("File Path");
						ImGui::TableSetupColumn("File Size");
						ImGui::TableHeadersRow();

						ImGuiListClipper DuplicatesClipper;
						DuplicatesClipper.Begin(m_duplicateFiles.size());
						while (DuplicatesClipper.Step()) 
						{
							for (size_t i = DuplicatesClipper.DisplayStart ; i < DuplicatesClipper.DisplayEnd; ++i) 
							{
								ImGui::PushID(i);
								ImGui::TableNextRow();

								const fs::directory_entry& duplicate = m_duplicateFiles[i].duplicateEntry;

								bool is_duplicate_selected = (duplicate.path() == tabs[current_tab]->selected_path) ? true : false;

								ImGui::TableNextColumn();
								std::u8string utf8_path = duplicate.path().u8string();
#if defined(__cpp_lib_char8_t)
								std::string display_path(utf8_path.begin(), utf8_path.end());
#else
								std::string display_path = utf8_path;
#endif
								std::string label = display_path + "##" + std::to_string(i);

								ImGui::Selectable(label.c_str(), &is_duplicate_selected);

								// The mouse clicks behaviour ================================================================
								handle_entries_mouse_click(duplicate);

								// To add a seprator after the last duplicate in the group ===================================
								ImGui::TableNextColumn();
								const fileSize DuplicateSizeInfo = tabs[current_tab]->fileExplorer->decide_file_size_format(duplicate.file_size());
								ImGui::Text(std::format("{:.2f} {}", DuplicateSizeInfo.size, DuplicateSizeInfo.label).c_str());

								if (i + 1 < m_duplicateFiles.size() && m_duplicateFiles[i + 1].groupId != m_duplicateFiles[i].groupId)
								{
							
								}
								ImGui::PopID();
							}
						}

						ImGui::EndTable();
					}
				}
				ImGui::End();

			}
			else if (m_duplicateFilesTask->finished == true) 
			{
				is_searching_for_duplicates_files = false;
				if (m_duplicateFilesTask->worker.joinable()) 
				{
					m_duplicateFilesTask->worker.join();
				}
				m_duplicateFilesTask = nullptr;
				m_duplicateFiles.clear();
				tabs[current_tab]->selected_path.clear();
			}
		}
		
		// Largest Files UI
		if (m_largestFilesTask != nullptr) 
		{
			if (m_largestFilesTask->cancel_request == false) 
			{
				is_searching_for_largest_files = true;
				if (ImGui::Begin("Largest Files Window"))
				{
					// Largest Files Tool bar
					if (ImGui::Button("Cancel"))
					{
						m_largestFilesTask->cancel_request = true;
					}
					
					ImGui::SameLine();

					ImGui::Text(std::format(" | Number of Largest Files: {}", (int)m_largestFiles.size()).c_str());

					if (!m_largestFilesTask->finished)
					{
						ImGui::Separator();
						ImGui::Text("Searching for the largest files...");
					}

					ImGui::Separator();

					if (ImGui::BeginTable("LargestFilesTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
					{
						ImGui::TableSetupColumn("File Path");
						ImGui::TableSetupColumn("File Size");
						ImGui::TableHeadersRow();

						ImGuiListClipper LargestFilesClipper;
						LargestFilesClipper.Begin(m_largestFiles.size());
						while (LargestFilesClipper.Step())
						{
							for (int i = LargestFilesClipper.DisplayStart; i < LargestFilesClipper.DisplayEnd; ++i)
							{
								ImGui::PushID(i);

								ImGui::TableNextRow();

								// File path ================================
								ImGui::TableNextColumn();
								std::u8string utf8_path = m_largestFiles[i].path().u8string();
#if defined(__cpp_lib_char8_t)
								std::string display_path(utf8_path.begin(), utf8_path.end());
#else
								std::string display_path = utf8_path;
#endif
								std::string label = display_path + "##" + std::to_string(i);
								ImGui::Selectable(label.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);

								// The mouse clicks behaviour ================================================================
								handle_entries_mouse_click(m_largestFiles[i]);

								// File Size =================================
								ImGui::TableNextColumn();
								fileSize fileDataInfo = tabs[current_tab]->fileExplorer->decide_file_size_format(m_largestFiles[i].file_size());
								ImGui::Text(std::format("{:.2f} {}", fileDataInfo.size, fileDataInfo.label).c_str());

								ImGui::PopID();
							}
						}

						ImGui::EndTable();
					}

					
				}
				ImGui::End();	
			}
			else if (m_largestFilesTask->finished == true) 
			{
				is_searching_for_largest_files = false;
				if (m_largestFilesTask->worker.joinable()) 
				{
					m_largestFilesTask->worker.join();
				}
				m_largestFilesTask.reset();
				m_largestFiles.clear();
				tabs[current_tab]->selected_path.clear();
			}
		}

		decide_correct_rightClick_popup();

		access_denied();
	}
		ImGui::End();
}

void ForestView::refocus_reload()
{
	GLFWwindow* native_window = (GLFWwindow*)ImGui::GetMainViewport()->PlatformHandle;
	bool is_focused = glfwGetWindowAttrib(native_window, GLFW_FOCUSED) != 0;
	static bool was_focused = true;
	if (!is_focused)
	{
		was_focused = false;
	}

	if (is_focused && !was_focused)
	{
		if (tabs[current_tab]->is_searching)
		{
			tabs[current_tab]->entries = tabs[current_tab]->fileExplorer->search(tabs[current_tab]->current_path, tabs[current_tab]->ThisDirectoryKeyWord);
		}
		else 
		{
			tabs[current_tab]->entries_need_update = true;
			was_focused = true;
		}
	}
}

void ForestView::on_shutdown()
{
	shutdown_background_tasks();
}

// Tab Bar
void ForestView::tab_bar()
{
	ImGui::BeginChild("tabs", ImVec2(0, 40), ImGuiChildFlags_Borders);

	ImGui::BeginTabBar("ExplorerTabs");
	for (size_t i = 0; i < (int)tabs.size(); ++i)
	{
		std::string tabLabel = tabs[i]->utf8_search_bar_hint + "###tab" + std::to_string(tabs[i]->tabID);

		// Reselect the tab (if needed) ==============================================================
		ImGuiTabItemFlags flags;
		if (i == (int)open_new_tab_factor)
		{
			flags = ImGuiTabItemFlags_SetSelected;
			open_new_tab_factor = -1;
		}
		else
		{
			flags = ImGuiTabItemFlags_None;
		}

		// The Tabs itself ===========================================================================
		if (ImGui::BeginTabItem(tabLabel.c_str(), nullptr, flags))
		{
			current_tab = (int)i;
			ImGui::EndTabItem();
		}

		// Drag And Drop =============================================================================
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("TAP", &i, sizeof(int));
			ImGui::Text(("Draging tab: " + tabs[i]->utf8_search_bar_hint).c_str());

			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TAP"))
			{
				if (payload->IsDelivery())
				{
					drop_source = *(const int*)payload->Data;

					drop_target = i;
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Close Tab Logic =========================================================================== 
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Middle))
		{
			if ((current_tab == i && current_tab == (int)tabs.size() - 1) && (int)tabs.size() != 1)
			{
				tabs.pop_back();
				current_tab = tabs.size() - 1;

			}
			else
			{
				if (current_tab == i)
				{
					open_new_tab_factor = i;
				}
				if ((int)tabs.size() == 1)
				{
					continue;
				}
				tabs.erase(tabs.begin() + i);
				--i;
			}

		}
	}
	ImGui::EndTabBar();

	// Drag And Drop Logic ===========================================================================
	if (drop_source != -1 && drop_target != -1)
	{
		// Remove the source tab and reinsert it in the correct index to control visual order only 
		ImGuiTabBar* tab_bar = ImGui::TabBarFindByID(ImGui::GetID("ExplorerTabs"));
		if (tab_bar && drop_source < tab_bar->Tabs.Size && drop_target < tab_bar->Tabs.Size)
		{
			ImGuiTabItem moved = tab_bar->Tabs[drop_source];
			tab_bar->Tabs.erase(tab_bar->Tabs.Data + drop_source);
			tab_bar->Tabs.insert(tab_bar->Tabs.Data + drop_target, moved);
		}

		// Reoreder the main vector of tabs logic
		std::unique_ptr dragged_tab = std::move(tabs[drop_source]);

		if (drop_target < drop_source)
		{
			for (size_t j = drop_source; j > drop_target; j--)
			{
				tabs[j] = std::move(tabs[j - 1]);
			}
			tabs[drop_target] = std::move(dragged_tab);
			// ___________
			// |         |			              
			// V	     |							
	  //Tab1 Tab2 Tab3 Tab4
		}
		else if (drop_target > drop_source)
		{
			for (size_t j = drop_source; j < drop_target; j++)
			{
				tabs[j] = std::move(tabs[j + 1]);
			}
			tabs[drop_target] = std::move(dragged_tab);
			// __________
			// |        |			              
			// |	    V							
	  //Tab1 Tab2 Tab3 Tab4
		}
		current_tab = drop_target;
		open_new_tab_factor = drop_target;
		drop_source = -1;
		drop_target = -1;
	}

	ImGui::SameLine();
	if (ImGui::Button("+"))
	{
		std::unique_ptr<State> newTab = std::make_unique<State>();
		if (m_openTabType == NEW)
		{
			tabs.emplace_back(open_new_tab(newTab));
		}
		else if (m_openTabType == COPY)
		{
			tabs.emplace_back(open_copy_tab(newTab));
		}
		current_tab = (int)tabs.size() - 1;
		open_new_tab_factor = current_tab;
	}

	if (ImGui::BeginPopupContextItem("##openTapTyoe"))
	{
		if (ImGui::Button("New Tab"))
		{
			m_openTabType = NEW;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Copy Current Tab"))
		{
			m_openTabType = COPY;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::EndChild();
}

std::unique_ptr<State> ForestView::open_new_tab(std::unique_ptr<State>& newTab)
{
	newTab->fileExplorer = std::make_unique<Explorer>(M_texturesManager);
	newTab->entries = newTab->fileExplorer->get_entries();
	newTab->t.iniThisPc("THIS PC");
	newTab->t.fillThisPcWithDrives(newTab->fileExplorer->get_partitions());

	return std::move(newTab);
}

std::unique_ptr<State> ForestView::open_copy_tab(std::unique_ptr<State>& newTab)
{
	newTab->fileExplorer = std::make_unique<Explorer>(M_texturesManager);
	newTab->fileExplorer->set_undo_stack(tabs[current_tab]->fileExplorer->get_undo_stack());
	newTab->fileExplorer->set_redo_stack(tabs[current_tab]->fileExplorer->get_redo_stack());
	newTab->current_path = tabs[current_tab]->current_path;
	newTab->entries = tabs[current_tab]->entries;
	newTab->fav = tabs[current_tab]->fav;
	newTab->icons_height = tabs[current_tab]->icons_height;
	newTab->icons_width = tabs[current_tab]->icons_width;
	newTab->padding = tabs[current_tab]->padding;
	newTab->recents = tabs[current_tab]->recents;
	newTab->utf8_search_bar_hint = tabs[current_tab]->utf8_search_bar_hint;
	newTab->view_type = tabs[current_tab]->view_type;
	newTab->t.iniThisPc("THIS PC");
	newTab->t.fillThisPcWithDrives(newTab->fileExplorer->get_partitions());

	return std::move(newTab);
}


// Search Bar
void ForestView::search_bar()
{
	if (ImGui::BeginChild("SearchBar", ImVec2(0, 40), ImGuiChildFlags_Borders))
	{
		// Navigation
		static int ids = 0;
		if (tabs[current_tab]->global_key_need_update)
		{
			tabs[current_tab]->globalKeyWord = tabs[current_tab]->current_path.string();
			tabs[current_tab]->global_key_need_update = false;
			ids++;
			ImGui::SetKeyboardFocusHere();
		}
		ImGui::SetNextItemWidth(550.0f);

		ImGui::PushID(ids);
		if (ImGui::InputTextWithHint("##Global Search Bar", tabs[current_tab]->fileExplorer->form_breadcrumb_navigation_string(tabs[current_tab]->current_path).c_str(), &tabs[current_tab]->globalKeyWord) || !tabs[current_tab]->globalKeyWord.empty())
		{
			tabs[current_tab]->ThisDirectoryKeyWord = "";
			tabs[current_tab]->fileExplorer->refresh(tabs[current_tab]->current_path);
			if (ImGui::IsKeyReleased(ImGuiKey_Enter))
			{
				fs::path path_(tabs[current_tab]->globalKeyWord);
				if (fs::is_directory(path_) && fs::exists(path_))
				{
					DirAccessResult checkAccess = tabs[current_tab]->fileExplorer->check_directory_access(path_);
					if (checkAccess == DirAccessResult::ACCESS_DENIED) 
					{
						need_access_denied_popup = true;
						tabs[current_tab]->entries_need_update = true;
						tabs[current_tab]->globalKeyWord = "";
					}
					else 
					{
						tabs[current_tab]->fileExplorer->navigate_to(path_, tabs[current_tab]->current_path);
						tabs[current_tab]->entries_need_update = true;
						tabs[current_tab]->current_path = path_;
						tabs[current_tab]->globalKeyWord = "";
					}

				}
				else
				{
					tabs[current_tab]->globalKeyWord = "";
				}
			}
		}
		ImGui::PopID();

		if (ImGui::IsItemActivated() && tabs[current_tab]->globalKeyWord.empty())
		{
			if (!tabs[current_tab]->current_path.empty())
			{
				tabs[current_tab]->global_key_need_update = true;
			}

		}
		if (ImGui::IsItemDeactivated() && tabs[current_tab]->globalKeyWord == tabs[current_tab]->current_path.string())
		{
			tabs[current_tab]->globalKeyWord = "";
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(-1.0f);

		// Normal search bar
		ImGui::BeginDisabled(tabs[current_tab]->current_path.empty());

		if (ImGui::InputTextWithHint("##Search Bar", std::format("Search {}", tabs[current_tab]->utf8_search_bar_hint).c_str(), &tabs[current_tab]->ThisDirectoryKeyWord) || !tabs[current_tab]->ThisDirectoryKeyWord.empty())
		{
			tabs[current_tab]->entries = tabs[current_tab]->fileExplorer->search(tabs[current_tab]->current_path, tabs[current_tab]->ThisDirectoryKeyWord);
			tabs[current_tab]->globalKeyWord = "";
		}
		ImGui::EndDisabled();

		if (!tabs[current_tab]->ThisDirectoryKeyWord.empty())
		{
			tabs[current_tab]->is_searching = true;
		}
		else 
		{
			tabs[current_tab]->is_searching = false;
		}
	}
	ImGui::EndChild();
}

// Tool Bar
void ForestView::tool_bar()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 7.0f));
	if (ImGui::BeginChild("ToolBar", ImVec2(0, 40), ImGuiChildFlags_Borders))
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::ImageButton("##<--", M_texturesManager.get_backwards_button_icon_texture_id(), ImVec2(15.0f, 15.0f)) || ImGui::IsKeyPressed(ImGuiKey_MouseX1, false))
		{
			tabs[current_tab]->show_selected_folder_options = false;
			tabs[current_tab]->ThisDirectoryKeyWord = "";
			tabs[current_tab]->globalKeyWord = "";
			tabs[current_tab]->selected_path = "";
			tabs[current_tab]->fileExplorer->back(tabs[current_tab]->current_path);
			tabs[current_tab]->recents.visit(fs::directory_entry(tabs[current_tab]->current_path));
			tabs[current_tab]->entries_need_update = true;

		}
		if (!ImGui::GetIO().WantCaptureKeyboard) 
		{
			if (ImGui::IsKeyPressed(ImGuiKey_Backspace, false)) 
			{
				tabs[current_tab]->show_selected_folder_options = false;
				tabs[current_tab]->ThisDirectoryKeyWord = "";
				tabs[current_tab]->globalKeyWord = "";
				tabs[current_tab]->selected_path = "";
				tabs[current_tab]->fileExplorer->back(tabs[current_tab]->current_path);
				tabs[current_tab]->recents.visit(fs::directory_entry(tabs[current_tab]->current_path));
				tabs[current_tab]->entries_need_update = true;
			}
		}


		ImGui::SameLine();

		ImGui::BeginDisabled(!tabs[current_tab]->fileExplorer->can_go_forward());

		if (ImGui::ImageButton("##-->", M_texturesManager.get_forward_button_icon_texture_id(), ImVec2(15.0f, 15.0f)) || ImGui::IsKeyPressed(ImGuiKey_MouseX2, false))
		{
			tabs[current_tab]->show_selected_folder_options = false;
			tabs[current_tab]->ThisDirectoryKeyWord = "";
			tabs[current_tab]->globalKeyWord = "";
			tabs[current_tab]->selected_path = "";
			tabs[current_tab]->fileExplorer->redo(tabs[current_tab]->current_path);
			tabs[current_tab]->recents.visit(fs::directory_entry(tabs[current_tab]->current_path));
			tabs[current_tab]->entries_need_update = true;
		}

		ImGui::EndDisabled();
		ImGui::PopStyleColor(1);

		ImGui::SameLine();

		if (ImGui::Button("Refresh"))
		{
			tabs[current_tab]->selected_path = "";
			tabs[current_tab]->entries_need_update = true;
			tabs[current_tab]->globalKeyWord = "";
			tabs[current_tab]->ThisDirectoryKeyWord = "";
			tabs[current_tab]->tree_need_update = true;
		}

		ImGui::SameLine();

		ImGui::BeginDisabled(tabs[current_tab]->current_path.empty());
		if (ImGui::Button("New"))
		{
			ImGui::OpenPopup("New?");
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(tabs[current_tab]->selected_path.empty() || tabs[current_tab]->selected_path == tabs[current_tab]->selected_path.root_path());
		if (ImGui::Button("Delete"))
		{
			need_delete_confirmation = true;
			tabs[current_tab]->deleted_path = tabs[current_tab]->selected_path;
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(tabs[current_tab]->selected_path.empty() || tabs[current_tab]->selected_path == tabs[current_tab]->selected_path.root_path());
		if (ImGui::Button("Copy"))
		{
			tabs[current_tab]->copied_path = tabs[current_tab]->selected_path;
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(tabs[current_tab]->copied_path.empty());
		if (ImGui::Button("Paste"))
		{
			fs::path src = tabs[current_tab]->copied_path;
			fs::path dest = tabs[current_tab]->current_path;

			EntryDataReturn data = tabs[current_tab]->fileExplorer->get_total_files_size(tabs[current_tab]->copied_path);

			std::unique_ptr<Task> copyTask = std::make_unique<Task>();
			copyTask->totalSize = data.directorySize;
			copyTask->totalfilesCount = data.filesCount;
			copyTask->source = src;
			copyTask->destination = dest;
			copyTask->task_type = TaskType::COPY_PASTE;
			copyTask->sourceName = Utils::reform_string(src.filename().string(), 10);
			copyTask->destinationName = Utils::reform_string(dest.filename().string(), 10);

			Task* task = copyTask.get();

			copyTask->start = std::chrono::steady_clock::now();

			copyTask->worker = std::thread([this,src, dest, task]()
				{
					tabs[current_tab]->fileExplorer->copy_paste(src, dest, task->sizeProgress, task->filesCountProgress,task->cancel_request ,task->finished);
				});

			tasks.emplace_back(std::move(copyTask));

			tabs[current_tab]->copied_path.clear();
		}
		ImGui::EndDisabled();

		ImGui::SameLine();
		ImGui::BeginDisabled(tabs[current_tab]->current_path.empty());
		if (ImGui::Button("Sort"))
		{
			ImGui::OpenPopup("Sort Type");
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(tabs[current_tab]->current_path.empty());
		if (ImGui::Button("Filter")) 
		{
			ImGui::OpenPopup("FilterByOptions");
		}
		ImGui::EndDisabled();
		if (ImGui::BeginItemTooltip())
		{
			std::string filter_by_type = "Entries being filtered by: ";
			switch (tabs[current_tab]->fileExplorer->get_filter_by())
			{
			case FilterOptions::NONE: 
			{
				filter_by_type += "None";
				break;
			}
			case FilterOptions::ALL:
			{
				filter_by_type += "All";
				break;
			}
			case FilterOptions::FOLDERS:
			{
				filter_by_type += "Folders";
				break;
			}
			case FilterOptions::FILES:
			{
				filter_by_type += "Files";
				break;
			}
			default:
				break;
			}
			ImGui::Text(filter_by_type.c_str());
			ImGui::EndTooltip();
		}

		ImGui::SameLine();

		ImGui::BeginDisabled(tabs[current_tab]->current_path.empty());
		if (ImGui::Button("Empty Folders")) 
		{
			// To be able to override the task mid seraching for the empty folders =======================
			if (m_emptyFoldersTask != nullptr) 
			{
				m_emptyFoldersTask->cancel_request = true;
				if (m_emptyFoldersTask->worker.joinable())
				{
					m_emptyFoldersTask->worker.join();
				}
				m_emptyFoldersTask = nullptr;
				m_emptyFolders.clear();
			}
			if (!m_emptyFolders.empty()) m_emptyFolders.clear();
			// ===========================================================================================

			if (tabs[current_tab]->selected_path.empty()) 
			{
				m_emptyFoldersTask = std::make_unique<Task>();
				m_emptyFoldersTask->sourceName = Utils::reform_string(tabs[current_tab]->current_path.string(), 32);
				m_emptyFoldersTask->task_type = TaskType::FIND_EMPTY_DIRECTORIES;

				Task* task = m_emptyFoldersTask.get();

				m_emptyFoldersTask->worker = std::thread([this, task]()
					{
						tabs[current_tab]->fileExplorer->find_empty_folders(tabs[current_tab]->current_path, m_emptyFolders, task->cancel_request, task->finished);
					});
			}
			else if (!tabs[current_tab]->selected_path.empty()) 
			{
				m_emptyFoldersTask = std::make_unique<Task>();
				m_emptyFoldersTask->sourceName = Utils::reform_string(tabs[current_tab]->selected_path.string(), 32);
				m_emptyFoldersTask->task_type = TaskType::FIND_EMPTY_DIRECTORIES;

				Task* task = m_emptyFoldersTask.get();

				m_emptyFoldersTask->worker = std::thread([this, task]()
					{
						tabs[current_tab]->fileExplorer->find_empty_folders(tabs[current_tab]->selected_path, m_emptyFolders, task->cancel_request, task->finished);
					});
			}
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(tabs[current_tab]->current_path.empty());
		if (ImGui::Button("Find Duplicates"))
		{
			// To be able to override the task mid seraching for the Duplicate files =====================
			if (m_duplicateFilesTask != nullptr) 
			{
				m_duplicateFilesTask->cancel_request = true;

				if (m_duplicateFilesTask->worker.joinable()) 
				{
					m_duplicateFilesTask->worker.join();
				}
				m_duplicateFilesTask = nullptr;
				m_duplicateFiles.clear();
			}
			if (!m_emptyFolders.empty()) m_emptyFolders.clear();
			// ===========================================================================================

			if (tabs[current_tab]->selected_path.empty()) 
			{
				m_duplicateFilesTask = std::make_unique<Task>();
				m_duplicateFilesTask->sourceName = Utils::reform_string(tabs[current_tab]->current_path.string(), 32);
				m_duplicateFilesTask->task_type = TaskType::FIND_DUPLICATE_FILES;

				Task* task = m_duplicateFilesTask.get();

				m_duplicateFilesTask->worker = std::thread([this, task]()
					{
						tabs[current_tab]->fileExplorer->find_duplicates(m_duplicateFiles, tabs[current_tab]->current_path, task->cancel_request, task->finished);
					});
			}
			else if (!tabs[current_tab]->selected_path.empty()) 
			{
				m_duplicateFilesTask = std::make_unique<Task>();
				m_duplicateFilesTask->sourceName = Utils::reform_string(tabs[current_tab]->selected_path.string(), 32);
				m_duplicateFilesTask->task_type = TaskType::FIND_DUPLICATE_FILES;

				Task* task = m_duplicateFilesTask.get();

				m_duplicateFilesTask->worker = std::thread([this, task]()
					{
						tabs[current_tab]->fileExplorer->find_duplicates(m_duplicateFiles, tabs[current_tab]->selected_path, task->cancel_request, task->finished);
					});
			}

		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(tabs[current_tab]->current_path.empty());
		if (ImGui::Button("Largest Files")) 
		{
			ImGui::OpenPopup("LargestFilesSizeMaximumSlider");
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		if (ImGui::Button("Advanced Search")) 
		{
			m_showSearchWindow = !m_showSearchWindow;
		}

		ImGui::SameLine();
		// ##########################################################################
		// For Debug TODO: Delete this button or remove it from the release mode
		// ##########################################################################
		ImGui::BeginDisabled(!m_index.isDone());
		if (ImGui::Button("Print Index Stats")) 
		{
			m_index.printIndexStats();
			m_index.printExtensionSample(L".exe");
		}
		ImGui::EndDisabled();
		if (ImGui::BeginItemTooltip()) 
		{
			std::string isIndexingDone = (m_index.isDone()) ? "True" : "False";
			ImGui::Text(isIndexingDone.c_str());
			
			ImGui::Text(std::format("Number of Files Scanned: {}", m_index.getFilesCount()).c_str());
			ImGui::Text(std::format("Number of Directories Scanned: {}", m_index.getDirectoriesCount()).c_str());
			ImGui::Text(std::format("Number of Total Entries Scanned: {}", m_index.getFilesCount() + m_index.getDirectoriesCount()).c_str());

			ImGui::EndTooltip();
		}
		// ##########################################################################

		float spacing = ImGui::GetStyle().ItemSpacing.x;
		float padding = ImGui::GetStyle().FramePadding.x;
		float target_pos = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (15.0f + padding * 1.7);

		ImGui::SetCursorPosX(target_pos);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::ImageButton("##Settings", M_texturesManager.get_settings_button_icon_texture_id(), ImVec2(15.0f, 15.0f)))
		{
			ImGui::OpenPopup("ViewSettings");
		}
		ImGui::PopStyleColor(1);

		if (need_delete_confirmation)
		{
			ImGui::OpenPopup("Delete Warning?");
			need_delete_confirmation = false; 
		}

		view_settings();

		new_whatever();

		view_sort_options();

		delete_warning();

		filter_by();

		largest_files_slider();

		advanced_search();
	}
	ImGui::EndChild();
	ImGui::PopStyleVar(1);
}

// ####################################################### POPUPS ######################################################
// View Settings Popup
void ForestView::view_settings() 
{
	if (ImGui::BeginPopup("ViewSettings"))
	{
		// Dark Mode button
		ImVec2 cursor = ImGui::GetCursorPos();
		if (ImGui::Selectable("##DarkModeSelectable", false, ImGuiSelectableFlags_SpanAllColumns))
		{
			change_theme(Theme::DARK_MODE);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetCursorPos(cursor);
		ImGui::Image(M_texturesManager.get_DarkMode_button_icon_texture_id(), ImVec2(15.0f, 15.0f));
		ImGui::SameLine();
		ImGui::Text("Dark Mode");

		// Light Mode Button
		cursor = ImGui::GetCursorPos();
		if (ImGui::Selectable("##LightModeSelectable", false, ImGuiSelectableFlags_SpanAllColumns))
		{
			change_theme(Theme::LIGHT_MODE);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetCursorPos(cursor);
		ImGui::Image(M_texturesManager.get_LightMode_button_icon_texture_id(), ImVec2(15.0f, 15.0f));
		ImGui::SameLine();
		ImGui::Text("Light Mode");

		/*if (ImGui::Selectable("Test Mode", false, ImGuiSelectableFlags_SpanAllColumns))
		{
			change_theme(Theme::TEST);
			ImGui::CloseCurrentPopup();
		}*/

		// icons size slider
		if (!show_icons_slider)
		{
			if (ImGui::Selectable("Open Icons Size Slider", false, ImGuiSelectableFlags_SpanAllColumns))
			{
				show_icons_slider = true;
				ImGui::CloseCurrentPopup();
			}
		}
		else
		{
			if (ImGui::Selectable("close Icons Size Slider", false, ImGuiSelectableFlags_SpanAllColumns))
			{
				show_icons_slider = false;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}
}

// New popup (New folder or file)
void ForestView::new_whatever() 
{
	if (ImGui::BeginPopup("New?"))
	{
		if (ImGui::Selectable("Folder"))
		{

			std::string name_of_the_folder = Utils::generate_new_folder_name(tabs[current_tab]->current_path, "New folder", FOLDER);
			tabs[current_tab]->fileExplorer->create_folder(tabs[current_tab]->current_path, name_of_the_folder, FOLDER);
			tabs[current_tab]->entries_need_update = true;
			tabs[current_tab]->tree_need_update = true;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Selectable("Text Document"))
		{
			std::string name_of_the_file = Utils::generate_new_folder_name(tabs[current_tab]->current_path, "New Text Document.txt", REG_FILE);
			tabs[current_tab]->fileExplorer->create_folder(tabs[current_tab]->current_path, name_of_the_file, REG_FILE);
			tabs[current_tab]->entries_need_update = true;
			tabs[current_tab]->tree_need_update = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

// Sorting popup
void ForestView::view_sort_options() 
{
	if (ImGui::BeginPopup("Sort Type"))
	{
		if (ImGui::Selectable("Name"))
		{
			tabs[current_tab]->fileExplorer->sort_entries([](const Entry& a, const Entry& b)
				{
					return a.entry.path() < b.entry.path();
				});
			tabs[current_tab]->entries = tabs[current_tab]->fileExplorer->get_entries();
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Selectable("Last Modified time"))
		{
			tabs[current_tab]->fileExplorer->sort_entries([](const Entry& a, const Entry& b) {return a.entry.last_write_time() > b.entry.last_write_time();});
			tabs[current_tab]->entries = tabs[current_tab]->fileExplorer->get_entries();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

// Deleting Warning popup
void ForestView::delete_warning()
{
	if (ImGui::BeginPopupModal("Delete Warning?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure you want to delete this folder?\nThe deleted file will be deleted permanently and will not go to the recycle bin \nThis action cannot be undone.");
		ImGui::Separator();

		if (ImGui::Button("YES"))
		{
			// Syncing the Tasks (Duplicates , Empty Folders, Largest Files) =======================
			// Sync the Empty Folders Deletion
			if (is_searching_for_empty_folders && fs::is_directory(tabs[current_tab]->deleted_path))
			{
				std::erase(m_emptyFolders, tabs[current_tab]->deleted_path);
			}
			
			if (fs::is_regular_file(tabs[current_tab]->deleted_path))
			{
				// Sync the Duplicate Files Deletion
				if (is_searching_for_duplicates_files) 
				{
					auto targetIt = std::find_if(m_duplicateFiles.begin(), m_duplicateFiles.end(),
						[this](const Duplicate& duplicateObj) {
							return tabs[current_tab]->deleted_path == duplicateObj.duplicateEntry.path();
						});

					int tempId = 0;
					size_t count_deleted = std::erase_if(m_duplicateFiles, [this, &tempId](const Duplicate& duplicateObj)
						{
							if (tabs[current_tab]->deleted_path == duplicateObj.duplicateEntry.path())
							{
								tempId = duplicateObj.groupId;
								return true;
							}
							return false;
						});
					if (count_deleted != 0)
				{
					size_t remainingInGroup = std::count_if(m_duplicateFiles.begin(), m_duplicateFiles.end(), [&tempId](const Duplicate& duplicateObj)
						{
							return duplicateObj.groupId == tempId;
						});

					if (remainingInGroup < 2) 
					{
						std::erase_if(m_duplicateFiles, [&tempId](const Duplicate& cleanUp)
							{
								return cleanUp.groupId == tempId;
							});
					}
				}
				}
				// Sync the Largest Files Deletion 
				if (is_searching_for_largest_files) 
				{
					std::erase(m_largestFiles, tabs[current_tab]->deleted_path);
				}
			}
			// =====================================================================================


			if (tabs[current_tab]->fav.is_exist(tabs[current_tab]->deleted_path))
			{
				tabs[current_tab]->fav.delete_fav(fs::directory_entry(tabs[current_tab]->deleted_path));
			}
			tabs[current_tab]->recents.remove(fs::directory_entry(tabs[current_tab]->deleted_path));

			fs::path src = tabs[current_tab]->deleted_path;

			std::unique_ptr<Task> deleteTask = std::make_unique<Task>();
			deleteTask->task_type = TaskType::_DELETE;
			deleteTask->source = tabs[current_tab]->deleted_path;
			deleteTask->sourceName = Utils::reform_string(tabs[current_tab]->deleted_path.filename().string(), 10);

			Task* task = deleteTask.get();

			deleteTask->worker = std::thread([this, src, task]()
				{
					tabs[current_tab]->fileExplorer->remove(src);
					task->finished = true;
				});

			tasks.emplace_back(std::move(deleteTask));
			tabs[current_tab]->deleted_path = "";
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("NO"))
		{
			tabs[current_tab]->selected_path = "";
			tabs[current_tab]->deleted_path = "";
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

// Access denied Popup
void ForestView::access_denied() 
{
	if (need_access_denied_popup) 
	{
		ImGui::OpenPopup("Access Denied");
		need_access_denied_popup = false;
	}

	if (ImGui::BeginPopupModal("Access Denied", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("You don't currently have permission to access this folder.");
		if (ImGui::Button("Ok"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

// Filter By popup
void ForestView::filter_by()
{
	if (ImGui::BeginPopup("FilterByOptions"))
	{
		if (ImGui::Selectable("All")) 
		{
			tabs[current_tab]->fileExplorer->set_filter_by(FilterOptions::ALL);
			tabs[current_tab]->entries_need_update = true;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Selectable("Files"))
		{
			tabs[current_tab]->fileExplorer->set_filter_by(FilterOptions::FILES);
			tabs[current_tab]->entries_need_update = true;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Selectable("Folders")) 
		{
			tabs[current_tab]->fileExplorer->set_filter_by(FilterOptions::FOLDERS);
			tabs[current_tab]->entries_need_update = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

// Largest Files Size Slider popup
void ForestView::largest_files_slider() 
{
	if (ImGui::BeginPopup("LargestFilesSizeMaximumSlider")) 
	{
		static float MaxSize = 10.0;
		ImGui::SliderFloat("Max Size", &MaxSize, 10.0, 1024.0);
		if (ImGui::Button("Find")) 
		{
			// To be able to override the task mid seraching for the Largest files =====================
			if (m_largestFilesTask != nullptr) 
			{
				m_largestFilesTask->cancel_request = true;
				if (m_largestFilesTask->worker.joinable())
				{
					m_largestFilesTask->worker.join();
				}
				m_largestFilesTask.reset();
				m_largestFiles.clear();
			}
			// ===========================================================================================


			if (tabs[current_tab]->selected_path.empty()) 
			{
				m_largestFilesTask = std::make_unique<Task>();
				m_largestFilesTask->task_type = TaskType::FIND_LARGEST_FILES;
				m_largestFilesTask->sourceName = Utils::reform_string(tabs[current_tab]->current_path.string(), 32);

				Task* task = m_largestFilesTask.get();

				m_largestFilesTask->worker = std::thread([this, task]()
					{
						tabs[current_tab]->fileExplorer->find_largest_files(m_largestFiles, tabs[current_tab]->current_path, task->cancel_request, task->finished, MaxSize);
					});
			}
			else if (!tabs[current_tab]->selected_path.empty()) 
			{
				m_largestFilesTask = std::make_unique<Task>();
				m_largestFilesTask->task_type = TaskType::FIND_LARGEST_FILES;
				m_largestFilesTask->sourceName = Utils::reform_string(tabs[current_tab]->selected_path.string(), 32);
				Task* task = m_largestFilesTask.get();
				m_largestFilesTask->worker = std::thread([this, task]()
					{
						tabs[current_tab]->fileExplorer->find_largest_files(m_largestFiles, tabs[current_tab]->selected_path, task->cancel_request, task->finished, MaxSize);
					});
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void ForestView::advanced_search()
{
	//ImGui::SetNextWindowSize(ImVec2(500.0f, 200.0f));

	if (m_showSearchWindow) 
	{
		if (ImGui::Begin("Advanced Search Window")) 
		{
			static std::wstring FileNameSearch = L"";

			// Search Window Tool Bar
			if (ImGui::Button("Close")) 
			{
				m_showSearchWindow = false;
				FileNameSearch.clear();
			}

			ImGui::SameLine();

			ImGui::Text(std::format(" | Number of Founds: {}", (int)m_founds.size()).c_str());

			ImGui::Separator();

			static constexpr std::array<const char*, 2> searchByFilter = { "Name", "Extention" };

			static SearchBy current_searchBy_filter = SearchBy::NAME;
			int choice = static_cast<int>(current_searchBy_filter);

			ImGui::SetNextItemWidth(150.0f);
			if (ImGui::Combo("Search By: ", &choice, searchByFilter.data(), static_cast<int>(searchByFilter.size())))
			{
				current_searchBy_filter = static_cast<SearchBy>(choice);
				FileNameSearch.clear();
				m_founds.clear();
			}

			static bool is_limitedSearch_enabled = false;
			if (ImGui::Checkbox("Limit search to current path", &is_limitedSearch_enabled)) 
			{
				if (current_searchBy_filter == SearchBy::NAME) 
				{
					if (FileNameSearch.size() >= 2) 
					{
						if (is_limitedSearch_enabled)
						{
							m_index.fastNameLookUp(m_founds, FileNameSearch, tabs[current_tab]->current_path);
						}
						else
						{
							m_index.fastNameLookUp(m_founds, FileNameSearch);
						}
					}
				}
				else if (current_searchBy_filter == SearchBy::EXTENTION) 
				{
					if (is_limitedSearch_enabled)
					{
						m_index.searchForExtention(m_founds, FileNameSearch, tabs[current_tab]->current_path);
					}
					else
					{
						m_index.searchForExtention(m_founds, FileNameSearch);
					}
				}
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Only show files located inside the current root folder and its subdirectories.");
			}

			ImGui::Separator();
			switch (current_searchBy_filter)
			{
			// Search By Name =====================================================
			case SearchBy::NAME: 
			{
				if (InputTextWString("File/Folder Name", FileNameSearch))
				{
					if (FileNameSearch.size() >= 2)
					{
						if (is_limitedSearch_enabled)
						{
							m_index.fastNameLookUp(m_founds, FileNameSearch, tabs[current_tab]->current_path);
						}
						else 
						{
							m_index.fastNameLookUp(m_founds, FileNameSearch);
						}
					}
					else
					{
						m_founds.clear();
					}
				}

				break;
			}
			// Search By Extention ================================================
			case SearchBy::EXTENTION:
			{
				if (InputTextWString("Extention Name", FileNameSearch))
				{
					if (is_limitedSearch_enabled) 
					{
						m_index.searchForExtention(m_founds, FileNameSearch, tabs[current_tab]->current_path);
					}
					else 
					{
						m_index.searchForExtention(m_founds, FileNameSearch);
					}
				}

				break;
			}

			default:
				break;
			}
			ImGui::SeparatorText("Founds ");

			// Founds Table =========================================================
			if (!m_founds.empty())
			{
				if (ImGui::BeginTable("FoundsTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
				{
					ImGui::TableSetupColumn("Path");
					ImGui::TableSetupColumn("Type");
					ImGui::TableSetupColumn("Size");
					ImGui::TableHeadersRow();

					ImGuiListClipper FoundsClipper;
					FoundsClipper.Begin((int)m_founds.size());

					while (FoundsClipper.Step())
					{
						for (size_t i = FoundsClipper.DisplayStart; i < FoundsClipper.DisplayEnd; i++)
						{
							ImGui::TableNextRow();


							// ========================================== Path ================================================
							ImGui::TableNextColumn();
							std::u8string utf8_filename= m_founds[i].filename().u8string();
	#if defined(__cpp_lib_char8_t)
							std::string display_filename(utf8_filename.begin(), utf8_filename.end());
	#else
							std::string display_path = utf8_filename;
	#endif
							std::string label = display_filename + "##" + std::to_string(i);


							ImGui::Selectable(label.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);
							if (ImGui::BeginItemTooltip()) 
							{
								std::u8string utf8_path = m_founds[i].u8string();
#if defined(__cpp_lib_char8_t)
								std::string display_path(utf8_path.begin(), utf8_path.end());
#else
								std::string display_path = utf8_path;
#endif
								ImGui::Text(display_path.c_str());
								ImGui::EndTooltip();
							}
							fs::directory_entry foundsEntry = fs::directory_entry(m_founds[i]);
							handle_entries_mouse_click(foundsEntry);

							// ========================================== Type ================================================
							ImGui::TableNextColumn();
							fs::file_type type = foundsEntry.status().type();

							ImGui::Selectable(Utils::file_type_to_string(type).c_str());

							// ========================================== Size ================================================
							ImGui::TableNextColumn();

							if (!foundsEntry.is_directory())
							{
								fileSize size = tabs[current_tab]->fileExplorer->decide_file_size_format(foundsEntry.file_size());
								ImGui::Selectable(std::format("{:.2f} {}", size.size, size.label).c_str());
							}

						}
					}
					ImGui::EndTable();
				}
			}
			else 
			{
				ImGui::Text("Nothing Found");
			}
		}
			ImGui::End();
	}
}

// FOLDERS RIGHT CLICK 
void ForestView::folders_right_click_popup(const fs::directory_entry& FolderEntry)
{
	if (tabs[current_tab]->need_folder_right_click_call)
	{
		ImGui::OpenPopup("FoldersRightClickPopUp");
		tabs[current_tab]->need_folder_right_click_call = false;
	}
	static bool folder_popup_was_open = false;
	bool folder_popup_is_open = ImGui::BeginPopup("FoldersRightClickPopUp");
	if (folder_popup_is_open)
	{
		if (FolderEntry.path() != tabs[current_tab]->selected_path) tabs[current_tab]->selected_path = FolderEntry.path();
		
		// Rename 
		if (tabs[current_tab]->selected_path != tabs[current_tab]->selected_path.root_path())
		{
			static std::string newName;
			ImGui::InputText("New Name", &newName);
			if (ImGui::Button("Rename"))
			{
				RenameReturn rename = tabs[current_tab]->fileExplorer->rename_file(tabs[current_tab]->selected_path, newName);
				if (rename.success)
				{
					if (is_searching_for_empty_folders && fs::is_directory(rename.finalPath)) 
					{
						for (auto& path : m_emptyFolders) 
						{
							if (path == tabs[current_tab]->selected_path) 
							{
								path = rename.finalPath;
							}
						}
					}
						
					if (tabs[current_tab]->fav.is_exist(fs::directory_entry(tabs[current_tab]->selected_path)))
					{
						tabs[current_tab]->fav.update_name(fs::directory_entry(tabs[current_tab]->selected_path), rename.finalPath);
					}
					if (tabs[current_tab]->recents.update_name(fs::directory_entry(tabs[current_tab]->selected_path), rename.finalPath))
					{
					}
					tabs[current_tab]->entries_need_update = true;
					tabs[current_tab]->tree_need_update = true;
					newName.clear();
					tabs[current_tab]->selected_path = "";
				}
				else
				{
					tabs[current_tab]->selected_path = "";
					tabs[current_tab]->message = "This name already exists";
				}
				ImGui::CloseCurrentPopup();
			}
			if (!tabs[current_tab]->message.empty()) ImGui::SameLine();
			render_msg();
			ImGui::Separator();
		}

		// Delete
		if (ImGui::Button("Delete")) 
		{
			need_delete_confirmation = true;
			tabs[current_tab]->deleted_path = tabs[current_tab]->selected_path;
			ImGui::CloseCurrentPopup();
		}

		// Copy path
		if (ImGui::Button("Copy Path"))
		{
			tabs[current_tab]->show_rename_file = false;
			ImGui::SetClipboardText(tabs[current_tab]->selected_path.string().c_str());


			tabs[current_tab]->selected_path = "";
			ImGui::CloseCurrentPopup();
		}

		// copy structrure
		if (ImGui::Button("Copy Structure"))
		{
			tabs[current_tab]->folder_structure = std::async(std::launch::async, [path = tabs[current_tab]->selected_path]() {
				// Create a local, isolated tree instance inside the thread so it doesn't touch main thread data
				tree localTree;
				localTree.BuildTree(path);
				return localTree.put_structure_into_string();
				});
			tabs[current_tab]->is_coping_structure = true;
			tabs[current_tab]->selected_path = "";
			ImGui::CloseCurrentPopup();
		}

		// Add to favorites logic
		if (FolderEntry.is_directory())
		{
			if (!tabs[current_tab]->fav.is_exist(FolderEntry))
			{
				if (ImGui::Button("Add to favorites"))
				{
					tabs[current_tab]->fav.add_fav(FolderEntry);

					tabs[current_tab]->selected_path = "";
					ImGui::CloseCurrentPopup();
				}
			}
			else
			{
				if (ImGui::Button("Unpin from favorites"))
				{
					tabs[current_tab]->fav.delete_fav(FolderEntry);

					tabs[current_tab]->selected_path = "";
					ImGui::CloseCurrentPopup();
				}
			}
		}

		ImGui::EndPopup();
	}
	if (folder_popup_was_open && !folder_popup_is_open)
	{
		tabs[current_tab]->selected_path = "";
	}
	folder_popup_was_open = folder_popup_is_open;
}

// Files LEFT CLICK
void ForestView::files_right_click_popup(const fs::directory_entry& FileEntry) 
{
	if (tabs[current_tab]->need_file_right_click_call)
	{
		ImGui::OpenPopup("FilesRightClickPopUp");
		tabs[current_tab]->need_file_right_click_call = false;
	}
	static bool files_popup_was_open = false;
	bool files_popup_is_open = ImGui::BeginPopup("FilesRightClickPopUp");
	if (files_popup_is_open)
	{
		if (FileEntry.path() != tabs[current_tab]->selected_path) tabs[current_tab]->selected_path = FileEntry.path();
		// Rename 
		if (tabs[current_tab]->selected_path != tabs[current_tab]->selected_path.root_path())
		{
			static std::string newName;
			ImGui::InputText("New Name##FileRightClick", &newName);
			if (ImGui::Button("Rename##FileRightClick"))
			{
				RenameReturn rename = tabs[current_tab]->fileExplorer->rename_file(tabs[current_tab]->selected_path, newName);
				if (rename.success)
				{
					if (is_searching_for_duplicates_files)
					{
						for (Duplicate& DuplicateObject : m_duplicateFiles)
						{
							if (DuplicateObject.duplicateEntry.path() == tabs[current_tab]->selected_path)
							{
								DuplicateObject.duplicateEntry.assign(rename.finalPath);
								break;
							}
						}
					}
					if (is_searching_for_largest_files) 
					{
						for (fs::directory_entry& largeEntry : m_largestFiles)
						{
							if (largeEntry.path() == tabs[current_tab]->selected_path)
							{
								largeEntry.assign(rename.finalPath);
								break;
							}
						}
					}

					if (tabs[current_tab]->fav.is_exist(fs::directory_entry(tabs[current_tab]->selected_path)))
					{
						tabs[current_tab]->fav.update_name(fs::directory_entry(tabs[current_tab]->selected_path), rename.finalPath);
					}
					if (tabs[current_tab]->recents.update_name(fs::directory_entry(tabs[current_tab]->selected_path), rename.finalPath))
					{
					}
					tabs[current_tab]->entries_need_update = true;
					tabs[current_tab]->tree_need_update = true;
					newName.clear();
					tabs[current_tab]->selected_path = "";
				}
				else
				{
					tabs[current_tab]->selected_path = "";
					tabs[current_tab]->message = "This name already exists";
				}
				ImGui::CloseCurrentPopup();
			}
			if (!tabs[current_tab]->message.empty()) ImGui::SameLine();

			render_msg();

			ImGui::Separator();

		}

		// Delete
		if (ImGui::Button("Delete##Duplicate"))
		{
			need_delete_confirmation = true;
			tabs[current_tab]->deleted_path = tabs[current_tab]->selected_path;
			ImGui::CloseCurrentPopup();
		}

		// Copy path
		if (ImGui::Button("Copy Path##Duplicate"))
		{
			tabs[current_tab]->show_rename_file = false;
			ImGui::SetClipboardText(tabs[current_tab]->selected_path.string().c_str());


			tabs[current_tab]->selected_path = "";
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (files_popup_was_open && !files_popup_is_open)
	{
		tabs[current_tab]->selected_path = "";
	}
	files_popup_was_open = files_popup_is_open;
}
	
// ######################################################################################################################

// Folders Tree 
void ForestView::folders_tree(const float& remainingHeight)
{
	if (ImGui::BeginChild("FolderTree##identifer", ImVec2(200, remainingHeight), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar))
	{
		// Bookmarks ===================================================== 
		if (ImGui::BeginTabBar("Tabs"))
		{
			// Favorites =========================================================
			if (ImGui::BeginTabItem("Favorites"))
			{

				for (const auto& favorite_item : tabs[current_tab]->fav.get_favorites())
				{
					ImGui::PushID(favorite_item.path().c_str());

					auto u8_str = (favorite_item.path() == favorite_item.path().root_path()) ? favorite_item.path().u8string() : favorite_item.path().filename().u8string();
					std::string favorite_to_show(u8_str.begin(), u8_str.end());

					if (ImGui::Selectable(favorite_to_show.c_str()))
					{
						tabs[current_tab]->fileExplorer->navigate_to(favorite_item.path(), tabs[current_tab]->current_path);
						tabs[current_tab]->current_path = favorite_item.path();
						tabs[current_tab]->entries_need_update = true;
						tabs[current_tab]->selected_path = "";
					}
					if (ImGui::BeginPopupContextItem("favoritesRightClick"))
					{
						if (ImGui::Button("Unpin from favorites"))
						{
							tabs[current_tab]->fav.delete_fav(favorite_item);
							ImGui::CloseCurrentPopup();
						}

						ImGui::EndPopup();
					}
					ImGui::PopID();
				}

				if (tabs[current_tab]->fav.is_empty()) ImGui::Text("No favorites pinned...");

				ImGui::Separator();

				ImGui::EndTabItem();
			}

			// Recent History ============================================================
			if (ImGui::BeginTabItem("Recent History"))
			{
				for (const auto& recent_item : tabs[current_tab]->recents.get_recent_history())
				{
					ImGui::PushID(recent_item.path().c_str());

					auto u8_str = (recent_item.path() == recent_item.path().root_path()) ? recent_item.path().u8string() : recent_item.path().filename().u8string();
					std::string recent_to_show(u8_str.begin(), u8_str.end());

					ImGui::Selectable(recent_to_show.c_str());

					if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) 
					{
						fs::path recentPath = recent_item.path();
						tabs[current_tab]->fileExplorer->navigate_to(recent_item.path(), tabs[current_tab]->current_path);
						tabs[current_tab]->current_path = recent_item.path();
						tabs[current_tab]->entries = tabs[current_tab]->fileExplorer->get_entries();
						tabs[current_tab]->recents.visit(recent_item);
						tabs[current_tab]->selected_path = "";
					}
					ImGui::PopID();
				}

				if (tabs[current_tab]->recents.is_empty()) ImGui::Text("No Recents yet...");

				ImGui::Separator();

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		// Folders Tree =======================================================
		if (tabs[current_tab]->tree_need_update)
		{
			tabs[current_tab]->t.update_tree(tabs[current_tab]->fileExplorer->get_partitions());
			tabs[current_tab]->tree_need_update = false;
		}

		draw_tree(tabs[current_tab]->t.get_root());
	}
	ImGui::EndChild();

}

void ForestView::draw_tree(Node* node_to_show)
{
	if (node_to_show == nullptr) return;

	std::string node_to_show_path_string_to_show;
	if (node_to_show->path.empty())
	{
		node_to_show_path_string_to_show = node_to_show->virtualName;
	}
	else
	{
		auto u8 = (node_to_show->path == node_to_show->path.root_path()) ? node_to_show->path.u8string() : node_to_show->path.filename().u8string();
		node_to_show_path_string_to_show.assign(u8.begin(), u8.end());
	}


	ImGui::PushID(node_to_show);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (ImGui::TreeNodeEx(node_to_show_path_string_to_show.c_str(), flags))
	{
		if (node_to_show->is_loaded == false)
		{
			tabs[current_tab]->t.AddChildrenOf(node_to_show->path);
			node_to_show->is_loaded = true;
		}

		for (const auto& i : node_to_show->children)
		{
			draw_tree(i);
		}
		ImGui::TreePop();
	}

	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (!node_to_show->path.empty())
			{
				tabs[current_tab]->fileExplorer->navigate_to(node_to_show->path, tabs[current_tab]->current_path);
				tabs[current_tab]->current_path = node_to_show->path;
				tabs[current_tab]->selected_path = "";
			}
		}
	}

	ImGui::PopID();

	return;
}

// Files Navigation Panel
void ForestView::files_navigation_panel(const float& remainingHeight)
{
	if (ImGui::BeginChild("Files##identifier", ImVec2(0, remainingHeight), ImGuiChildFlags_Borders))
	{
		ImGui::Text("Files");
		ImGui::Separator();

		// Updating the entries vector ============================================================
		if (tabs[current_tab]->entries_need_update)
		{
			tabs[current_tab]->fileExplorer->refresh(tabs[current_tab]->current_path);
			tabs[current_tab]->entries = tabs[current_tab]->fileExplorer->get_entries();
			auto u8_str = (tabs[current_tab]->current_path == tabs[current_tab]->current_path.root_path()) ? tabs[current_tab]->current_path.u8string() : tabs[current_tab]->current_path.filename().u8string();
			tabs[current_tab]->utf8_search_bar_hint.assign(u8_str.begin(), u8_str.end());
			if (tabs[current_tab]->utf8_search_bar_hint.empty()) tabs[current_tab]->utf8_search_bar_hint = "This PC";
			tabs[current_tab]->entries_need_update = false;
		}

		// rendring the entries logic (for loop) ==================================================
		render_entries();

		// clearing the selected_Path if i press anywhere that not an entry =======================
		if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				tabs[current_tab]->selected_path = "";
			}
		}

		// the current and selceted path debug panel ==============================================
		render_debug_panel();

		// asyncc folder structure copying ======================================================== 
		if (tabs[current_tab]->folder_structure.valid() && tabs[current_tab]->folder_structure.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			tabs[current_tab]->copied_structure = tabs[current_tab]->folder_structure.get();
			ImGui::SetClipboardText(tabs[current_tab]->copied_structure.c_str());
			tabs[current_tab]->is_coping_structure = false;
			tabs[current_tab]->copied_structure = "";
		}

	}
	ImGui::EndChild();
}

void ForestView::render_entries()
{
	if (tabs[current_tab]->view_type == LARGE_THUMBNAILS)
	{
		float avalWidth = ImGui::GetContentRegionAvail().x;
		int columnscount = (int)(avalWidth / (tabs[current_tab]->icons_width + tabs[current_tab]->padding));
		if (columnscount < 1) columnscount = 1;
		ImGui::Columns(columnscount, 0, false);

		for (size_t i = 0; i < tabs[current_tab]->entries.size(); ++i)
		{
			ImGui::PushID(i);

			bool is_selected = (tabs[current_tab]->entries[i].entry.path() == tabs[current_tab]->selected_path) ? true : false;

			if (is_selected) 
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Header]);
			}
			else 
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			}

			ImGui::ImageButton(tabs[current_tab]->entries[i].name.c_str(), tabs[current_tab]->entries[i].textureId, ImVec2(tabs[current_tab]->icons_width, tabs[current_tab]->icons_height));

			ImGui::PopStyleColor(1);

			handle_entries_mouse_click(tabs[current_tab]->entries[i].entry);

			float cursorPosX = ImGui::GetCursorPos().x;
			float cursorPosY = ImGui::GetCursorPos().y;
			ImGui::SetCursorPos(ImVec2(cursorPosX + folder_icons_horizontel_indent, cursorPosY));
			ImGui::TextWrapped(tabs[current_tab]->entries[i].name.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);
	}

	else if (tabs[current_tab]->view_type == ROWS)
	{
		if (ImGui::BeginTable("entries", 4, ImGuiTableFlags_Resizable))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Last Modified");
			ImGui::TableSetupColumn("Type");
			ImGui::TableSetupColumn("Size");
			ImGui::TableHeadersRow();

			ImGuiListClipper Clipper;
			Clipper.Begin(tabs[current_tab]->entries.size());
			while (Clipper.Step()) 
			{
				for (size_t i = Clipper.DisplayStart; i < Clipper.DisplayEnd; ++i)
				{
					ImGui::TableNextRow();

					ImGui::PushID(i);

					const float& icon_height = tabs[current_tab]->icons_height;
					// ======================================== Name ==============================================
					ImGui::TableNextColumn();

					bool is_selected = (tabs[current_tab]->entries[i].entry.path() == tabs[current_tab]->selected_path) ? true : false;

					ImGui::Image(tabs[current_tab]->entries[i].textureId, ImVec2(tabs[current_tab]->icons_width, icon_height));

					ImGui::SameLine();

					ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.005f, 0.5f));

					ImGui::Selectable(tabs[current_tab]->entries[i].name.c_str(), is_selected, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0, icon_height));

					handle_entries_mouse_click(tabs[current_tab]->entries[i].entry);

					// ========================================== Last Modified ================================================
					ImGui::TableNextColumn();

					fs::file_time_type time = tabs[current_tab]->entries[i].entry.last_write_time();

					auto sys_time = std::chrono::clock_cast<std::chrono::system_clock>(time);			
																										
					auto local = std::chrono::zoned_time{ std::chrono::current_zone(), sys_time };		
																										
					std::string res = std::format("{:%Y-%m-%d %H:%M}", local);							
													
					ImGui::Selectable(res.c_str(), false, 0, ImVec2(0.0f, icon_height));

					// ========================================== Type ================================================
					ImGui::TableNextColumn();
					fs::file_type type = tabs[current_tab]->entries[i].entry.status().type();

					ImGui::Selectable(Utils::file_type_to_string(type).c_str(), false, 0, ImVec2(0.0f, icon_height));

					// ========================================== Size ================================================
					ImGui::TableNextColumn();

					if (!tabs[current_tab]->entries[i].entry.is_directory())
					{
						fileSize size = tabs[current_tab]->fileExplorer->decide_file_size_format(tabs[current_tab]->entries[i].entry.file_size());
						ImGui::Selectable(std::format("{:.2f} {}", size.size, size.label).c_str(), false, 0, ImVec2(0.0f, icon_height));
					}
					ImGui::PopStyleVar(1);
					ImGui::PopID();
				}

			}
			ImGui::EndTable();
		}
			
		
	}

}

void ForestView::render_debug_panel()
{
	auto u8 = tabs[current_tab]->current_path.u8string();
	std::string current_path_string_to_show(u8.begin(), u8.end());

	auto u8_selected = tabs[current_tab]->selected_path.u8string();
	std::string selected_path_string_to_show(u8_selected.begin(), u8_selected.end());

	auto u8_deleted = tabs[current_tab]->deleted_path.u8string();
	std::string deleted_path_string_to_show(u8_deleted.begin(), u8_deleted.end());


	ImGui::Separator();

	ImGui::Text("Current Path: "); ImGui::SameLine();
	ImGui::Text(current_path_string_to_show.c_str());

	ImGui::Text("Seleted Path: "); ImGui::SameLine();
	ImGui::Text(selected_path_string_to_show.c_str());

	ImGui::Text("Deleted Path: "); ImGui::SameLine();
	ImGui::Text(deleted_path_string_to_show.c_str());

	std::string flag = (is_searching_for_empty_folders) ? "True" : "False";
	ImGui::Text("is_searching_for_empty_folders Flag: "); ImGui::SameLine();
	ImGui::Text(flag.c_str());

	std::string flag2 = (is_searching_for_duplicates_files) ? "True" : "False";
	ImGui::Text("is_searching_for_duplicates_files Flag: "); ImGui::SameLine();
	ImGui::Text(flag2.c_str());

	ImGui::Separator();
}

void ForestView::handle_entries_mouse_click(const fs::directory_entry& entry)
{
	if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				DirAccessResult checkAccess = tabs[current_tab]->fileExplorer->check_directory_access(entry.path());
				if (checkAccess == DirAccessResult::ACCESS_DENIED)
				{
					need_access_denied_popup = true;
					return;
				}

				if (entry.is_directory()) 
				{
					left_double_click(entry);
				}
				else if (entry.is_regular_file()) 
				{
					const fs::path& path = entry.path().parent_path();
					if (tabs[current_tab]->current_path != path)
					{
						const fs::directory_entry parent = fs::directory_entry(path);
						left_double_click(parent);
					}
				}
			}
			else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))	
			{
				DirAccessResult checkAccess = tabs[current_tab]->fileExplorer->check_directory_access(entry.path());
				if (checkAccess == DirAccessResult::ACCESS_DENIED)
				{
					need_access_denied_popup = true;
					return;
				}

				tabs[current_tab]->selected_path = entry.path();
				if (entry.is_directory())
				{
					tabs[current_tab]->need_folder_right_click_call = true;
				}
				else
				{
					tabs[current_tab]->need_file_right_click_call = true;
				}
			}
			else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			{
				DirAccessResult checkAccess = tabs[current_tab]->fileExplorer->check_directory_access(entry.path());
				if (checkAccess == DirAccessResult::ACCESS_DENIED)
				{
					need_access_denied_popup = true;
					return;
				}

				left_click(entry);
			}
		}
}

// PARENT HANDLER TO CHOOSE WHICH POPUP TRIGGER
void ForestView::decide_correct_rightClick_popup()
{
	folders_right_click_popup(fs::directory_entry(tabs[current_tab]->selected_path));
	files_right_click_popup(fs::directory_entry(tabs[current_tab]->selected_path));

}

void ForestView::left_double_click(const fs::directory_entry& entry)
{
	if (entry.is_directory())
	{
		DirAccessResult checkAccess = tabs[current_tab]->fileExplorer->check_directory_access(entry.path());

		if (checkAccess == DirAccessResult::ACCESS_DENIED)
		{
			ImGui::OpenPopup("Access Denied");
		}
		else
		{
			tabs[current_tab]->ThisDirectoryKeyWord = "";
			tabs[current_tab]->globalKeyWord = "";
			tabs[current_tab]->recents.visit(entry);
			tabs[current_tab]->fileExplorer->navigate_to(entry.path(), tabs[current_tab]->current_path);
			tabs[current_tab]->entries_need_update = true;

			tabs[current_tab]->current_path = entry.path();
		}
	}
}

void ForestView::left_click(const fs::directory_entry& entry)
{
	if (tabs[current_tab]->selected_path == entry.path())
	{
		tabs[current_tab]->selected_path = "";
		tabs[current_tab]->file_size.size = 0.0;
		tabs[current_tab]->file_size.label = "";
		tabs[current_tab]->last_modified = "";
	}
	else
	{
		tabs[current_tab]->selected_path = entry.path();
		tabs[current_tab]->need_file_size_call = true;
		tabs[current_tab]->need_modified_since_call = true;
	}
}

// Stats Bar
void ForestView::stats_bar()
{
	if (ImGui::BeginChild("StatsBar", ImVec2(0, 40), ImGuiChildFlags_Borders))
	{
		// Number of entries =========================================================================================================================================
		ImGui::Text(std::to_string(tabs[current_tab]->fileExplorer->get_size()).c_str());
		ImGui::SameLine();
		ImGui::Text(" Items ");

		ImGui::SameLine();

		// Size of selected entry (the if condition is = we calculate the size only and only if the selected path is NOT empty and NOT a root path like (D:\, C;\))
		if (tabs[current_tab]->selected_path != tabs[current_tab]->selected_path.root_path() && !tabs[current_tab]->selected_path.empty())
		{
			ImGui::Text("| File Size: ");
			ImGui::SameLine();

			if (tabs[current_tab]->need_file_size_call)
			{
				tabs[current_tab]->file_size_future.push_back(std::async(std::launch::async, [&fileExplorer = this->tabs[current_tab]->fileExplorer, path = tabs[current_tab]->selected_path]() {return fileExplorer->decide_file_size_format( fileExplorer->get_total_files_size(path).directorySize);}));
				tabs[current_tab]->need_file_size_call = false;
				tabs[current_tab]->is_calulating_size = true;
			}

			if (tabs[current_tab]->is_calulating_size && !tabs[current_tab]->file_size_future.empty() && tabs[current_tab]->file_size_future.back().valid())
			{
				if (tabs[current_tab]->file_size_future.back().wait_for(std::chrono::seconds(0)) == std::future_status::ready)
				{
					tabs[current_tab]->file_size = tabs[current_tab]->file_size_future.back().get();
					tabs[current_tab]->file_size_future.pop_back();
					tabs[current_tab]->is_calulating_size = false;
				}
			}

			std::erase_if(tabs[current_tab]->file_size_future, [](auto& x) {return !x.valid() || x.wait_for(std::chrono::seconds(0)) == std::future_status::ready;});

			if (!tabs[current_tab]->is_calulating_size) ImGui::Text((std::format("{:.2f} {}", tabs[current_tab]->file_size.size, tabs[current_tab]->file_size.label)).c_str());
			else ImGui::Text("Calculating size....");


			if (tabs[current_tab]->need_modified_since_call)
			{
				tabs[current_tab]->last_modified = tabs[current_tab]->fileExplorer->get_last_modified_since(tabs[current_tab]->selected_path);
				tabs[current_tab]->need_modified_since_call = false;
			}
			if (!tabs[current_tab]->last_modified.empty())
			{
				ImGui::SameLine();
				ImGui::Text(" | Modified Since: ");
				ImGui::SameLine();
				ImGui::Text(tabs[current_tab]->last_modified.c_str());
			}
		}

		// Copying strucure msg (only if we actually coping a structure) ===========================================================================================
		if (tabs[current_tab]->is_coping_structure)
		{
			ImGui::SameLine();

			ImGui::Text(" | Coping Structure....");
		}

		ImGui::SameLine();

		// View Style Buttons =======================================
		float spacing = ImGui::GetStyle().ItemSpacing.x;
		float padding = ImGui::GetStyle().FramePadding.x;
		float target_pos = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (15.0f * 2 + spacing + (padding * 4));

		ImGui::SetCursorPosX(target_pos);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		if (ImGui::ImageButton("Rows", M_texturesManager.get_Rows_button_icon_texture_id(), ImVec2(12.0f, 12.0f)))
		{
			tabs[current_tab]->view_type = ROWS;
			tabs[current_tab]->icons_height = 24.0f;
			tabs[current_tab]->icons_width = 24.0f;
		}
		ImGui::SameLine();
		if (ImGui::ImageButton("Large thumbnails", M_texturesManager.get_LargeThumbnail_button_icon_texture_id(), ImVec2(12.0f, 12.0f)))
		{
			tabs[current_tab]->view_type = LARGE_THUMBNAILS;
			tabs[current_tab]->icons_height = 128.0f;
			tabs[current_tab]->icons_width = 128.0f;
		}
		ImGui::PopStyleColor(1);
	}

	ImGui::EndChild();
}

void ForestView::icons_size_slider()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(50.0f, 600.f), ImVec2(50.0f, 600.f));
	if (ImGui::Begin("##iconsSizeSlider", nullptr,  ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize)) 
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 5.0f));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::ImageButton("##resetButton" , M_texturesManager.get_reset_button_icon_texture_id(), ImVec2(15.0f, 15.0f)))
		{
			switch (tabs[current_tab]->view_type)
			{
			case LARGE_THUMBNAILS:
			{
				tabs[current_tab]->icons_height = 128.0f;
				tabs[current_tab]->icons_width = 128.0f;
				break;
			}
			case ROWS:
			{
				tabs[current_tab]->icons_height = 24.0f;
				tabs[current_tab]->icons_width = 24.0f;
				break;
			}
			default:
				break;
			}
		}
		ImGui::PopStyleColor(1);

		if (tabs[current_tab]->view_type == LARGE_THUMBNAILS)
		{
			if (ImGui::VSliderFloat("##vslideridentifier", ImVec2(25.0f, 540.0f), &tabs[current_tab]->icons_width, 64.0f, 256.0f, "%.0f"))
			{
				tabs[current_tab]->icons_height = tabs[current_tab]->icons_width;
			}
		}
		else if (tabs[current_tab]->view_type == ROWS)
		{
			if (ImGui::VSliderFloat("##vslideridentifier", ImVec2(25.0f, 540.0f), &tabs[current_tab]->icons_width, 16.0f, 128.0f, "%.0f"))
			{
				tabs[current_tab]->icons_height = tabs[current_tab]->icons_width;
			}
		}
		ImGui::PopStyleVar(3);
	}
	ImGui::End();
}

// Progress Bar
void ForestView::show_progress_bar(const std::string& overlay)
{
	if (ImGui::Begin("Progress"))
	{
		ImGui::ProgressBar(0.5f, ImVec2(.00f, 50.0f), overlay.c_str());
	}
	ImGui::End();
}

void ForestView::shutdown_background_tasks()
{
	auto start = std::chrono::high_resolution_clock::now();

	if (tabs[current_tab]->folder_structure.valid())
	{
		tabs[current_tab]->folder_structure.wait();
	}
	for (const auto& x : tabs[current_tab]->file_size_future)
	{
		if (x.valid()) 
		{
			x.wait();
		}
	}

	for (std::unique_ptr<Task>& task : tasks)
	{
		task->cancel_request = true;
	}
	for (std::unique_ptr<Task>& task : tasks) 
	{
		if (task->worker.joinable()) task->worker.join();
	}

	// The empty Folder Task shut down
	if (m_emptyFoldersTask != nullptr) 
	{
		m_emptyFoldersTask->cancel_request = true;
		if (m_emptyFoldersTask->worker.joinable()) 
	{
		m_emptyFoldersTask->worker.join();
	}
	}
	tabs[current_tab]->file_size_future.clear();

	// The Duplicate Files Task shut down
	if (m_duplicateFilesTask != nullptr) 
	{
		m_duplicateFilesTask->cancel_request = true;
		if (m_duplicateFilesTask->worker.joinable())
		{
			m_duplicateFilesTask->worker.join();
		}
	}

	// The Largest Files Task shut down
	if (m_largestFilesTask != nullptr)
	{
		m_largestFilesTask->cancel_request = true;
		if (m_largestFilesTask->worker.joinable())
		{
			m_largestFilesTask->worker.join();
		}
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> duration = end - start;
	double durationToShow = duration.count();

	if (durationToShow < 0.001) std::cout << "Time taken to shutdown the multithreaded tasks: " << durationToShow * 1000 * 1000 << " Microseconds" << std::endl;
	else if (durationToShow < 1) std::cout << "Time taken to shutdown the multithreaded tasks: " << durationToShow * 1000 << " Milliseconds" << std::endl;
	else std::cout << "Time taken to shutdown the multithreaded tasks: " << std::endl << duration.count() << " Seconds" << std::endl;
}


