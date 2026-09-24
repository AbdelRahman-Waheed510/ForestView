#include "pchApp.h"
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
	arabic_font = io.Fonts->AddFontFromFileTTF("assets/fonts/NotoSansArabic-SemiBold.ttf", 24.0f, nullptr);

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
	style.WindowPadding = ImVec2(10, 7);
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

	style.Colors[ImGuiCol_CheckMark] = unique_color;
	style.Colors[ImGuiCol_SliderGrab] = unique_color;
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.48f, 0.70f, 1.00f, 1.00f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.18039216f, 0.18823530f, 0.19607843f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.20000000f, 0.21568628f, 0.23921569f, 1.00f);

	style.Colors[ImGuiCol_Header] = ImVec4(0.14117648f, 0.16470589f, 0.20784314f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.17647059f, 0.20000000f, 0.24313726f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.20000000f, 0.22745098f, 0.27450982f, 1.00f);

	style.Colors[ImGuiCol_Separator] = ImVec4(0.12941177f, 0.14901961f, 0.19215687f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = unique_color;
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.48f, 0.70f, 1.00f, 1.00f);

	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.18f, 0.20f, 0.24f, 0.70f);
	style.Colors[ImGuiCol_ResizeGripHovered] = unique_color;
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.48f, 0.70f, 1.00f, 1.00f);

	style.Colors[ImGuiCol_Tab] = ImVec4(0.07843138f, 0.08627451f, 0.10196079f, 1.00f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.14500000f, 0.16000000f, 0.19000000f, 1.00f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.16000000f, 0.18000000f, 0.22000000f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.07843138f, 0.08627451f, 0.10196079f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.12000000f, 0.14000000f, 0.17000000f, 1.00f);

	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.55f, 0.60f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = unique_color;
	style.Colors[ImGuiCol_PlotHistogram] = unique_color;
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.48f, 0.70f, 1.00f, 1.00f);

	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.04705882f, 0.05490196f, 0.07058824f, 1.00f);
	style.Colors[ImGuiCol_TableBorderStrong] = style.Colors[ImGuiCol_Border];
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);

	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09f, 0.10f, 0.12f, 0.45f);

	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.38f, 0.60f, 0.95f, 0.30f);

	style.Colors[ImGuiCol_DragDropTarget] = unique_color;
	style.Colors[ImGuiCol_NavHighlight] = unique_color;
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

	// Steel Blue unique_color
	const ImVec4 unique_color = ImVec4(0.38f, 0.60f, 0.95f, 1.0f);

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

	style.Colors[ImGuiCol_CheckMark] = unique_color;
	style.Colors[ImGuiCol_SliderGrab] = unique_color;
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.28f, 0.52f, 0.90f, 1.00f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.89f, 0.91f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.87f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.79f, 0.83f, 0.89f, 1.00f);

	style.Colors[ImGuiCol_Header] = ImVec4(0.88f, 0.91f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.81f, 0.86f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.74f, 0.81f, 0.92f, 1.00f);

	style.Colors[ImGuiCol_Separator] = ImVec4(0.78f, 0.80f, 0.84f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = unique_color;
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.28f, 0.52f, 0.90f, 1.00f);

	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.72f, 0.75f, 0.80f, 0.70f);
	style.Colors[ImGuiCol_ResizeGripHovered] = unique_color;
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.28f, 0.52f, 0.90f, 1.00f);

	style.Colors[ImGuiCol_Tab] = ImVec4(0.90f, 0.92f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.88f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.80f, 0.85f, 0.93f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.92f, 0.94f, 0.97f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.86f, 0.89f, 0.95f, 1.00f);

	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.45f, 0.50f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = unique_color;
	style.Colors[ImGuiCol_PlotHistogram] = unique_color;
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.28f, 0.52f, 0.90f, 1.00f);

	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.90f, 0.92f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_TableBorderStrong] = style.Colors[ImGuiCol_Border];
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.86f, 0.88f, 0.91f, 1.00f);

	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.95f, 0.96f, 0.98f, 0.60f);

	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.38f, 0.60f, 0.95f, 0.30f);

	style.Colors[ImGuiCol_DragDropTarget] = unique_color;
	style.Colors[ImGuiCol_NavHighlight] = unique_color;
	style.Colors[ImGuiCol_NavWindowingHighlight] = unique_color;
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.12f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
}

void App::test_mode()
{
	ImGuiStyle& style = ImGui::GetStyle();

	spacing_and_padding_and_stuff(style);

}

