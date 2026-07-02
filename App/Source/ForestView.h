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


#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui_stdlib.h>
#include "imgui_internal.h"

#include <filesystem>

namespace fs = std::filesystem;

#include "FileExplorer.h"

class App 
{
private:

	GLFWwindow* window = nullptr;

	virtual void on_update() = 0;

	bool initialize_GLFW();

	bool create_window();

	bool initialize_GLAD();

	void initialize_ImGui();

	void render();

	void clean_up();

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

public:

	ForestView() 
	{
		fileExplorer = new Explorer();
	}

	void on_update() 
	{
		ImGui::Begin("test");

		std::vector<fs::path> entries = fileExplorer->get_entries();

		for (size_t i = 0; i < entries.size(); ++i) 
		{
			ImGui::PushID(i);

			if (ImGui::Selectable(entries[i].string().c_str(), false))
			{
				if (fs::is_directory(entries[i])) fileExplorer->load_entries_from_specific_path(entries[i]);
			}

			ImGui::PopID();
		}

		ImGui::End();
	}
};