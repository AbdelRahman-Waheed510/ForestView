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

void ForestView::render_msg()
{
	if (!message.empty())
	{
		if (message == "Task added successfuly") ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), message.c_str());
		else if (message == "Task edtied successfuly")  ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), message.c_str());
		else if (message == "Added dependencies successfuly") ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), message.c_str());
		else if (message == "Removed dependencies successfuly") ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), message.c_str());
		else ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), message.c_str());
	}
}

// Main update function (each frame gets called)
void ForestView::on_update()
{

	ImGui::SetNextWindowSizeConstraints(ImVec2(800.0f, 600.0f), ImVec2(FLT_MAX, FLT_MAX));

	if (ImGui::Begin("ForestView"))
	{
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

	}
	ImGui::End();

}

// Search Bar
void ForestView::search_bar()
{
	if (ImGui::BeginChild("SearchBar", ImVec2(0, 40), ImGuiChildFlags_Borders))
	{
		static int ids = 0;
		if (global_key_need_update)
		{
			globalKeyWord = current_path.string();
			global_key_need_update = false;
			ids++;
			ImGui::SetKeyboardFocusHere();
		}

		ImGui::SetNextItemWidth(550.0f);
		ImGui::PushID(ids);
		if (ImGui::InputTextWithHint("##Global Search Bar", fileExplorer->form_breadcrumb_navigation_string(current_path).c_str(), &globalKeyWord) || !globalKeyWord.empty())
		{
			ThisDirectoryKeyWord = "";
			fileExplorer->refresh(current_path);
			if (ImGui::IsKeyReleased(ImGuiKey_Enter))
			{
				fs::path path_(globalKeyWord);
				if (fs::is_directory(path_) && fs::exists(path_))
				{
					fileExplorer->navigate_to(path_);
					current_path = path_;
					globalKeyWord = "";

				}
				else
				{
					globalKeyWord = "";
				}
			}
		}
		ImGui::PopID();
		if (ImGui::IsItemActivated() && globalKeyWord.empty())
		{
			if (!current_path.empty())
			{
				global_key_need_update = true;
			}

		}
		if (ImGui::IsItemDeactivated() && globalKeyWord == current_path.string())
		{
			globalKeyWord = "";
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(-1.0f);

		ImGui::BeginDisabled(current_path.empty());
		if (ImGui::InputText("##Search Bar", &ThisDirectoryKeyWord) || !ThisDirectoryKeyWord.empty())
		{
			fileExplorer->search(current_path, ThisDirectoryKeyWord);
			globalKeyWord = "";
		}
		ImGui::EndDisabled();
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
			show_selected_folder_options = false;
			ThisDirectoryKeyWord = "";
			globalKeyWord = "";
			selected_path = "";
			if (ImGui::IsKeyPressed(ImGuiKey_Backspace)) {

				if (!ImGui::GetIO().WantCaptureKeyboard) {
					fileExplorer->back(current_path);
				}
			}
			else
			{
				fileExplorer->back(current_path);
			}
		}

		ImGui::SameLine();

		ImGui::BeginDisabled(!fileExplorer->can_go_forward());

		if (ImGui::Button("->") || ImGui::IsKeyPressed(ImGuiKey_MouseX2, false))
		{
			show_selected_folder_options = false;
			ThisDirectoryKeyWord = "";
			globalKeyWord = "";
			selected_path = "";
			fileExplorer->redo(current_path);
		}

		ImGui::EndDisabled();

		ImGui::SameLine();

		if (ImGui::Button("Refresh"))
		{
			selected_path = "";
			fileExplorer->refresh(current_path);
			globalKeyWord = "";
			ThisDirectoryKeyWord = "";
			tree_need_update = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("New"))
		{
			ImGui::OpenPopup("New?");
		}

		ImGui::SameLine();

		ImGui::BeginDisabled(selected_path.empty() || selected_path == selected_path.root_path());
		if (ImGui::Button("Delete"))
		{
			ImGui::OpenPopup("Delete Warning?");
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(selected_path.empty() || selected_path == selected_path.root_path());
		if (ImGui::Button("Copy"))
		{
			copied_path = selected_path;
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(copied_path.empty());
		if (ImGui::Button("Paste"))
		{
			fileExplorer->copy_paste(copied_path, current_path);
			entries_need_update = true;
			tree_need_update = true;
			copied_path.clear();
		}
		ImGui::EndDisabled();

		ImGui::SameLine();
		ImGui::BeginDisabled(current_path.empty());
		if (ImGui::Button("Sort"))
		{
			ImGui::OpenPopup("Sort Type");
		}
		ImGui::EndDisabled();


		if (ImGui::BeginPopup("New?"))
		{
			if (ImGui::Selectable("Folder"))
			{

				std::string name_of_the_folder = Utils::generate_new_folder_name(current_path, "New folder", FOLDER);
				fileExplorer->create_folder(current_path, name_of_the_folder, FOLDER);
				entries_need_update = true;
				tree_need_update = true;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Selectable("Text Document"))
			{
				std::string name_of_the_file = Utils::generate_new_folder_name(current_path, "New Text Document.txt", REG_FILE);
				fileExplorer->create_folder(current_path, name_of_the_file, REG_FILE);
				entries_need_update = true;
				tree_need_update = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("Sort Type"))
		{
			if (ImGui::Selectable("Name"))
			{
				fileExplorer->sort_entries([](const fs::directory_entry& a, const fs::directory_entry& b) {return a.path() < b.path();});
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Selectable("Last Modified time"))
			{
				fileExplorer->sort_entries([](const fs::directory_entry& a, const fs::directory_entry& b) {return a.last_write_time() > b.last_write_time();});
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
				if (fav.is_exist(selected_path)) 
				{
					fav.delete_fav(fs::directory_entry(selected_path));
					favorites_need_update = true;
				}
				fileExplorer->remove(selected_path);
				entries_need_update = true;
				tree_need_update = true;
				selected_path = "";
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("NO"))
			{
				selected_path = "";
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
		// Favorites =========================================================
		if (favorites_need_update) 
		{
			favorites = fav.get_favorites();
			favorites_need_update = false;
		}

		for (size_t i = 0; i < (int)favorites.size(); ++i) 
		{
			ImGui::PushID(i);

			auto u8_str = (favorites[i].path() == favorites[i].path().root_path()) ? favorites[i].path().u8string() : favorites[i].path().filename().u8string();
			std::string favorite_to_show(u8_str.begin(), u8_str.end());

			if (ImGui::Selectable(favorite_to_show.c_str())) 
			{
				fileExplorer->navigate_to(favorites[i].path());
				current_path = favorites[i].path();
				selected_path = "";
			}
			if (ImGui::BeginPopupContextItem("favoritesRightClick"))
			{
				if (ImGui::Button("Unpin from favorites")) 
				{
					fav.delete_fav(favorites[i]);
					favorites_need_update = true;
					ImGui::CloseCurrentPopup();
				}
				
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}

		if (!fav.is_empty()) ImGui::Separator();

		// Folders Tree =======================================================
		if (tree_need_update)
		{
			t.update_tree(fileExplorer->get_partitions());
			tree_need_update = false;
		}

		draw_tree(t.get_root());
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
			t.AddChildrenOf(node_to_show->path);
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
				fileExplorer->navigate_to(node_to_show->path);
				current_path = node_to_show->path;
				selected_path = "";
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

		std::vector<fs::directory_entry> entries = fileExplorer->get_entries();

		for (size_t i = 0; i < entries.size(); ++i)
		{
			ImGui::PushID(i);
			static bool is_selected = false;

			auto u8_str = (entries[i].path() == entries[i].path().root_path()) ? entries[i].path().u8string() : entries[i].path().filename().u8string();
			std::string utf8_path(u8_str.begin(), u8_str.end());

			ImGui::Selectable(utf8_path.c_str(), is_selected);

			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (entries[i].is_directory())
					{
						ThisDirectoryKeyWord = "";
						globalKeyWord = "";
						fileExplorer->navigate_to(entries[i]);
						current_path = entries[i];
					}
				}
				else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{

					if (entries[i].path() != entries[i].path().root_path())
					{
						if (selected_path == entries[i].path())
						{
							show_selected_folder_options = !show_selected_folder_options;
							show_rename_file = false;

							file_size.first = 0.0;
							file_size.second = "";
							last_modified = "";
						}
						else
						{
							show_selected_folder_options = true;
							show_rename_file = false;
							need_file_size_call = true;
							need_modified_since_call = true;
						}
					}
					message = "";
					if (selected_path == entries[i].path()) selected_path.clear();
					else selected_path = entries[i].path();
				}
			}

			if (ImGui::BeginPopupContextItem("entriesRightClick"))
			{
				// Add to favorites logic
				if (entries[i].is_directory()) 
				{
					if (!fav.is_exist(entries[i]))
					{
						if (ImGui::Button("Add to favorites"))
						{
							if (fav.add_fav(entries[i]))
							{
								favorites_need_update = true;
							}
							ImGui::CloseCurrentPopup();
						}
					}
					else
					{
						if (ImGui::Button("Unpin from favorites"))
						{
							fav.delete_fav(entries[i]);
							favorites_need_update = true;
							ImGui::CloseCurrentPopup();
						}
					}
				}

				ImGui::EndPopup();
			}

			if (show_selected_folder_options && selected_path == entries[i].path())
			{
				if (ImGui::Button("Rename"))
				{
					show_rename_file = !show_rename_file;
				}
				if (show_rename_file)
				{
					static std::string newName;
					ImGui::InputText("New Name", &newName);
					if (ImGui::Button("Submit"))
					{
						RenameReturn rename = fileExplorer->rename_file(selected_path, newName);
						if (rename.success) 
						{
							if (fav.is_exist(fs::directory_entry(selected_path))) 
							{
								fav.update_name(fs::directory_entry(selected_path) ,rename.finalPath);
								favorites_need_update = true;
							}
							entries_need_update = true;
							tree_need_update = true;
							newName.clear();
							selected_path = "";
						}
						else
						{
							message = "This name already exists";
						}
					}

					if (!message.empty()) ImGui::SameLine();

					render_msg();

				}

				ImGui::SameLine();

				if (ImGui::Button("Copy Path"))
				{
					show_rename_file = false;
					ImGui::SetClipboardText(selected_path.string().c_str());
				}

				if (fs::is_directory(selected_path))
				{
					ImGui::SameLine();
					if (ImGui::Button("Copy Structure"))
					{
						folder_structure = std::async(std::launch::async, [fileExplorer = this->fileExplorer, path = selected_path]() {
							// Create a local, isolated tree instance inside the thread so it doesn't touch main thread data
							tree localTree;
							localTree.BuildTree(path);
							return localTree.put_structure_into_string();
							});
						is_coping_structure = true;
					}

				}

			}

			if (folder_structure.valid() && folder_structure.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				copied_structure = folder_structure.get();
				ImGui::SetClipboardText(copied_structure.c_str());
				is_coping_structure = false;
				copied_structure = "";
			}
			ImGui::PopID();
		}


		// temporary

		auto u8 = current_path.u8string();
		std::string current_path_string_to_show(u8.begin(), u8.end());

		auto u8_selected = selected_path.u8string();
		std::string selected_path_string_to_show(u8_selected.begin(), u8_selected.end());

		ImGui::Separator();

		ImGui::Text("Current Path: "); ImGui::SameLine();
		ImGui::Text(current_path_string_to_show.c_str());

		ImGui::Text("Seleted Path: "); ImGui::SameLine();
		ImGui::Text(selected_path_string_to_show.c_str());

		ImGui::Separator();



		if (entries_need_update)
		{
			fileExplorer->refresh(current_path);
			entries_need_update = false;
		}
	}
	ImGui::EndChild();
}

// Stats Bar
void ForestView::stats_bar()
{

	if (ImGui::BeginChild("StatsBar", ImVec2(0, 40), ImGuiChildFlags_Borders))
	{


		ImGui::Text(std::to_string(fileExplorer->get_size()).c_str());
		ImGui::SameLine();
		ImGui::Text(" Items ");

		ImGui::SameLine();

		if (selected_path != selected_path.root_path() && !selected_path.empty())
		{
			ImGui::Text("| File Size: ");
			ImGui::SameLine();

			if (need_file_size_call)
			{
				// Timer timer;
				file_size_future.push_back(std::async(std::launch::async, [fileExplorer = this->fileExplorer, path = selected_path]() {return fileExplorer->get_total_files_size(path);}));
				need_file_size_call = false;
				is_calulating_size = true;
			}

			if (is_calulating_size && !file_size_future.empty() && file_size_future.back().valid())
			{
				if (file_size_future.back().wait_for(std::chrono::seconds(0)) == std::future_status::ready)
				{
					file_size = file_size_future.back().get();
					file_size_future.pop_back();
					is_calulating_size = false;
				}
			}

			std::erase_if(file_size_future, [](auto& x) {return !x.valid() || x.wait_for(std::chrono::seconds(0)) == std::future_status::ready;});

			if (!is_calulating_size) ImGui::Text((std::format("{:.2f}{}", file_size.first, file_size.second)).c_str());
			else ImGui::Text("Calculating size....");


			if (need_modified_since_call)
			{
				last_modified = fileExplorer->get_last_modified_since(selected_path);
				need_modified_since_call = false;
			}
			if (!last_modified.empty())
			{
				ImGui::SameLine();
				ImGui::Text(" | Modified Since: ");
				ImGui::SameLine();
				ImGui::Text(last_modified.c_str());
			}
		}
		if (is_coping_structure)
		{
			ImGui::SameLine();

			ImGui::Text(" | Coping Structure....");
		}


	}
	ImGui::EndChild();
}