void App::Shut_Down()
{
	m_isRunning = false;
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

	while (m_isRunning)
	{
		glfwPollEvents();

		if (glfwWindowShouldClose(window)) 
		{
			//std::quick_exit(0);
			m_isRunning = false;
			break;
		}

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

		ImVec2 mousePos = ImGui::GetMousePos();
		// 3. Offset the tooltip so it doesn't obscure the cursor (e.g., +15px, +15px)
		ImVec2 tooltipPos = ImVec2(mousePos.x + 15.0f, mousePos.y + 15.0f);
		// 4. Set position and pivot (Always pass ImGuiCond_Always so it updates every frame)
		ImGui::SetNextWindowPos(tooltipPos, ImGuiCond_Always);
		// 5. Render custom tooltip window
		if (ImGui::BeginTooltip())
		{
			ImGui::Text("Tracking mouse: (%.0f, %.0f)", mousePos.x, mousePos.y);
			ImGui::EndTooltip();
		}

		ImGui::GetMousePos();

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

		if (m_ctrlPressd) 
		{
			if (!ImGui::GetIO().WantCaptureKeyboard) 
			{
				if (ImGui::IsKeyPressed(ImGuiKey_1, false)) 
				{
					tabs[current_tab]->view_type = ROWS;
					tabs[current_tab]->icons_height = 24.0f;
					tabs[current_tab]->icons_width = 24.0f;
				}
				else if (ImGui::IsKeyPressed(ImGuiKey_2, false))
				{
					tabs[current_tab]->view_type = LARGE_THUMBNAILS;
					tabs[current_tab]->icons_height = 128.0f;
					tabs[current_tab]->icons_width = 128.0f;
				}
				else if (m_shiftPressed && ImGui::IsKeyPressed(ImGuiKey_C, false))
				{
					if (!tabs[current_tab]->current_path.empty())
					{
						ImGui::SetClipboardText(tabs[current_tab]->current_path.string().c_str());
						m_copyPathtimer.Restart();
					}
				}
			}
		}
		timed_tooltip("Current Path is copied to the clipboard", m_copyPathtimer, 2.0);

		files_navigation_panel(remainingHeight);

		stats_bar();

		if (show_icons_slider) icons_size_slider();

		if (!m_queuedTasks.empty()) 
		{
			for (size_t i = 0; i < m_queuedTasks.size(); i++) 
			{
				switch (m_queuedTasks[i]->task_type)	
				{
				case TaskType::COPY_PASTE:
				{
					if (m_activeCopyingTasks.load() < 3) 
					{
						Task* task = m_queuedTasks[i].get();

						m_queuedTasks[i]->start = std::chrono::steady_clock::now();

						m_activeCopyingTasks++;
						m_queuedTasks[i]->worker = std::thread([this, task]()
							{
								fs::path finalPath = tabs[current_tab]->fileExplorer->copy_paste(task->source, task->destination, task->sizeProgress, task->filesCountProgress, task->cancel_request, task->finished);
								m_index.addRecuirselyToDataBase(finalPath);
								m_activeCopyingTasks--;
							});

						tasks.emplace_back(std::move(m_queuedTasks[i]));
						m_queuedTasks.erase(m_queuedTasks.begin() + i);
						i--;
					}
					break;
				}
				case TaskType::_DELETE:
				{
					if (m_activeDeleteTasks.load() < 3)
					{
						Task* task = m_queuedTasks[i].get();

						m_activeDeleteTasks++;
						m_queuedTasks[i]->worker = std::thread([this, task]()
							{
								tabs[current_tab]->fileExplorer->recycleBin_delete(task->source);
								const fs::path& root = task->source;
								if (m_isSearcing) 
								{
									if (fs::is_directory(task->source))
									{
										std::erase_if(m_founds, [&root](const fs::path& path)
											{
												auto [root_it, path_it] = std::mismatch(root.begin(), root.end(), path.begin(), path.end());
												return root_it == root.end();
											}
										);
									}
									else 
									{
										std::erase(m_founds, root);
									}
								}
								// Remove from the index
								m_index.removeFromDataBase(task->source);
								task->finished = true;
								m_activeDeleteTasks--;
							});
						tasks.emplace_back(std::move(m_queuedTasks[i]));
						m_queuedTasks.erase(m_queuedTasks.begin() + i);
						i--;
						break;
					}
					break;
				}
				default:
					break;
				}
			}
			/*while ((int)tasks.size() <= 2 && !m_queuedTasks.empty()) 
			{
				switch (m_queuedTasks.front()->task_type)
				{
				case TaskType::COPY_PASTE: 
				{
					Task* task = m_queuedTasks.front().get();

					m_queuedTasks.front()->start = std::chrono::steady_clock::now();

					m_queuedTasks.front()->worker = std::thread([this, task]()
						{
							tabs[current_tab]->fileExplorer->copy_paste(task->source, task->destination, task->sizeProgress, task->filesCountProgress, task->cancel_request, task->finished);
						});

					tasks.emplace_back(std::move(m_queuedTasks.front()));
					m_queuedTasks.pop_front();
					break;
				}
				case TaskType::_DELETE: 
				{
					Task* task = m_queuedTasks.front().get();
					m_queuedTasks.front()->worker = std::thread([this, task]()
						{
							tabs[current_tab]->fileExplorer->remove(task->source);
							task->finished = true;
						});
					tasks.emplace_back(std::move(m_queuedTasks.front()));
					m_queuedTasks.pop_front();
					break;
				}
				default:
					break;
				}
			}*/
			if (!m_queuedTasks.empty()) 
			{ 
				if (ImGui::Begin("scheduled Tasks")) 
				{
					for (size_t i = 0; i < m_queuedTasks.size(); i++) 
					{
						ImGui::PushID(i);

						const std::unique_ptr<Task>& task = m_queuedTasks[i];

						switch (task->task_type)
						{
						case TaskType::NONE:
							break;
						case TaskType::COPY_PASTE: 
						{
							ImGui::SeparatorText("Copy/Paste Task");
							ImGui::Text(std::format("Copy From: {}", task->sourceName).c_str());
							ImGui::Text(std::format("Paste To: {}", task->destinationName).c_str());
							break;
						}
						case TaskType::_DELETE: 
						{
							ImGui::SeparatorText("Delete Task");
							ImGui::Text(std::format("Delete: {}", task->source.string()).c_str());
							break;
						}
						case TaskType::FIND_EMPTY_DIRECTORIES:
						{
						
							break;
						}
						case TaskType::FIND_DUPLICATE_FILES:
						{
						
							break;
						}
						case TaskType::FIND_LARGEST_FILES:
						{
						
							break;
						}
						default:
							break;
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove")) 
						{
							m_queuedTasks.erase(m_queuedTasks.begin() + i);
							i--;
						}
						ImGui::PopID();
					}
				}
				ImGui::End();
			}
		}
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

		render_debug_panel(); 

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
		tabs[current_tab]->copied_path.clear();
		tabs[current_tab]->copied_paths.clear();
		tabs[current_tab]->deleted_path.clear();
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
		bool isTabOpen = true;
		if (ImGui::BeginTabItem(tabLabel.c_str(), &isTabOpen, flags))
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
		if ((ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Middle, false)) || (!isTabOpen))
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
					Shut_Down();
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
		create_tab_helper();
	}
	if (m_ctrlPressd && ImGui::IsKeyPressed(ImGuiKey_T, false)) 
	{
		if (!ImGui::GetIO().WantCaptureKeyboard)
		{
			create_tab_helper();
		}
	}
	
	if (m_ctrlPressd && m_shiftPressed && ImGui::IsKeyPressed(ImGuiKey_Tab)) 
	{
		if (!ImGui::GetIO().WantCaptureKeyboard)
		{
			if (current_tab - 1 < 0) open_new_tab_factor = static_cast<int>(tabs.size()) - 1;
			else open_new_tab_factor = current_tab - 1;
		}
	}
	else if (m_ctrlPressd && ImGui::IsKeyPressed(ImGuiKey_Tab)) 
	{
		if (!ImGui::GetIO().WantCaptureKeyboard) 
		{
			open_new_tab_factor = (current_tab + 1) % static_cast<int>(tabs.size());
		}
	}
	
	if (m_ctrlPressd && ImGui::IsKeyPressed(ImGuiKey_W, false))
	{
		if (!ImGui::GetIO().WantCaptureKeyboard)
		{
			int tabToClose = current_tab;

			if (tabs.size() == 1)
			{
				Shut_Down();
			}

			tabs.erase(tabs.begin() + tabToClose);

			const int& size = static_cast<int>(tabs.size());
			if (current_tab >= size) current_tab = size - 1;

			open_new_tab_factor = current_tab;
		}
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

void ForestView::create_tab_helper() 
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

ImVec2 ForestView::GetSelectionStartScreenPos() const
{
	ImVec2 p = m_SelectionRecStartingPos;
	p.y -= ImGui::GetScrollY(); 
	return p;
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
					AccessResult checkAccess = tabs[current_tab]->fileExplorer->check_access(path_);
					if (checkAccess == AccessResult::ACCESS_DENIED)
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

		const bool& canGoBack = tabs[current_tab]->fileExplorer->can_go_back();
		ImGui::BeginDisabled(!canGoBack);
		if (ImGui::ImageButton("##<--", M_texturesManager.get_backwards_button_icon_texture_id(), ImVec2(15.0f, 15.0f)) || ImGui::IsKeyPressed(ImGuiKey_MouseX1, false))
		{
			go_back_helper();
		}
		ImGui::EndDisabled();
		if (canGoBack)
		{
			if (ImGui::IsKeyPressed(ImGuiKey_Backspace, false) || (m_altPressed && ImGui::IsKeyPressed(ImGuiKey_LeftArrow))) 
			{
				if (!ImGui::GetIO().WantCaptureKeyboard) 
				{
					go_back_helper();
				}
			}
		}

		ImGui::SameLine();

		const bool& canGoForward = tabs[current_tab]->fileExplorer->can_go_forward();
		ImGui::BeginDisabled(!canGoForward);
		if (ImGui::ImageButton("##-->", M_texturesManager.get_forward_button_icon_texture_id(), ImVec2(15.0f, 15.0f)) || ImGui::IsKeyPressed(ImGuiKey_MouseX2, false))
		{
			go_forward_helper();
		}
		ImGui::EndDisabled();
		if (canGoForward) 
		{
			if (m_altPressed && ImGui::IsKeyPressed(ImGuiKey_RightArrow))
			{
				if (!ImGui::GetIO().WantCaptureKeyboard)
				{
					go_forward_helper();
				}
			}
		}

		ImGui::PopStyleColor(1);

		ImGui::SameLine();

		if (ImGui::Button("Refresh"))
		{
			tabs[current_tab]->selected_path = "";
			tabs[current_tab]->selectionManager.Reset();
			tabs[current_tab]->entries_need_update = true;
			tabs[current_tab]->globalKeyWord = "";
			tabs[current_tab]->ThisDirectoryKeyWord = "";
			tabs[current_tab]->tree_need_update = true;
		}

		ImGui::SameLine();

		bool cantCreate = tabs[current_tab]->current_path.empty();
		ImGui::BeginDisabled(cantCreate);
		if (ImGui::Button("New"))
		{
			ImGui::OpenPopup("New?");
		}
		ImGui::EndDisabled();
		if (!cantCreate) 
		{
			if (m_ctrlPressd && m_shiftPressed && ImGui::IsKeyPressed(ImGuiKey_N, false)) 
			{
				if (!ImGui::GetIO().WantCaptureKeyboard) 
				{
					create_new_folder_helper();
				}
			}
		}


		ImGui::SameLine();

		bool isRootFolder = false;
		if (tabs[current_tab]->isSelecting) 
		{
			for (const SelectionRange& range : tabs[current_tab]->selectionManager.GetRanges()) 
			{
				for (int i = range.first ; i <= range.last ; i++) 
				{
					if (tabs[current_tab]->entries[i].entry.path() == tabs[current_tab]->entries[i].entry.path().root_path()) 
					{
						isRootFolder = true;
						break;
					}
				}
			}
		}
		else 
		{
			if (!tabs[current_tab]->selected_path.empty() && tabs[current_tab]->selected_path == tabs[current_tab]->selected_path.root_path()) 
			{
				isRootFolder = true;
			}
		}


		bool cantDelete = (tabs[current_tab]->selected_path.empty() && tabs[current_tab]->selectionManager.Empty()) || isRootFolder;
		ImGui::BeginDisabled(cantDelete);
		if (ImGui::Button("Delete"))
		{
			need_delete_confirmation = true;
			tabs[current_tab]->deleted_path = tabs[current_tab]->selected_path;
		}
		ImGui::EndDisabled();
		if (!cantDelete) 
		{
			if (ImGui::IsKeyPressed(ImGuiKey_Delete) && !ImGui::GetIO().WantCaptureKeyboard) 
			{
				need_delete_confirmation = true;
				tabs[current_tab]->deleted_path = tabs[current_tab]->selected_path;
			}
		}

		ImGui::SameLine();

		bool cantCopy = isRootFolder || (tabs[current_tab]->selected_path.empty() && tabs[current_tab]->selectionManager.Empty());
		///*ImGui::BeginDisabled(cantCopy);
		//if (ImGui::Button("Copy"))
		//{
		//	copy_helper();
		//}
		//ImGui::EndDisabled();*/
		if (!cantCopy && (!ImGui::GetIO().WantCaptureKeyboard && (m_ctrlPressd && ImGui::IsKeyPressed(ImGuiKey_C, false))))
		{
			bool permissonToCopy = true;
			if (tabs[current_tab]->isSelecting) 
			{
				for (const SelectionRange& range : tabs[current_tab]->selectionManager.GetRanges()) 
				{
					for (int j = range.first; j <= range.last; ++j) 
					{
						AccessResult res = tabs[current_tab]->fileExplorer->check_access(tabs[current_tab]->entries[j].entry.path());
						if (res == AccessResult::ACCESS_DENIED || res == AccessResult::LOCKED_OR_IN_USE)
						{
							permissonToCopy = false;
						}
					}
				}
			}
			if (permissonToCopy) copy_helper();
		}

		ImGui::SameLine();

		bool cantPaste = tabs[current_tab]->copied_path.empty() && tabs[current_tab]->copied_paths.empty();
		ImGui::BeginDisabled(cantPaste);
		if (ImGui::Button("Paste"))
		{
			paste_helper();
		}
		ImGui::EndDisabled();
		if (!cantPaste && (!ImGui::GetIO().WantCaptureKeyboard && (m_ctrlPressd && ImGui::IsKeyPressed(ImGuiKey_V, false)))) 
		{
			paste_helper();
		}

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
				m_emptyFoldersTask = std::make_unique<Task>(TaskType::FIND_EMPTY_DIRECTORIES);
				m_emptyFoldersTask->sourceName = Utils::reform_string(tabs[current_tab]->current_path.string(), 32);

				Task* task = m_emptyFoldersTask.get();

				m_emptyFoldersTask->worker = std::thread([this, task]()
					{
						tabs[current_tab]->fileExplorer->find_empty_folders(tabs[current_tab]->current_path, m_emptyFolders, task->cancel_request, task->finished);
					});
			}
			else if (!tabs[current_tab]->selected_path.empty()) 
			{
				m_emptyFoldersTask = std::make_unique<Task>(TaskType::FIND_EMPTY_DIRECTORIES);
				m_emptyFoldersTask->sourceName = Utils::reform_string(tabs[current_tab]->selected_path.string(), 32);

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
				m_duplicateFilesTask = std::make_unique<Task>(TaskType::FIND_DUPLICATE_FILES);
				m_duplicateFilesTask->sourceName = Utils::reform_string(tabs[current_tab]->current_path.string(), 32);

				Task* task = m_duplicateFilesTask.get();

				m_duplicateFilesTask->worker = std::thread([this, task]()
					{
						tabs[current_tab]->fileExplorer->find_duplicates(m_duplicateFiles, tabs[current_tab]->current_path, task->cancel_request, task->finished);
					});
			}
			else if (!tabs[current_tab]->selected_path.empty()) 
			{
				m_duplicateFilesTask = std::make_unique<Task>(TaskType::FIND_DUPLICATE_FILES);
				m_duplicateFilesTask->sourceName = Utils::reform_string(tabs[current_tab]->selected_path.string(), 32);

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
		ImGui::SameLine();
		// ##########################################################################

		ImGui::SetCursorPosX(1860.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::ImageButton("##Settings", M_texturesManager.get_settings_button_icon_texture_id(), ImVec2(15.0f, 15.0f)))
		{
			ImGui::OpenPopup("ViewSettings");
		}
		ImGui::PopStyleColor(1);

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

void ForestView::copy_helper()
{
	if (tabs[current_tab]->isSelecting)
	{
		tabs[current_tab]->copied_path.clear();
		tabs[current_tab]->copied_paths.clear();

		for (const SelectionRange& range : tabs[current_tab]->selectionManager.GetRanges())
		{
			for (int i = range.first; i <= range.last; i++)
			{
				tabs[current_tab]->copied_paths.emplace_back(tabs[current_tab]->entries[i].entry.path());
			}
		}
	}
	else
	{
		tabs[current_tab]->copied_paths.clear();
		tabs[current_tab]->copied_path = tabs[current_tab]->selected_path;
	}
}

void ForestView::paste_helper()
{
	if (!tabs[current_tab]->copied_paths.empty())
	{
		for (const fs::path& path : tabs[current_tab]->copied_paths)
		{
			fs::path src = path;
			fs::path dest = tabs[current_tab]->current_path;

			EntryDataReturn data = tabs[current_tab]->fileExplorer->get_total_files_size(path);

			std::unique_ptr<Task> copyTask = std::make_unique<Task>(TaskType::COPY_PASTE);
			copyTask->totalSize = data.directorySize;
			copyTask->totalfilesCount = data.filesCount;
			copyTask->source = src;
			copyTask->destination = dest;
			copyTask->sourceName = Utils::reform_string(src.filename().string(), 10);
			copyTask->destinationName = Utils::reform_string(dest.filename().string(), 10);

			m_queuedTasks.emplace_back(std::move(copyTask));
		}
		tabs[current_tab]->copied_paths.clear();
	}
	else
	{
		fs::path src = tabs[current_tab]->copied_path;
		fs::path dest = tabs[current_tab]->current_path;

		EntryDataReturn data = tabs[current_tab]->fileExplorer->get_total_files_size(tabs[current_tab]->copied_path);

		std::unique_ptr<Task> copyTask = std::make_unique<Task>(TaskType::COPY_PASTE);
		copyTask->totalSize = data.directorySize;
		copyTask->totalfilesCount = data.filesCount;
		copyTask->source = src;
		copyTask->destination = dest;
		copyTask->sourceName = Utils::reform_string(src.filename().string(), 10);
		copyTask->destinationName = Utils::reform_string(dest.filename().string(), 10);

		m_queuedTasks.emplace_back(std::move(copyTask));

		tabs[current_tab]->copied_path.clear();
	}
}

void ForestView::timed_tooltip(const std::string& msg, Timer& timer, const float& secondsNeeded)
{
	if (timer.IsRunning()) 
	{
		if (timer.GetElapsedSeconds() < secondsNeeded) 
		{
			ImGui::SetTooltip(msg.c_str());
		}
		else 
		{
			timer.Stop();
		}
	}
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
			create_new_folder_helper();	
		}
		if (ImGui::Selectable("Text Document"))
		{
			create_new_file_helper(NewType::TEXT_DOCUMENT);
		}
		if (ImGui::Selectable("Microsoft Word Document"))
		{
			create_new_file_helper(NewType::WORD);
		}
		if (ImGui::Selectable("Microsoft PowerPoint Presentation"))
		{
			create_new_file_helper(NewType::POWER_POINT);
		}
		if (ImGui::Selectable("Microsoft Excel Worksheet"))
		{
			create_new_file_helper(NewType::EXCEL);
		}
		if (ImGui::Selectable("WinRAR archive"))
		{
			create_new_file_helper(NewType::RAR);
		}
		if (ImGui::Selectable("WinRAR ZIP archive"))
		{
			create_new_file_helper(NewType::ZIP);
		}
		if (ImGui::Selectable("Microsoft Access Database"))
		{
			create_new_file_helper(NewType::ACCESS_DATABASE);
		}
		if (ImGui::Selectable("Microsoft Publisher Document"))
		{
			create_new_file_helper(NewType::PUBLISHER_DOCUMENT);
		}
		if (ImGui::Selectable("Bitmap image"))
		{
			create_new_file_helper(NewType::BITMAP);
		}
		
		ImGui::EndPopup();
	}
}

void ForestView::create_new_folder_helper()
{
	std::string name_of_the_folder = Utils::generate_new_folder_name(tabs[current_tab]->current_path, "New folder", NewType::FOLDER);
	fs::path finalNewPath = tabs[current_tab]->fileExplorer->create_folder(tabs[current_tab]->current_path, name_of_the_folder, NewType::FOLDER);

	if (!finalNewPath.empty())
	{
		m_index.addToDataBase(finalNewPath);
		if (m_isSearcing)
		{
			m_founds.emplace_back(finalNewPath);
		}
	}

	tabs[current_tab]->entries_need_update = true;
	tabs[current_tab]->tree_need_update = true;
	ImGui::CloseCurrentPopup();
}

void ForestView::create_new_file_helper(const NewType& type) 
{
	std::string name_of_the_file;
	fs::path finalNewPath = "";
	switch (type)
	{
	case NewType::TEXT_DOCUMENT:
	{
		name_of_the_file = Utils::generate_new_folder_name(tabs[current_tab]->current_path, "New Text Document.txt", NewType::REG_FILE);
		fs::path finalNewPath = tabs[current_tab]->fileExplorer->create_folder(tabs[current_tab]->current_path, name_of_the_file, NewType::TEXT_DOCUMENT);
		break;
	}     
	case NewType::POWER_POINT:
	{
		name_of_the_file = Utils::generate_new_folder_name(tabs[current_tab]->current_path, "New Microsoft PowerPoint Presentation.pptx", NewType::REG_FILE);
		fs::path finalNewPath = tabs[current_tab]->fileExplorer->create_folder(tabs[current_tab]->current_path, name_of_the_file, NewType::POWER_POINT);
		break;
	}  
	case NewType::WORD:
	{
		name_of_the_file = Utils::generate_new_folder_name(tabs[current_tab]->current_path, "New Microsoft Word Document.docx", NewType::REG_FILE);
		fs::path finalNewPath = tabs[current_tab]->fileExplorer->create_folder(tabs[current_tab]->current_path, name_of_the_file, NewType::WORD);
		break;
	}  
	case NewType::RAR:
	{
		name_of_the_file = Utils::generate_new_folder_name(tabs[current_tab]->current_path, "New WinRAR archive.rar", NewType::REG_FILE);
		fs::path finalNewPath = tabs[current_tab]->fileExplorer->create_folder(tabs[current_tab]->current_path, name_of_the_file, NewType::RAR);
		break;
	}  
	case NewType::ZIP:
	{
		name_of_the_file = Utils::generate_new_folder_name(tabs[current_tab]->current_path, "New WinRAR ZIP archive.zip", NewType::REG_FILE);
		fs::path finalNewPath = tabs[current_tab]->fileExplorer->create_folder(tabs[current_tab]->current_path, name_of_the_file, NewType::ZIP);
		break;
	}  
	case NewType::EXCEL:
	{
		name_of_the_file = Utils::generate_new_folder_name(tabs[current_tab]->current_path, "New Microsoft Excel Worksheet.xlsx", NewType::REG_FILE);
		fs::path finalNewPath = tabs[current_tab]->fileExplorer->create_folder(tabs[current_tab]->current_path, name_of_the_file, NewType::EXCEL);
		break;
	}  
	case NewType::BITMAP:
	{
		name_of_the_file = Utils::generate_new_folder_name(tabs[current_tab]->current_path, "New Bitmap image.bmp", NewType::REG_FILE);
		fs::path finalNewPath = tabs[current_tab]->fileExplorer->create_folder(tabs[current_tab]->current_path, name_of_the_file, NewType::BITMAP);
		break;
	}  
	case NewType::PUBLISHER_DOCUMENT:
	{
		name_of_the_file = Utils::generate_new_folder_name(tabs[current_tab]->current_path, "New Microsoft Publisher Document.pub", NewType::REG_FILE);
		fs::path finalNewPath = tabs[current_tab]->fileExplorer->create_folder(tabs[current_tab]->current_path, name_of_the_file, NewType::PUBLISHER_DOCUMENT);
		break;
	}  
	case NewType::ACCESS_DATABASE:
	{
		name_of_the_file = Utils::generate_new_folder_name(tabs[current_tab]->current_path, "New Microsoft Access Database.accdb", NewType::REG_FILE);
		fs::path finalNewPath = tabs[current_tab]->fileExplorer->create_folder(tabs[current_tab]->current_path, name_of_the_file, NewType::ACCESS_DATABASE);
		break;
	} 
	default:
		break;
	}

	if (!finalNewPath.empty())
	{
		m_index.addToDataBase(finalNewPath);
		if (m_isSearcing)
		{
			m_founds.emplace_back(finalNewPath);
		}
	}

	tabs[current_tab]->entries_need_update = true;
	tabs[current_tab]->tree_need_update = true;
	ImGui::CloseCurrentPopup();
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
			tabs[current_tab]->entries_need_update = true;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Selectable("Last Modified time"))
		{
			tabs[current_tab]->fileExplorer->sort_entries([](const Entry& a, const Entry& b) {return a.entry.last_write_time() > b.entry.last_write_time();});
			tabs[current_tab]->entries_need_update = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

// Deleting Warning popup
void ForestView::delete_warning()
{
	if (need_delete_confirmation)
	{
		ImGui::OpenPopup("Delete Warning?");
		need_delete_confirmation = false;
	}
	if (ImGui::BeginPopupModal("Delete Warning?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure you want to delete this folder?\nThe deleted file will be deleted permanently and will not go to the recycle bin \nThis action cannot be undone.");
		ImGui::Separator();

		if (ImGui::Button("YES"))
		{
			// Syncing the Tasks (Duplicates, Empty Folders, Largest Files) =======================
			// Sync the Empty Folders Deletion
			if (tabs[current_tab]->isSelecting) 
			{
				for (const SelectionRange& range : tabs[current_tab]->selectionManager.GetRanges())
				{
					for (int i = range.first; i <= range.last; i++)
					{
						const fs::path& path = tabs[current_tab]->entries[i].entry.path();
						if (is_searching_for_empty_folders)
						{
							if (fs::is_directory(path))
							{
								std::erase(m_emptyFolders, path);
							}
						}
						if (is_searching_for_duplicates_files)
						{
							if (fs::is_regular_file(path)) 
							{
								auto targetIt = std::find_if(m_duplicateFiles.begin(), m_duplicateFiles.end(),
									[&path](const Duplicate& duplicateObj) {
										return path == duplicateObj.duplicateEntry.path();
									});

								int tempId = 0;
								size_t count_deleted = std::erase_if(m_duplicateFiles, [&path, &tempId](const Duplicate& duplicateObj)
									{
										if (path == duplicateObj.duplicateEntry.path())
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
						}
						if (is_searching_for_largest_files)
						{
							if (fs::is_regular_file(path))
							{
								std::erase(m_largestFiles, path);
							}
						}
					}
				}
			}
			else 
			{
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
			}

			// =====================================================================================


			if (tabs[current_tab]->fav.is_exist(tabs[current_tab]->deleted_path))
			{
				tabs[current_tab]->fav.delete_fav(fs::directory_entry(tabs[current_tab]->deleted_path));
			}
			tabs[current_tab]->recents.remove(fs::directory_entry(tabs[current_tab]->deleted_path));

			if (!tabs[current_tab]->selectionManager.Empty()) 
			{
				for (const SelectionRange& range : tabs[current_tab]->selectionManager.GetRanges())
				{
					for (int i = range.first; i <= range.last; i++)
					{
						const fs::path& path = tabs[current_tab]->entries[i].entry.path();
						std::unique_ptr<Task> deleteTask = std::make_unique<Task>(TaskType::_DELETE);
						deleteTask->source = path;
						deleteTask->sourceName = Utils::reform_string(path.filename().string(), 10);

						m_queuedTasks.emplace_back(std::move(deleteTask));
					}
				}
			}
			else 
			{
				fs::path src = tabs[current_tab]->deleted_path;

				std::unique_ptr<Task> deleteTask = std::make_unique<Task>(TaskType::_DELETE);
				deleteTask->source = tabs[current_tab]->deleted_path;
				deleteTask->sourceName = Utils::reform_string(tabs[current_tab]->deleted_path.filename().string(), 10);

				m_queuedTasks.emplace_back(std::move(deleteTask));
			}

			tabs[current_tab]->selectionManager.Clear();
			if (tabs[current_tab]->isSelecting) 
			{
				tabs[current_tab]->isSelecting = false;
			}

			tabs[current_tab]->deleted_path = "";
			tabs[current_tab]->selected_path.clear();
			tabs[current_tab]->copied_path.clear();
			tabs[current_tab]->copied_paths.clear();
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
				m_largestFilesTask = std::make_unique<Task>(TaskType::FIND_LARGEST_FILES);
				m_largestFilesTask->sourceName = Utils::reform_string(tabs[current_tab]->current_path.string(), 32);

				Task* task = m_largestFilesTask.get();

				m_largestFilesTask->worker = std::thread([this, task]()
					{
						tabs[current_tab]->fileExplorer->find_largest_files(m_largestFiles, tabs[current_tab]->current_path, task->cancel_request, task->finished, MaxSize);
					});
			}
			else if (!tabs[current_tab]->selected_path.empty()) 
			{
				m_largestFilesTask = std::make_unique<Task>(TaskType::FIND_LARGEST_FILES);
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
				m_isSearcing = false;
				FileNameSearch.clear();
				m_founds.clear();
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
				m_isSearcing = true;
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
								std::error_code ec;
								fileSize size = tabs[current_tab]->fileExplorer->decide_file_size_format(foundsEntry.file_size(ec));
								ImGui::Selectable(std::format("{:.2f} {}", size.size, size.label).c_str());
							}

						}
					}
					ImGui::EndTable();
				}
			}
			else 
			{
				m_isSearcing = false;
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
					if (m_isSearcing) 
					{
						for (auto& path : m_founds)
						{
							if (path == tabs[current_tab]->selected_path)
							{
								path = rename.finalPath;
							}
						}
					}

					m_index.renameFromDataBase(tabs[current_tab]->selected_path, rename.finalPath);

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

		// Copy 
		if (ImGui::Selectable("Copy"))
		{
			copy_helper();
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

// Files RIGHT CLICK
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
					if (m_isSearcing) 
					{
						for (auto& path : m_founds)
						{
							if (path == tabs[current_tab]->selected_path)
							{
								path = rename.finalPath;
							}
						}
					}
					m_index.renameFromDataBase(tabs[current_tab]->selected_path, rename.finalPath);

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

		// Copy 
		if (ImGui::Selectable("Copy")) 
		{
			copy_helper();
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
	
// Selections RIGHT CLICK
void ForestView::selection_right_click_popup(const std::vector<SelectionRange>& selections) 
{
	if(tabs[current_tab]->need_selection_right_click_call)
	{
		ImGui::OpenPopup("SelectionRightClickPopUp");
		tabs[current_tab]->need_selection_right_click_call = false;
	}
	static bool selection_popup_was_open = false;
	bool selection_popup_is_open = ImGui::BeginPopup("SelectionRightClickPopUp");
	if (selection_popup_is_open)
	{
		if (ImGui::Selectable("Copy")) 
		{
			const std::vector<Entry>& entries = tabs[current_tab]->entries;
			bool canCopy = true;
			for (const SelectionRange& range : selections)
			{
				if (!canCopy) break;
				if (range.first < 0 || range.last >= static_cast<int>(entries.size()) || range.first > range.last)
				{
					canCopy = false;
					break;
				}
				for (int i = range.first; i <= range.last; ++i)
				{
					AccessResult res = tabs[current_tab]->fileExplorer->check_access(entries[i].entry.path());
					if (res == AccessResult::ACCESS_DENIED || res == AccessResult::LOCKED_OR_IN_USE)
					{
						canCopy = false;
						break;
					}
				}
			}
			if (!canCopy) 
			{
				need_access_denied_popup = true;
				ImGui::CloseCurrentPopup();
			}
			else if (canCopy) 
			{
				copy_helper();
				ImGui::CloseCurrentPopup();
			}

		}
		if (ImGui::Selectable("Delete")) 
		{
			const std::vector<Entry>& entries = tabs[current_tab]->entries;
			bool canDelete = true;
			for (const SelectionRange& range : selections) 
			{
				if (!canDelete) break;
				if (range.first < 0 || range.last >= static_cast<int>(entries.size()) || range.first > range.last)
				{
					canDelete = false;
					break;
				}
				for (int i = range.first; i <= range.last; ++i) 
				{
					AccessResult res = tabs[current_tab]->fileExplorer->check_access(entries[i].entry.path());
					if (res == AccessResult::ACCESS_DENIED || res == AccessResult::LOCKED_OR_IN_USE) 
					{
						canDelete = false;
						break;
					}
				}
			}

			if (!canDelete) 
			{
				need_access_denied_popup = true;
				ImGui::CloseCurrentPopup();
			}
			else if (canDelete)
			{
				need_delete_confirmation = true;
			}

		}
		ImGui::EndPopup();
	}
	if (selection_popup_was_open && !selection_popup_is_open)
	{
		tabs[current_tab]->selected_path = "";
	}
	selection_popup_was_open = selection_popup_is_open;
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
				tabs[current_tab]->entries_need_update = true;
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
			tabs[current_tab]->selectionManager.Reset();

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
		ImGuiIO io = ImGui::GetIO();
		m_ctrlPressd = io.KeyCtrl;
		m_shiftPressed = io.KeyShift;
		m_altPressed = io.KeyAlt;
		if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				tabs[current_tab]->selectionManager.ResetAnchor();
				tabs[current_tab]->selected_path = "";
				if (m_ctrlPressd)
				{
					tabs[current_tab]->selectedEntriesSnapShot = tabs[current_tab]->selectionManager.GetRanges();
				}
				else if (m_shiftPressed) 
				{
					tabs[current_tab]->selectedEntriesSnapShot = tabs[current_tab]->selectionManager.GetRanges();
				}
				else
				{
					tabs[current_tab]->selectedEntriesSnapShot.clear();

					tabs[current_tab]->selectionManager.Clear();

					tabs[current_tab]->isSelecting = false;
				}

				m_SelectionRecStartingPos = ImGui::GetMousePos();
				m_SelectionRecStartingPos.y += ImGui::GetScrollY();
				m_isDraggingSelection = true;
			}
		}

		// asyncc folder structure copying ======================================================== 
		if (tabs[current_tab]->folder_structure.valid() && tabs[current_tab]->folder_structure.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			tabs[current_tab]->copied_structure = tabs[current_tab]->folder_structure.get();
			ImGui::SetClipboardText(tabs[current_tab]->copied_structure.c_str());
			tabs[current_tab]->is_coping_structure = false;
			tabs[current_tab]->copied_structure = "";
		}


		// Drawing the selection rectagle
		if (m_isDraggingSelection) 
		{
			ImVec2 childPos = ImGui::GetWindowPos();
			ImVec2 childSize = ImGui::GetWindowSize();
			ImVec2 childMax = ImVec2(childPos.x + childSize.x, childPos.y + childSize.y);

			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			ImVec2 p_min = ImVec2(ImMin(GetSelectionStartScreenPos(), ImGui::GetMousePos()));
			ImVec2 p_max = ImVec2(ImMax(GetSelectionStartScreenPos(), ImGui::GetMousePos()));

			ImU32 SelectionColor = IM_COL32(unique_color.x * 255, unique_color.y * 255, unique_color.z * 255, (unique_color.w * 255) / 2);  // Transparent green fill
			ImU32 SelectionOutlineColor = IM_COL32(unique_color.x * 255, unique_color.y * 255, unique_color.z * 255, unique_color.w * 255); // Solid green border outline

			const float currentScrollOffset = ImGui::GetScrollY();
			const float MaxScrollOffset = ImGui::GetScrollMaxY();
			const float MaxSpeed = 150.0f;

			if (ImGui::GetMousePos().y > childMax.y && currentScrollOffset != MaxScrollOffset) // Going Down
			{
				const float goDown = (ImGui::GetMousePos().y - childMax.y);
				ImGui::SetScrollY(currentScrollOffset + (std::min)(goDown, MaxSpeed));
			}
			else if (ImGui::GetMousePos().y < childPos.y && currentScrollOffset != 0.0f) // Going Up
			{
				const float goUp = (childPos.y - ImGui::GetMousePos().y);
				ImGui::SetScrollY(currentScrollOffset - (std::min)(goUp,MaxSpeed));

			}
			draw_list->AddRectFilled(p_min, p_max, SelectionColor, 3.0f);
			draw_list->AddRect(p_min, p_max, SelectionOutlineColor, 3.0f, 0, 1.0f);
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			{
				m_isDraggingSelection = false;
			}
		}

		// Ctrl+A selcetion 
		if (!io.WantCaptureKeyboard) 
		{
			if (m_ctrlPressd && ImGui::IsKeyPressed(ImGuiKey_A, false)) 
			{
				tabs[current_tab]->selected_path = "";
				tabs[current_tab]->selectionManager.Reset();
				for (size_t i = 0; i < tabs[current_tab]->entries.size(); ++i)
				{
					const Entry& entry = tabs[current_tab]->entries[i];
					AccessResult check = tabs[current_tab]->fileExplorer->check_access(entry.entry.path());
					if (check == AccessResult::ACCESS_DENIED)
					{
						need_access_denied_popup = true;
					}
					else 
					{
						tabs[current_tab]->selectionManager.AddSingleSelection(i);
					}
				}
			}
		}
	}
	ImGui::EndChild();
}

void ForestView::render_entries()
{
	if (tabs[current_tab]->selectionManager.Empty()) 
	{
		tabs[current_tab]->isSelecting = false;
	}
	else 
	{
		tabs[current_tab]->isSelecting = true;
	}


	if (m_isDraggingSelection) 
	{
		if (m_ctrlPressd) 
		{
			tabs[current_tab]->selectionManager.SetRanges(tabs[current_tab]->selectedEntriesSnapShot);
		}
		else if (m_shiftPressed)
		{
			tabs[current_tab]->selectionManager.SetRanges(tabs[current_tab]->selectedEntriesSnapShot);
		}
		else 
		{
			tabs[current_tab]->selectionManager.Reset();
		}
	}

	if (tabs[current_tab]->view_type == LARGE_THUMBNAILS)
	{
		float avalWidth = ImGui::GetContentRegionAvail().x;
		int columnscount = (int)(avalWidth / (tabs[current_tab]->icons_width + tabs[current_tab]->padding));
		if (columnscount < 1) columnscount = 1;
		ImGui::Columns(columnscount, 0, false);

		for (size_t i = 0; i < tabs[current_tab]->entries.size(); ++i)
		{
			ImGui::PushID(i);

			bool overlaps = false;
			if (m_isDraggingSelection)
			{
				ImVec2 square_min = ImGui::GetCursorScreenPos();
				ImVec2 square_max = ImVec2(square_min.x + tabs[current_tab]->icons_width, square_min.y + tabs[current_tab]->icons_height);

				ImVec2 selection_min = ImMin(ImGui::GetMousePos(), GetSelectionStartScreenPos());
				ImVec2 selection_max = ImMax(ImGui::GetMousePos(), GetSelectionStartScreenPos());

				overlaps = (square_min.x < selection_max.x && square_max.x > selection_min.x) && (square_min.y < selection_max.y && square_max.y > selection_min.y);

			}

			bool is_selected = false;
			if (overlaps)
			{
				if (m_ctrlPressd)
				{
					tabs[current_tab]->selectionManager.toggleSingleSelection(i);
					is_selected = tabs[current_tab]->selectionManager.IsSelected(i);
				}
				else 
				{
					tabs[current_tab]->selectionManager.AddSingleSelection(i);
					is_selected = true;
				}
			}
			else if (tabs[current_tab]->isSelecting)
			{
				is_selected = tabs[current_tab]->selectionManager.IsSelected(i);
			}
			else
			{
				is_selected = (tabs[current_tab]->entries[i].entry.path() == tabs[current_tab]->selected_path);
			}



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
		const float tableTopContentX = ImGui::GetCursorScreenPos().x;
		if (ImGui::BeginTable("entries", 4, ImGuiTableFlags_Resizable, ImVec2(1000.0f, 0.0f)))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Last Modified");
			ImGui::TableSetupColumn("Type");
			ImGui::TableSetupColumn("Size");
			ImGui::TableHeadersRow();


			ImGuiListClipper Clipper;
			Clipper.Begin(tabs[current_tab]->entries.size());
			if (m_isDraggingSelection)
			{
				const float tableTopContentY = ImGui::GetCursorScreenPos().y + ImGui::GetScrollY();

				const float maxEntriesX = 1000.0;

				const float SelectionStartY = m_SelectionRecStartingPos.y; // already content-space
				const float SelectionStartX = m_SelectionRecStartingPos.x;

				const float mouseContentY = ImGui::GetMousePos().y + ImGui::GetScrollY();
				const float mouseContentX = ImGui::GetMousePos().x;

				const float selMinY = (std::min)(SelectionStartY, mouseContentY) - tableTopContentY;
				const float selMaxY = (std::max)(SelectionStartY, mouseContentY) - tableTopContentY;

				const float selMinX = (std::min)(SelectionStartX, mouseContentX);
				const float selMaxX = (std::max)(SelectionStartX, mouseContentX);

				bool checkForTheY = (selMinX < tableTopContentX + maxEntriesX && selMaxX > tableTopContentX) ? true : false;
				if (checkForTheY)
				{
					const float rowYHeight = tabs[current_tab]->icons_height + ImGui::GetStyle().CellPadding.y * 2.0f;
					const int totalEntries = (int)tabs[current_tab]->entries.size();

					if (totalEntries > 0)
					{
						const float totalContentHeight = totalEntries * rowYHeight;

						if (selMinY < totalContentHeight && selMaxY > 0.0f)
						{
							int lastIndex = totalEntries - 1;

							int minIndex = (int)std::floor(selMinY / rowYHeight);
							int maxIndex = (int)std::floor(selMaxY / rowYHeight);

							minIndex = std::clamp(minIndex, 0, lastIndex);
							maxIndex = std::clamp(maxIndex, 0, lastIndex);

							if (m_ctrlPressd)
							{
								tabs[current_tab]->selectionManager.ToggleRange(minIndex, maxIndex);
							}
							else if (m_shiftPressed)
							{
								tabs[current_tab]->selectionManager.AddRange(minIndex, maxIndex);
							}
							else
							{
								tabs[current_tab]->selectionManager.SelectRange(minIndex, maxIndex);
							}
						}
						
					}
				}
			}
			while (Clipper.Step()) 
			{
				for (size_t i = Clipper.DisplayStart; i < Clipper.DisplayEnd; ++i)
				{
					ImGui::TableNextRow();

					ImGui::PushID(i);

					const float& icon_height = tabs[current_tab]->icons_height;
					// ======================================== Name ==============================================
					ImGui::TableNextColumn();

					bool is_selected; 

					if (tabs[current_tab]->selectionManager.Empty()) 
					{
						is_selected = (tabs[current_tab]->entries[i].entry.path() == tabs[current_tab]->selected_path) ? true : false;
					}
					else 
					{
						is_selected = tabs[current_tab]->selectionManager.IsSelected(i);
					}


					ImGui::Image(tabs[current_tab]->entries[i].textureId, ImVec2(tabs[current_tab]->icons_width, icon_height));

					ImGui::SameLine();

					ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.005f, 0.5f));

					ImGui::Selectable(tabs[current_tab]->entries[i].name.c_str(), is_selected, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0, icon_height));
					handle_entries_mouse_click(tabs[current_tab]->entries[i].entry, i);
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

#ifdef _DEBUG
					ImGui::SameLine();
					ImGui::Text(std::format("  [ {} ]", std::to_string(tabs[current_tab]->entries[i].id)).c_str());
#endif
					ImGui::PopID();
				}

			}
			ImGui::EndTable();
		}
	}

	if (!ImGui::GetIO().WantCaptureKeyboard && !m_ctrlPressd && !m_shiftPressed) 
	{
		for (size_t i = 0; i < 26; i++) 
		{
			if (ImGui::IsKeyPressed(s_alphabetMappings[i].key) && !m_ctrlPressd && !m_shiftPressed)
			{
				WORD activeLangId = LOWORD(GetKeyboardLayout(0));	
				const std::wstring& selectedPathWstring = tabs[current_tab]->selected_path.filename().wstring();
				const std::vector<Entry>& entriesRef = tabs[current_tab]->entries;

				switch (activeLangId)
				{
				case 0x0409: // ENGLISH  
				{	

					if (!tabs[current_tab]->selected_path.empty() && (selectedPathWstring[0] == s_alphabetMappings[i].englishLower || selectedPathWstring[0] == s_alphabetMappings[i].englishUpper))
					{
						int selected_pathIdx = -1;
						for (size_t idx = 0 ; idx < entriesRef.size(); ++idx)
						{
							const fs::path& entryPath = entriesRef[idx].entry.path();
							if (entryPath == tabs[current_tab]->selected_path)
							{
								selected_pathIdx = idx;
								break;
							}
						}
						if (selected_pathIdx == -1) 
						{
							break;
						}
						int selected_PathIdxTemp = selected_pathIdx + 1;
						while (selected_PathIdxTemp != selected_pathIdx) 
						{
							if (entriesRef[selected_PathIdxTemp].entry.path().empty()) 
							{
								selected_PathIdxTemp = (selected_PathIdxTemp + 1) % (int)entriesRef.size();
								continue;
							}
							if (entriesRef[selected_PathIdxTemp].entry.path().filename().wstring()[0] == s_alphabetMappings[i].englishLower ||
								entriesRef[selected_PathIdxTemp].entry.path().filename().wstring()[0] == s_alphabetMappings[i].englishUpper)
							{
								AccessResult s = tabs[current_tab]->fileExplorer->check_access(entriesRef[selected_PathIdxTemp].entry.path());
								if (s == AccessResult::ACCESS_DENIED)
								{
									need_access_denied_popup = true;
									break;
								}
								tabs[current_tab]->selectionManager.Reset();
								tabs[current_tab]->selectionManager.SetAnchor(selected_PathIdxTemp);
								tabs[current_tab]->selected_path = entriesRef[selected_PathIdxTemp].entry.path();
								break;
							}
							selected_PathIdxTemp = (selected_PathIdxTemp + 1) % (int)entriesRef.size();
						}
					
					}
					else 
					{
						for (size_t j = 0; j < entriesRef.size(); ++j)
						{
							const Entry& entry = entriesRef[j];
							if (entry.entry.path().empty()) continue;
							if (entry.entry.path().filename().wstring()[0] == s_alphabetMappings[i].englishLower ||
								entry.entry.path().filename().wstring()[0] == s_alphabetMappings[i].englishUpper)
							{
								AccessResult s = tabs[current_tab]->fileExplorer->check_access(entry.entry.path());
								if (s == AccessResult::ACCESS_DENIED)
								{
									need_access_denied_popup = true;
									break;
								}
								tabs[current_tab]->selectionManager.Reset();
								tabs[current_tab]->selectionManager.SetAnchor(j);
								tabs[current_tab]->selected_path = entry.entry.path();
								break;
							}
						}
					}

					break;
				}
				case 0x0c01: // ARABIC
				{
				
					if (!tabs[current_tab]->selected_path.empty() && selectedPathWstring[0] == s_alphabetMappings[i].arabicChar)
					{
						int selected_pathIdx = -1;
						for (size_t idx = 0; idx < entriesRef.size(); ++idx)
						{
							const fs::path& entryPath = entriesRef[idx].entry.path();
							if (entryPath == tabs[current_tab]->selected_path)
							{
								selected_pathIdx = idx;
								break;
							}
						}
						if (selected_pathIdx == -1)
						{
							break;
						}
						int selected_PathIdxTemp = selected_pathIdx + 1;
						while (selected_PathIdxTemp != selected_pathIdx)
						{
							if (entriesRef[selected_PathIdxTemp].entry.path().empty())
							{
								selected_PathIdxTemp = (selected_PathIdxTemp + 1) % (int)entriesRef.size();
								continue;
							}
							if (entriesRef[selected_PathIdxTemp].entry.path().filename().wstring()[0] == s_alphabetMappings[i].arabicChar)
							{
								AccessResult s = tabs[current_tab]->fileExplorer->check_access(entriesRef[selected_PathIdxTemp].entry.path());
								if (s == AccessResult::ACCESS_DENIED)
								{
									need_access_denied_popup = true;
									break;
								}
								tabs[current_tab]->selectionManager.Reset();
								tabs[current_tab]->selectionManager.SetAnchor(selected_PathIdxTemp);
								tabs[current_tab]->selected_path = entriesRef[selected_PathIdxTemp].entry.path();
								break;
							}
							selected_PathIdxTemp = (selected_PathIdxTemp + 1) % (int)entriesRef.size();
						}

					}
					else
					{
						for (size_t j = 0; j < entriesRef.size(); ++j)
						{
							const Entry& entry = entriesRef[j];
							if (entry.entry.path().empty()) continue;
							if (entry.entry.path().filename().wstring()[0] == s_alphabetMappings[i].arabicChar)
							{
								AccessResult s = tabs[current_tab]->fileExplorer->check_access(entry.entry.path());
								if (s == AccessResult::ACCESS_DENIED)
								{
									need_access_denied_popup = true;
									break;
								}
								tabs[current_tab]->selectionManager.Reset();
								tabs[current_tab]->selectionManager.SetAnchor(j);
								tabs[current_tab]->selected_path = entry.entry.path();
								break;
							}
						}
					}
					break;
				}
				default:
					break;
				}

				break;
			}
		}
	}

}


