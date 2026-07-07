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

	bool need_file_size_call = false;

	

	fs::path selected_path;
	fs::path current_path;
	fs::path copied_path;

	std::string message;

	float file_size;

public:

	ForestView() 
	{
		fileExplorer = new Explorer();
	}

	void print_tree() 
	{

	}

	void render_msg()
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

	void on_update() 
	{

		ImGui::SetNextWindowSizeConstraints(ImVec2(800.0f,600.0f), ImVec2(FLT_MAX,FLT_MAX));

		if (ImGui::Begin("test")) 
		{
			if (ImGui::BeginChild("SearchBar", ImVec2(0, 40), ImGuiChildFlags_Borders))
			{
				static std::string globalKeyWord = "test";
				static std::string ThisDirectoryKeyWord;


				if (ImGui::InputTextWithHint("##Global Search Bar", fileExplorer->form_breadcrumb_navigation_string(current_path).c_str(), &globalKeyWord) || !globalKeyWord.empty())
				{
					ThisDirectoryKeyWord = "";
				}
				if (ImGui::IsItemActive() && globalKeyWord.empty())
				{
					globalKeyWord = current_path.string();
					ThisDirectoryKeyWord = "";
				}

				ImGui::SameLine();
				ImGui::SetNextItemWidth(150.0f);

				if (ImGui::InputText("Search Bar", &ThisDirectoryKeyWord) || !ThisDirectoryKeyWord.empty())
				{
					fileExplorer->search(current_path, ThisDirectoryKeyWord);
					globalKeyWord = "";
				}

			}
			ImGui::EndChild();

			if (ImGui::BeginChild("ToolBar", ImVec2(0, 40), ImGuiChildFlags_Borders))
			{
				if (ImGui::Button("<-") || ImGui::IsKeyPressed(ImGuiKey_Backspace, false) || ImGui::IsKeyPressed(ImGuiKey_MouseX1, false))
				{
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

				if (ImGui::Button("->") || ImGui::IsKeyPressed(ImGuiKey_MouseX2 , false))
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

			if (ImGui::BeginChild("FolderTree##identifer", ImVec2(200, remainingHeight), ImGuiChildFlags_Borders))
			{
				ImGui::Text("Folder Tree");
				ImGui::Separator();


			}
			ImGui::EndChild();

			ImGui::SameLine();
			
			if (ImGui::BeginChild("Files##identifier", ImVec2(0, remainingHeight), ImGuiChildFlags_Borders))
			{
				ImGui::Text("Files");
				ImGui::Separator();

				std::vector<fs::path> entries = fileExplorer->get_entries();

				for (size_t i = 0; i < entries.size(); ++i)
				{
					ImGui::PushID(i);
					static bool is_selected = false;


					auto u8_str = entries[i].u8string();
					std::string utf8_path(u8_str.begin(), u8_str.end());

					ImGui::Selectable(utf8_path.c_str(), is_selected);

					if (ImGui::IsItemHovered()) 
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) 
						{
							if (fs::is_directory(entries[i]))
							{
								fileExplorer->navigate_to(entries[i]);
								current_path = entries[i];
							}
						}
						else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
						{
							if (entries[i] != entries[i].root_path())
							{
								if (selected_path == entries[i])
								{
									show_selected_folder_options = !show_selected_folder_options;
									show_rename_file = false;
									file_size = 0.0;
								}
								else
								{
									show_selected_folder_options = true;
									show_rename_file = false;
									need_file_size_call = true;
								}
							}
							message = "";
							if (selected_path == entries[i]) selected_path.clear();
							else selected_path = entries[i];
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
								if (!fileExplorer->rename_file(selected_path, newName)) 
								{
									message = "This name already exists";
								}
								entries_need_update = true;
								newName.clear();
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
				

				ImGui::Text(std::to_string(fileExplorer->get_size()).c_str());
				ImGui::SameLine();
				ImGui::Text(" Items | ");
				
				ImGui::SameLine();

				if (selected_path != selected_path.root_path() && !selected_path.empty())
				{
					ImGui::Text("File Size: ");
					ImGui::SameLine();

					if (need_file_size_call)
					{
						file_size = fileExplorer->get_total_files_size(selected_path, [](uint64_t n) {return n / (1024 * 1024);});
						need_file_size_call = false;
					}

					ImGui::Text(std::to_string(file_size).c_str());
				}
				
			}
			ImGui::EndChild();

		}
		ImGui::End();
		
	}
};