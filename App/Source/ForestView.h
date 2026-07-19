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

enum viewType 
{
	LARGE_THUMBNAILS,
	ROWS,
};

class App 
{
private:

	GLFWwindow* window = nullptr;

	virtual void on_start() {};

	virtual void on_update() = 0;

	bool initialize_GLFW();

	bool create_window();

	bool initialize_GLAD();

	void initialize_ImGui();

	void render();

	void clean_up();

	void dark_mode();

public:

	App() = default;

	~App() 
	{
		clean_up();
	}

	void run_loop();

};

class ForestView : public App 
{
private:
	TexturesManager& M_texturesManager;

	std::unique_ptr<Explorer> fileExplorer;

	Favorites fav;
	Recent recents;

	float icons_width = 15.0f;
	float icons_height = 15.0f;
	float padding = 32.0f;
	
	viewType view_type = ROWS;

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
	bool favorites_need_update = false;
	bool recent_history_need_update = false;

	std::vector<fs::directory_entry> favorites;
	std::deque<fs::directory_entry> recent_history;

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


	std::future<std::string> folder_structure;

	tree t;
	tree structure_tree;
	Timer timer;

public:

	ForestView(TexturesManager& texture_manager) : M_texturesManager(texture_manager)
	{
		copied_structure.reserve(1024 * 512);
	}

	~ForestView() 
	{
		//delete fileExplorer;
	}

	void render_msg();

	void on_start() override;

	// Main update function (each frame gets called)W

	void on_update();

	void refocus_reload();

	// Search Bar
	void search_bar();

	// Tool Bar
	void tool_bar();

	// Folders Tree 
	void folders_tree(const float& remainingHeight);

	void draw_tree(Node* node_to_show);

	// Files Navigation Panel
	void files_navigation_panel(const float& remainingHeight);

	void handle_entries_mouse_click(const fs::directory_entry& entry);

	// Stats Bar
	void stats_bar();


	// Progress Bar
	void show_progress_bar(const std::string& overlay);
};