void ForestView::handle_entries_mouse_click(const fs::directory_entry& entry, int idx)
{
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			AccessResult checkAccess = tabs[current_tab]->fileExplorer->check_access(entry.path());
			if (checkAccess == AccessResult::ACCESS_DENIED || checkAccess == AccessResult::LOCKED_OR_IN_USE)
			{
				need_access_denied_popup = true;
				return;
			}
			
			// Clear the selected Items
			if (tabs[current_tab]->isSelecting)
			{
				tabs[current_tab]->selectionManager.Reset();
			}

			if (entry.is_directory())
			{
				left_double_click(entry);
			}
			else if (entry.is_regular_file())
			{
				const fs::path& parent_path = entry.path().parent_path();
				if (tabs[current_tab]->current_path != parent_path) // If i double click on a file that is not in the current path (curret directory)
				{
					const fs::directory_entry parent = fs::directory_entry(parent_path);
					left_double_click(parent);
				}
				else // if i double click on a file that is in the current path (current directory)
				{
					std::wstring parameters = L"/c start \"\" \"" + entry.path().wstring() + L"\"";
					HINSTANCE result = ShellExecuteW(nullptr, L"open", L"cmd.exe", parameters.c_str(), nullptr, SW_HIDE);
				}
			}
		}
		else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			if (tabs[current_tab]->isSelecting && tabs[current_tab]->selectionManager.IsSelected(idx))
			{
				tabs[current_tab]->need_selection_right_click_call = true;
			}
			else
			{
				tabs[current_tab]->selectionManager.Reset();

				AccessResult checkAccess = tabs[current_tab]->fileExplorer->check_access(entry.path());
				if (checkAccess == AccessResult::ACCESS_DENIED || checkAccess == AccessResult::LOCKED_OR_IN_USE)
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
		}
		else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			if (!m_ctrlPressd && !m_shiftPressed) 
			{
				AccessResult checkAccess = tabs[current_tab]->fileExplorer->check_access(entry.path());
				if (checkAccess == AccessResult::ACCESS_DENIED || checkAccess == AccessResult::LOCKED_OR_IN_USE)
				{
					need_access_denied_popup = true;
					return;
				}
			}
			ImGuiIO io = ImGui::GetIO();
			bool ctrlPressed = io.KeyCtrl;
			bool rightShiftPressed = io.KeyShift;
			if (ctrlPressed && !m_isDraggingSelection)
			{
				if (idx != -1)
				{
					tabs[current_tab]->selectionManager.SetAnchor(idx);
				}
				if (!tabs[current_tab]->selected_path.empty()) 
				{
					for (size_t i = 0; i < tabs[current_tab]->entries.size(); ++i) 
					{
						if (tabs[current_tab]->entries[i].entry.path() == tabs[current_tab]->selected_path) 
						{
							tabs[current_tab]->selectionManager.toggleSingleSelection(i);
							break;
						}
					}
				}
				if (idx != -1) tabs[current_tab]->selectionManager.toggleSingleSelection(idx);
				
				tabs[current_tab]->selected_path.clear();
			}
			else if (rightShiftPressed && !m_isDraggingSelection) 
			{
				if (tabs[current_tab]->selected_path.empty() && !tabs[current_tab]->isSelecting)
				{
					left_click(entry);
					tabs[current_tab]->selectionManager.SetAnchor(idx);
				}
				else 
				{
					if (idx != -1) tabs[current_tab]->selectionManager.SelectAnchoredRange(idx);
				}
			}
			else
			{
				if (!m_isDraggingSelection)
				{
					// Clear the selected Items
					if (tabs[current_tab]->isSelecting) 
					{
						tabs[current_tab]->selectionManager.Clear();
					}
					if (idx != -1) 
					{
						tabs[current_tab]->selectionManager.SetAnchor(idx);
					}
					left_click(entry);
				}
			}
		}
	}
}

