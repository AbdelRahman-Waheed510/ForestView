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

	dark_mode();
	
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

void App::dark_mode()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.4f;
	style.WindowPadding = ImVec2(12.0f, 12.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(20.0f, 20.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(20.0f, 3.4f);
	style.FrameRounding = 5.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(4.3f, 5.5f);
	style.ItemInnerSpacing = ImVec2(7.1f, 1.8f);
	style.CellPadding = ImVec2(12.1f, 9.2f);
	style.IndentSpacing = 10.0f;
	style.ColumnsMinSpacing = 4.9f;
	style.ScrollbarSize = 11.6f;
	style.ScrollbarRounding = 5.0f;
	style.GrabMinSize = 3.7f;
	style.GrabRounding = 5.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.27450982f, 0.31764707f, 0.4509804f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09411765f, 0.101960786f, 0.11764706f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.11372549f, 0.1254902f, 0.15294118f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803922f, 0.105882354f, 0.12156863f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.972549f, 1.0f, 0.49803922f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.972549f, 1.0f, 0.49803922f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.79607844f, 0.49803922f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.18039216f, 0.1882353f, 0.19607843f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15294118f, 0.15294118f, 0.15294118f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.14117648f, 0.16470589f, 0.20784314f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.105882354f, 0.105882354f, 0.105882354f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.12941177f, 0.14901961f, 0.19215687f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.15686275f, 0.18431373f, 0.2509804f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.15686275f, 0.18431373f, 0.2509804f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.14509805f, 0.14509805f, 0.14509805f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.972549f, 1.0f, 0.49803922f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1254902f, 0.27450982f, 0.57254905f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.52156866f, 0.6f, 0.7019608f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.039215688f, 0.98039216f, 0.98039216f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.88235295f, 0.79607844f, 0.56078434f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.95686275f, 0.95686275f, 0.95686275f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803922f, 0.105882354f, 0.12156863f, 1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9372549f, 0.9372549f, 0.9372549f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.49803922f, 0.5137255f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26666668f, 0.2901961f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.49803922f, 0.5137255f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.19607843f, 0.1764706f, 0.54509807f, 0.5019608f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.19607843f, 0.1764706f, 0.54509807f, 0.5019608f);

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

	rows_button_texture_id = M_texturesManager.get_Rows_button_icon_texture_id();
	tiles_button_texture_id = M_texturesManager.get_LargeThumbnail_button_icon_texture_id();

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
		
		if (tabs[current_tab]->copying_progress != 0.0) 
		{
			if (ImGui::Begin("p"))
			{
				ImGui::ProgressBar(tabs[current_tab]->copying_progress / 7932915, ImVec2(500.00f, 50.0f));
			}
			ImGui::End();
		}

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
					tabs[current_tab]->fileExplorer->navigate_to(path_, tabs[current_tab]->current_path);
					tabs[current_tab]->entries_need_update = true;
					tabs[current_tab]->current_path = path_;
					tabs[current_tab]->globalKeyWord = "";

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
	if (ImGui::BeginChild("ToolBar", ImVec2(0, 40), ImGuiChildFlags_Borders))
	{
		if (ImGui::Button("<-") || ImGui::IsKeyPressed(ImGuiKey_Backspace, false) || ImGui::IsKeyPressed(ImGuiKey_MouseX1, false))
		{
			tabs[current_tab]->show_selected_folder_options = false;
			tabs[current_tab]->ThisDirectoryKeyWord = "";
			tabs[current_tab]->globalKeyWord = "";
			tabs[current_tab]->selected_path = "";
			if (ImGui::IsKeyPressed(ImGuiKey_Backspace)) {

				if (!ImGui::GetIO().WantCaptureKeyboard) {
					tabs[current_tab]->fileExplorer->back(tabs[current_tab]->current_path);
					tabs[current_tab]->recents.visit(fs::directory_entry(tabs[current_tab]->current_path));
					tabs[current_tab]->entries_need_update = true;
				}
			}
			else
			{
				tabs[current_tab]->fileExplorer->back(tabs[current_tab]->current_path);
				tabs[current_tab]->recents.visit(fs::directory_entry(tabs[current_tab]->current_path));
				tabs[current_tab]->entries_need_update = true;
			}
		}

		ImGui::SameLine();

		ImGui::BeginDisabled(!tabs[current_tab]->fileExplorer->can_go_forward());

		if (ImGui::Button("->") || ImGui::IsKeyPressed(ImGuiKey_MouseX2, false))
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

		if (ImGui::Button("New"))
		{
			ImGui::OpenPopup("New?");
		}

		ImGui::SameLine();

		ImGui::BeginDisabled(tabs[current_tab]->selected_path.empty() || tabs[current_tab]->selected_path == tabs[current_tab]->selected_path.root_path());
		if (ImGui::Button("Delete"))
		{
			ImGui::OpenPopup("Delete Warning?");
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
			tabs[current_tab]->fileExplorer->copy_paste(tabs[current_tab]->copied_path, tabs[current_tab]->current_path);
			tabs[current_tab]->entries_need_update = true;
			tabs[current_tab]->tree_need_update = true;
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
		if (ImGui::Button("View")) 
		{
		
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			ImGui::OpenPopup("ViewSettings");
		}
		if (ImGui::BeginPopup("ViewSettings")) 
		{
			if (!show_icons_slider) 
			{
				if (ImGui::Button("Open Icons Size Slider"))
				{
					show_icons_slider = true;
					ImGui::CloseCurrentPopup();
				}
			}
			else 
			{
				if (ImGui::Button("close Icons Size Slider"))
				{
					show_icons_slider = false;
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

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

		if (ImGui::BeginPopup("Sort Type"))
		{
			if (ImGui::Selectable("Name"))
			{
				tabs[current_tab]->fileExplorer->sort_entries([](const Entry& a, const Entry& b) {return a.entry.path() < b.entry.path();});
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

		if (ImGui::BeginPopupModal("Delete Warning?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure you want to delete this folder?\nThe deleted file will be deleted permanently and will not go to the recycle bin \nThis action cannot be undone.");
			ImGui::Separator();

			if (ImGui::Button("YES"))
			{
				if (tabs[current_tab]->fav.is_exist(tabs[current_tab]->selected_path))
				{
					tabs[current_tab]->fav.delete_fav(fs::directory_entry(tabs[current_tab]->selected_path));
				}
				tabs[current_tab]->recents.remove(fs::directory_entry(tabs[current_tab]->selected_path));
				tabs[current_tab]->fileExplorer->remove(tabs[current_tab]->selected_path);
				tabs[current_tab]->entries_need_update = true;
				tabs[current_tab]->tree_need_update = true;
				tabs[current_tab]->selected_path = "";
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("NO"))
			{
				tabs[current_tab]->selected_path = "";
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	ImGui::EndChild();

}

// Folders Tree 
void ForestView::folders_tree(const float& remainingHeight)
{
	if (ImGui::BeginChild("FolderTree##identifer", ImVec2(200, remainingHeight), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar))
	{

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

		// the right click logic ==================================================================
		handle_right_click_operations();
		
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
		for (size_t i = 0; i < tabs[current_tab]->entries.size(); ++i)
		{
			ImGui::PushID(i);

			bool is_selected = (tabs[current_tab]->entries[i].entry.path() == tabs[current_tab]->selected_path) ? true : false;

			// ===========================================================================================
			ImGui::Image(tabs[current_tab]->entries[i].textureId, ImVec2(tabs[current_tab]->icons_width, tabs[current_tab]->icons_height));

			ImGui::SameLine();

			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.005f, 0.5f));
			
			ImGui::Selectable(tabs[current_tab]->entries[i].name.c_str(), is_selected, 0, ImVec2(0, tabs[current_tab]->icons_height));

			ImGui::PopStyleVar(1);
			// ===========================================================================================

			handle_entries_mouse_click(tabs[current_tab]->entries[i].entry);

			ImGui::PopID();
		}
	}

}

void ForestView::render_debug_panel()
{
	auto u8 = tabs[current_tab]->current_path.u8string();
	std::string current_path_string_to_show(u8.begin(), u8.end());

	auto u8_selected = tabs[current_tab]->selected_path.u8string();
	std::string selected_path_string_to_show(u8_selected.begin(), u8_selected.end());

	ImGui::Separator();

	ImGui::Text("Current Path: "); ImGui::SameLine();
	ImGui::Text(current_path_string_to_show.c_str());

	ImGui::Text("Seleted Path: "); ImGui::SameLine();
	ImGui::Text(selected_path_string_to_show.c_str());

	ImGui::Separator();
}

void ForestView::handle_entries_mouse_click(const fs::directory_entry& entry)
{
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (entry.is_directory())
			{
				tabs[current_tab]->ThisDirectoryKeyWord = "";
				tabs[current_tab]->globalKeyWord = "";
				tabs[current_tab]->recents.visit(entry);
				tabs[current_tab]->fileExplorer->navigate_to(entry.path(), tabs[current_tab]->current_path);
				tabs[current_tab]->entries_need_update = true;

				tabs[current_tab]->current_path = entry.path();
			}
		}
		else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))	
		{
			tabs[current_tab]->selected_path = entry.path();
			tabs[current_tab]->is_right_click_call = true;
		}
		else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
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
	}
}

void ForestView::handle_right_click_operations()
{
	if (tabs[current_tab]->is_right_click_call)
	{
		ImGui::OpenPopup("entriesRightClick");
		tabs[current_tab]->is_right_click_call = false;
	}
	std::error_code ec;
	fs::directory_entry entry(tabs[current_tab]->selected_path, ec);

	static bool was_open = false;
	bool is_open = ImGui::BeginPopup("entriesRightClick");
	if (is_open)
	{
		if (entry.path() != tabs[current_tab]->selected_path) tabs[current_tab]->selected_path = entry.path();
		// Rename 
		if (tabs[current_tab]->selected_path != tabs[current_tab]->selected_path.root_path())
		{
			ImGui::Separator();
			static std::string newName;
			ImGui::InputText("New Name", &newName);
			if (ImGui::Button("Rename"))
			{
				RenameReturn rename = tabs[current_tab]->fileExplorer->rename_file(tabs[current_tab]->selected_path, newName);
				if (rename.success)
				{
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

		// Copy path
		if (ImGui::Button("Copy Path"))
		{
			tabs[current_tab]->show_rename_file = false;
			ImGui::SetClipboardText(tabs[current_tab]->selected_path.string().c_str());


			tabs[current_tab]->selected_path = "";
			ImGui::CloseCurrentPopup();
		}

		//copy structrure
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
		if (entry.is_directory())
		{
			if (!tabs[current_tab]->fav.is_exist(entry))
			{
				if (ImGui::Button("Add to favorites"))
				{
					tabs[current_tab]->fav.add_fav(entry);

					tabs[current_tab]->selected_path = "";
					ImGui::CloseCurrentPopup();
				}
			}
			else
			{
				if (ImGui::Button("Unpin from favorites"))
				{
					tabs[current_tab]->fav.delete_fav(entry);

					tabs[current_tab]->selected_path = "";
					ImGui::CloseCurrentPopup();
				}
			}
		}

		ImGui::EndPopup();
	}
	if (was_open && !is_open)
	{
		tabs[current_tab]->selected_path = "";
	}
	was_open = is_open;
}

// Stats Bar
void ForestView::stats_bar()
{
	if (ImGui::BeginChild("StatsBar", ImVec2(0, 40), ImGuiChildFlags_Borders))
	{
		ImGui::Text(std::to_string(tabs[current_tab]->fileExplorer->get_size()).c_str());
		ImGui::SameLine();
		ImGui::Text(" Items ");

		ImGui::SameLine();

		if (tabs[current_tab]->selected_path != tabs[current_tab]->selected_path.root_path() && !tabs[current_tab]->selected_path.empty())
		{
			ImGui::Text("| File Size: ");
			ImGui::SameLine();

			if (tabs[current_tab]->need_file_size_call)
			{
				tabs[current_tab]->file_size_future.push_back(std::async(std::launch::async, [&fileExplorer = this->tabs[current_tab]->fileExplorer, path = tabs[current_tab]->selected_path]() {return fileExplorer->get_total_files_size(path);}));
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
		if (tabs[current_tab]->is_coping_structure)
		{
			ImGui::SameLine();

			ImGui::Text(" | Coping Structure....");
		}

		ImGui::SameLine();
		float spacing = ImGui::GetStyle().ItemSpacing.x;
		float padding = ImGui::GetStyle().FramePadding.x;
		float target_pos = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (15.0f * 2 + spacing + (padding * 4));

		ImGui::SetCursorPosX(target_pos);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		if (ImGui::ImageButton("Rows", rows_button_texture_id, ImVec2(12.0f, 12.0f)))
		{
			tabs[current_tab]->view_type = ROWS;
			tabs[current_tab]->icons_height = 15.0f;
			tabs[current_tab]->icons_width = 15.0f;
		}
		ImGui::SameLine();
		if (ImGui::ImageButton("Large thumbnails", tiles_button_texture_id, ImVec2(12.0f, 12.0f)))
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
	tabs[current_tab]->file_size_future.clear();
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

