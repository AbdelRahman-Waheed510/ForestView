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

#include "Texture.h"

namespace fs = std::filesystem;

#include "FileExplorer.h"

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
	Explorer* fileExplorer;
	Favorites fav;
	Recent recents;

	float icons_width = 15.0f;
	float icons_height = 15.0f;

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

	std::vector<std::future<std::pair<double, std::string>>> file_size_future;
	std::pair<double, std::string> file_size;
	std::string last_modified;


	std::future<std::string> folder_structure;

	tree t;
	tree structure_tree;
	Timer timer;

	Texture folder_icon;
	Texture word_icon;
	Texture power_point_icon;
	Texture exe_icon;
	Texture music_icon;
	Texture pdf_icon;
	Texture picture_icon;
	Texture zip_icon;
	Texture txt_file_icon;
	Texture default_icon;

public:

	ForestView() 
	{
		favorites = fav.get_favorites();
		copied_structure.reserve(1024*512);
		fileExplorer = new Explorer();
		t.iniThisPc("THIS PC");
		t.fillThisPcWithDrives(fileExplorer->get_partitions());
	}

	void render_msg();

	void on_start() override;

	// Main update function (each frame gets called)

	void on_update();

	// Search Bar
	void search_bar();

	// Tool Bar
	void tool_bar();

	// Folders Tree 
	void folders_tree(const float& remainingHeight);

	void draw_tree(Node* node_to_show);

	// Files Navigation Panel
	void files_navigation_panel(const float& remainingHeight);

	// Stats Bar
	void stats_bar();

	void load_images();

	void decide_icon(const fs::directory_entry& entry);
};