void ForestView::render_debug_panel()
{
	if (ImGui::Begin("Debug Panel")) 
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

		WORD langId = LOWORD(GetKeyboardLayout(0));

		WORD CurrentLanguage = PRIMARYLANGID(langId);
		if (CurrentLanguage == LANG_ENGLISH) 
		{
			ImGui::Text("Current Language is: English");
		}
		else if (CurrentLanguage == LANG_ARABIC)
		{
			ImGui::Text("Current Language is: Arabic");
		}

		ImGui::SeparatorText("Tasks Stuff");
		ImGui::Text(std::format("Number of Copy Tasks: {}", get_copy_paste_Tasks_count()).c_str());
		ImGui::Text(std::format("Number of Delete Tasks: {}", get_delete_Tasks_count()).c_str());
		ImGui::Text(std::format("Number of Active Copy Tasks: {}", m_activeCopyingTasks.load()).c_str());
		ImGui::Text(std::format("Number of Active Delete Tasks: {}", m_activeDeleteTasks.load()).c_str());

		ImGui::SeparatorText("Selected Entries");

		std::string flag4 = (m_isDraggingSelection) ? "True" : "False";
		ImGui::Text("m_isDraggingSelection Flag: "); ImGui::SameLine();
		ImGui::Text(flag4.c_str());

		std::string flag3 = (tabs[current_tab]->isSelecting) ? "True" : "False";
		ImGui::Text("isSelecting Flag: "); ImGui::SameLine();
		ImGui::Text(flag3.c_str());

		std::string flag5 = (m_ctrlPressd) ? "True" : "False";
		ImGui::Text("ctrl Pressed Flag: "); ImGui::SameLine();
		ImGui::Text(flag5.c_str());

		std::string flag6 = (m_shiftPressed) ? "True" : "False";
		ImGui::Text("shift Pressed Flag: "); ImGui::SameLine();
		ImGui::Text(flag6.c_str());

		for (const SelectionRange& range : tabs[current_tab]->selectionManager.GetRanges())
		{
			for (int i = range.first; i <= range.last; i++)
			{
				const fs::path& path = tabs[current_tab]->entries[i].entry.path();
				ImGui::Text(path.string().c_str());
			}
		}
		ImGui::Separator();
	}
	ImGui::End();
}

