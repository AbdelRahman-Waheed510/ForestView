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

#include <thread>
#include <future>

#include <chrono>

#include "tree.h"

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
	bool need_modified_since_call = false;

	bool show_deletion_warning = false;

	bool global_key_need_update = false;

	bool tree_need_update = false;
	bool is_calulating_size = false;
	bool is_coping_structure = false;


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
public:

	ForestView() 
	{
		copied_structure.reserve(1024*512);
		fileExplorer = new Explorer();
		t.iniThisPc("THIS PC");
		t.fillThisPcWithDrives(fileExplorer->get_partitions());
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
			static int test_id = 0;
			test_id++;
			ImGui::Text(std::to_string(test_id).c_str());


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

				if (ImGui::Button("->") || ImGui::IsKeyPressed(ImGuiKey_MouseX2 , false))
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
					//fileExplorer->sort_entries();
				}
				ImGui::EndDisabled();



				/*if (show_create_folder)
				{
					static std::string name;
					ImGui::InputText("Name", &name);

					if (ImGui::Button("Submit"))
					{
						fileExplorer->create_folder(current_path, name);
						entries_need_update = true;
					}
				}*/

				if (ImGui::BeginPopup("New?")) 
				{
					if (ImGui::Selectable("Folder"))
					{
						fileExplorer->create_folder(current_path, "test111", FOLDER);
						entries_need_update = true;
						tree_need_update = true;
						ImGui::CloseCurrentPopup();
					}
					if (ImGui::Selectable("Txt File"))
					{
						fileExplorer->create_folder(current_path, "test111.txt", REG_FILE);
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
						fileExplorer->sort_entries([](const fs::directory_entry& a,const fs::directory_entry& b) {return a.path() < b.path();});
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
						fileExplorer->remove(selected_path);
						entries_need_update = true;
						tree_need_update = true;
						ImGui::CloseCurrentPopup();
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("NO"))
					{
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}
			ImGui::EndChild();

			
	
			float footerHeight = 40.0f + ImGui::GetStyle().FramePadding.y * 2.0f;
			float remainingHeight = ImGui::GetContentRegionAvail().y - footerHeight;
			if (remainingHeight < 0.0f) 
			{
				remainingHeight = 0.0f;
			}

			if (ImGui::BeginChild("FolderTree##identifer", ImVec2(200, remainingHeight), ImGuiChildFlags_Borders , ImGuiWindowFlags_HorizontalScrollbar))
			{
				if (tree_need_update) 
				{
					t.update_tree(fileExplorer->get_partitions());
					tree_need_update = false;
				}
				draw_tree(t.get_root());
			}
			ImGui::EndChild();

			ImGui::SameLine();
			
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
						else if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
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
								if (!fileExplorer->rename_file(selected_path, newName)) 
								{
									message = "This name already exists";
								}
								entries_need_update = true;
								tree_need_update = true;
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
					if (folder_structure.valid() &&  folder_structure.wait_for(std::chrono::seconds(0)) == std::future_status::ready) 
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
						file_size_future.push_back(std::async(std::launch::async, [fileExplorer = this->fileExplorer, path = selected_path]() {return fileExplorer->get_total_files_size(path);} ));
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

					if (!is_calulating_size) ImGui::Text((std::format("{:.2f}{}",file_size.first, file_size.second )).c_str());
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
		ImGui::End();
		
	}

	void draw_tree(Node* node_to_show)
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
};