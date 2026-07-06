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
	bool show_create_folder = false;
	bool show_selected_folder_options = false;
	bool show_rename_file = false;

	bool entries_need_update = false;

	fs::path selected_path;
	fs::path current_path;
	fs::path copied_path;


public:

	ForestView() 
	{
		fileExplorer = new Explorer();
	}

	void print_tree() 
	{

	}

	void on_update() 
	{

		ImGui::SetNextWindowSizeConstraints(ImVec2(800.0f,600.0f), ImVec2(FLT_MAX,FLT_MAX));

		if (ImGui::Begin("test")) 
		{

			if (ImGui::BeginChild("SearchBar", ImVec2(0, 40), ImGuiChildFlags_Borders))
			{
			

			}
			ImGui::EndChild();

			if (ImGui::BeginChild("ToolBar", ImVec2(0, 40), ImGuiChildFlags_Borders))
			{
				if (ImGui::Button("<-") || ImGui::IsKeyPressed(ImGuiKey_Backspace))
				{
					fileExplorer->back(current_path);
				}

				ImGui::SameLine();

				ImGui::BeginDisabled(!fileExplorer->can_go_forward());

				if (ImGui::Button("->") || ImGui::IsKeyPressed(ImGuiKey_Backspace))
				{
					fileExplorer->redo(current_path);
				}

				ImGui::EndDisabled();


				ImGui::SameLine();

				if (ImGui::Button("New"))
				{
					// show_create_folder = true;
					ImGui::OpenPopup("Newfilepopup");
				}

				

				ImGui::SameLine();

				ImGui::BeginDisabled(selected_path.empty() || selected_path == selected_path.root_path());
				if (ImGui::Button("Delete"))
				{
					fileExplorer->remove(selected_path);
					entries_need_update = true;
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
					copied_path.clear();
				}
				ImGui::EndDisabled();



				if (show_create_folder)
				{
					static std::string name;
					ImGui::InputText("Name", &name);

					if (ImGui::Button("Submit"))
					{
						fileExplorer->create_folder(current_path, name);
						entries_need_update = true;
					}
				}

			}
			ImGui::EndChild();

			float footerHeight = 40.0f + ImGui::GetStyle().FramePadding.y * 2.0f;
			float remainingHeight = ImGui::GetContentRegionAvail().y - footerHeight;
			if (remainingHeight < 0.0f) 
			{
				remainingHeight = 0.0f;
			}

			if (ImGui::BeginChild("FolderTree", ImVec2(200, remainingHeight), ImGuiChildFlags_Borders))
			{
			

			}
			ImGui::EndChild();

			ImGui::SameLine();
			
			if (ImGui::BeginChild("Files", ImVec2(0, remainingHeight), ImGuiChildFlags_Borders))
			{

				std::vector<fs::path> entries = fileExplorer->get_entries();

				for (size_t i = 0; i < entries.size(); ++i)
				{
					ImGui::PushID(i);
					static bool is_selected = false;


					auto u8_str = entries[i].u8string();
					std::string utf8_path(u8_str.begin(), u8_str.end());

					if (ImGui::Selectable(utf8_path.c_str(), is_selected))
					{
						if (selected_path == entries[i]) selected_path.clear();
						else selected_path = entries[i];
						show_selected_folder_options = !show_selected_folder_options;
					}

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						if (fs::is_directory(entries[i]))
						{
							fileExplorer->navigate_to(entries[i]);
							current_path = entries[i];
						}
					}

					if (show_selected_folder_options && selected_path == entries[i])
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
								fileExplorer->rename_file(selected_path, newName);
								entries_need_update = true;
							}
						}
					}

					ImGui::PopID();
				}


				// temporary

				ImGui::Separator();

				ImGui::Text("Current Path: "); ImGui::SameLine();
				ImGui::Text(current_path.string().c_str());

				ImGui::Text("Seleted Path: "); ImGui::SameLine();
				ImGui::Text(selected_path.string().c_str());

				ImGui::Separator();



				if (entries_need_update)
				{
					fileExplorer->load_entries_from_specific_path(current_path);
				}


			}
			ImGui::EndChild();

			if (ImGui::BeginChild("StatsBar", ImVec2(0, 40), ImGuiChildFlags_Borders))
			{
				if (fs::is_regular_file(selected_path)) 
				{
					fs::directory_entry entry(selected_path);
					ImGui::Text("File size: "); ImGui::SameLine();
					ImGui::Text(std::to_string((float)entry.file_size() / (1024*1024)).c_str());
				}

			}
			ImGui::EndChild();

		}
		ImGui::End();
		
	}
};