// PARENT HANDLER TO CHOOSE WHICH POPUP TRIGGER
void ForestView::decide_correct_rightClick_popup()
{
	folders_right_click_popup(fs::directory_entry(tabs[current_tab]->selected_path));
	files_right_click_popup(fs::directory_entry(tabs[current_tab]->selected_path));
	selection_right_click_popup(tabs[current_tab]->selectionManager.GetRanges());
}

void ForestView::left_double_click(const fs::directory_entry& entry)
{
	if (entry.is_directory())
	{
		AccessResult checkAccess = tabs[current_tab]->fileExplorer->check_access(entry.path());
		if (checkAccess == AccessResult::ACCESS_DENIED || checkAccess == AccessResult::LOCKED_OR_IN_USE)
		{
			need_access_denied_popup = true;
			return;
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

		if (tabs[current_tab]->isSelecting)
		{
			ImGui::Text(std::format(" |  {} Selected Items ", tabs[current_tab]->selectionManager.GetCount()).c_str());
		}
		ImGui::SameLine();

		// Size of selected entry (the if condition is = we calculate the size only and only if the selected path is NOT empty and NOT a root path like (D:\, C;\))
		if (tabs[current_tab]->selected_path != tabs[current_tab]->selected_path.root_path() && !tabs[current_tab]->selected_path.empty())
		{
			ImGui::Text("|  File Size: ");
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
					try 
					{
						tabs[current_tab]->file_size = tabs[current_tab]->file_size_future.back().get();
					}
					catch (const fs::filesystem_error& e)
					{
						tabs[current_tab]->file_size.size = 0;
					}
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
				ImGui::Text(" |  Modified Since: ");
				ImGui::SameLine();
				ImGui::Text(tabs[current_tab]->last_modified.c_str());
			}
		}

		// Copying strucure msg (only if we actually coping a structure) ===========================================================================================
		if (tabs[current_tab]->is_coping_structure)
		{
			ImGui::SameLine();

			ImGui::Text(" |  Coping Structure....");
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


