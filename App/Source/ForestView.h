#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <limits>
#include <cmath>
#include <chrono>
#include <iomanip> 
#include <functional>
#include <ctime>
#include <sstream>
#include <deque>
#include <format>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui_stdlib.h>
#include "imgui_internal.h"

#include <filesystem>

#include <thread>
#include <future>

#include <chrono>

#include "favorites.h"
#include "recents.h"

#include "tree.h"

namespace fs = std::filesystem;

#include "FileExplorer.h"
#include "TasksManager.h"

static int id = 1;

enum viewType
{
	LARGE_THUMBNAILS,
	ROWS,
};

enum openTapType 
{
NEW, 
COPY
};

struct State 
{
	int tabID;
	std::unique_ptr<Explorer> fileExplorer;
	float icons_width = 24.0f;
	float icons_height = 24.0f;
	float padding = 32.0f;

	viewType view_type;

	Favorites fav;
	Recent recents;


	bool show_create_folder = false;
	bool show_selected_folder_options = false;
	bool show_rename_file = false;

	bool entries_need_update = false;

	bool need_file_size_call = false;
	bool need_modified_since_call = false;

	bool show_deletion_warning = false;

	bool global_key_need_update = false;

	bool tree_need_update = false;
	bool is_calulating_size = false;
	bool is_coping_structure = false;

	bool is_searching = false;

	bool is_right_click_call = false;

	fs::path selected_path;
	fs::path current_path;
	fs::path copied_path;
	std::string copied_structure = "";

	std::string message;

	std::string globalKeyWord;
	std::string ThisDirectoryKeyWord;

	std::vector<Entry> entries;

	std::vector<std::future<fileSize>> file_size_future;

	fileSize file_size;
	std::string last_modified;

	std::string utf8_search_bar_hint = "This PC";

	std::future<std::string> folder_structure;

	tree t;
	tree structure_tree;

	std::atomic<double> copying_progress;

	State() 
	{
		tabID = id;
		id++;
		copied_structure.reserve(1024 * 512);
		view_type = ROWS;
	}
};

enum class Theme 
{
DARK_MODE,
DARK_MODE_V2,
LIGHT_MODE,
TEST,
};

class App 
{
private:

	GLFWwindow* window = nullptr;

	Theme m_activeTheme = Theme::LIGHT_MODE;

	virtual void on_start() {};

	virtual void on_update() = 0;

	virtual void on_shutdown() {};

	bool initialize_GLFW();

	bool create_window();

	bool initialize_GLAD();

	void initialize_ImGui();

	void render();

	void clean_up();

	void dark_mode();

	void dark_mode_v2();

	void light_mode();

	void spacing_and_padding_and_stuff(ImGuiStyle& style);

	void test_mode();
protected:
	ImFont* default_font = nullptr;
	ImFont* arabic_font = nullptr;

public:

	App() = default;

	~App() 
	{
		clean_up();
	}

	void change_theme(const Theme& theme);

	const Theme get_active_theme();
	

	void run_loop();

};

class ForestView : public App 
{
private:
	TexturesManager& M_texturesManager;

	float folder_icons_horizontel_indent = 20.0f;
	float folder_icons_vertical_indent = 20.0f;

	std::vector<std::unique_ptr<State>> tabs;

	int current_tab;

	openTapType m_openTabType = NEW;

	int open_new_tab_factor = -1; // to auto switch to the new opend tab

	int drop_source = -1;
	int drop_target = -1;

	fileSize size_count;

	bool show_icons_slider = false;

	std::vector<std::unique_ptr<Task>> tasks;

	std::unique_ptr<Task> m_emptyFoldersTask;
	std::vector<fs::path> m_emptyFolders;

public:

	ForestView(TexturesManager& texture_manager) : M_texturesManager(texture_manager)
	{
		tabs.push_back(std::make_unique<State>());
		current_tab = 0;
		size_count.size = 0.0;
	}

	~ForestView() 
	{
	}

	void render_msg();

	void on_start() override;

	// Main update function (each frame gets called)W

	void on_update();

	void refocus_reload();

	// shutdown 

	void on_shutdown();

	// Search Bar
	void search_bar();

	// Tool Bar
	void tool_bar();

	// Folders Tree 
	void folders_tree(const float& remainingHeight);

	void draw_tree(Node* node_to_show);

	// Files Navigation Panel
	void files_navigation_panel(const float& remainingHeight);

	void render_entries();
	
	void render_debug_panel();

	void handle_entries_mouse_click(const fs::directory_entry& entry);

	void handle_right_click_operations();

	// Stats Bar
	void stats_bar();

	void icons_size_slider();

	// Progress Bar
	void show_progress_bar(const std::string& overlay);

	void shutdown_background_tasks();

	// tabs Bar
	void tab_bar();

	std::unique_ptr<State> open_new_tab(std::unique_ptr<State>& newTab);

	std::unique_ptr<State> open_copy_tab(std::unique_ptr<State>& newTab);
};