#pragma once 

#include <filesystem>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <windows.h>
#include <format>
#include <stack>
#include <stdexcept>
#include <fstream>
#include "tree.h"
#include <functional>
#include <chrono>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Texture.h"

namespace fs = std::filesystem;

class Timer
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start_;
	float duration = 0.0;
public:

	Timer()
	{
		start_ = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		stop();
		load_to_txt_file();
	}

	void restart()
	{
		start_ = std::chrono::high_resolution_clock::now();

	}

	void stop()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> end_ = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_).time_since_epoch().count();
		duration = end - start;
	}

	float get_duration()
	{
		return duration;
	}

	void load_to_txt_file()
	{
		std::ofstream speedLog("Log.txt", std::ios::app);
		if (speedLog.is_open())
		{
			speedLog << std::to_string(duration) << " MicroSeconds\n";
			speedLog << std::to_string(duration * 0.001) << " MillieSeconds\n";
			speedLog << std::to_string(duration * 0.000001) << " Seconds\n";
			speedLog << "=============================================================\n";
		}
		speedLog.close();
	}
};


struct Entry 
{
	fs::directory_entry entry;
	GLuint textureId;
	std::string name;
};

struct fileSize
{
	double size;
	std::string label;
};

enum NewType
{
	FOLDER,
	REG_FILE
};

namespace Utils 
{
	std::string generate_new_folder_name(const fs::path& path, const std::string& name, const NewType& type);
};

struct RenameReturn 
{
	fs::path finalPath;
	bool success;
};

struct EntryDataReturn 
{
	double directorySize = 0.0;
	int filesCount = 0;
};
class Explorer 
{
private:
	TexturesManager& M_texturesManager;	
	tree* t;
	std::vector<Entry> entries;
	std::stack<fs::path> redo_stack;
	std::stack<fs::path> undo_stack;
	

public:
	
	Explorer(TexturesManager& textures_manager) : M_texturesManager(textures_manager)
	{
		load_partitions();
	}
	
	~Explorer() {};

	const std::stack<fs::path>& get_redo_stack() 
	{
		return redo_stack;
	}

	void set_redo_stack(const std::stack<fs::path>& redo_stack_to_set) 
	{
		redo_stack = redo_stack_to_set;
	}

	const std::stack<fs::path>& get_undo_stack()
	{
		return undo_stack;
	}

	void set_undo_stack(const std::stack<fs::path>& undo_stack_to_set)
	{
		undo_stack = undo_stack_to_set;
	}

	void load_partitions() 
	{
		if (!entries.empty()) entries.clear();
		wchar_t buffer[256];
		DWORD buffer_len = GetLogicalDriveStringsW(ARRAYSIZE(buffer), buffer);
		if (buffer_len > 0 && buffer_len < ARRAYSIZE(buffer)) {
			wchar_t* drive = buffer;
			while (*drive) {
				std::error_code ec;
				fs::directory_entry ent(fs::path(drive), ec);	
				if (ec)
				{
					std::cerr << "Error while loading partitions" << std::endl;
					drive += wcslen(drive) + 1;
					continue;
				}

				Entry e;
				e.entry = ent;
				e.textureId = M_texturesManager.decide_icon(ent);
				auto u8_str = (ent.path() == ent.path().root_path()) ? ent.path().u8string() : ent.path().filename().u8string();
				std::string utf8_path(u8_str.begin(), u8_str.end());
				e.name = utf8_path;
				entries.emplace_back(std::move(e));
				drive += wcslen(drive) + 1;
			}
		}
		return;
	}

	std::vector<fs::path> get_partitions() 
	{
		std::vector<fs::path> drives;
		wchar_t buffer[256];
		DWORD buffer_len = GetLogicalDriveStringsW(ARRAYSIZE(buffer), buffer);
		if (buffer_len > 0 && buffer_len < ARRAYSIZE(buffer)) {
			wchar_t* drive = buffer;
			while (*drive) {
				drives.push_back(fs::path(drive));
				drive += wcslen(drive) + 1;
			}
		}
		return drives;
	}

	int get_size() 
	{
		return entries.size();
	}

	const fileSize decide_file_size_format(const double& count_in_bytes)
	{
		if (count_in_bytes > (1024.0 * 1024.0 * 1024.0))
			return { count_in_bytes / (1024 * 1024 * 1024), "GB" };
		else if (count_in_bytes > (1024.0 * 1024.0))
			return { count_in_bytes / (1024.0 * 1024.0), "MB" };
		else if (count_in_bytes > 1024.0)
			return { count_in_bytes / (1024.0), "KB" };
		return { count_in_bytes, "Bytes" };
	}

	EntryDataReturn get_total_files_size(const fs::path& path)
	{
		EntryDataReturn data;

		std::error_code ec;
		if (fs::is_directory(path, ec))
		{
			for (const auto& entry : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied, ec))
			{
				if (fs::is_regular_file(entry.status()))
				{
					data.directorySize += entry.file_size();
					data.filesCount++;
				}
			}
			if (ec) 
			{
				std::cerr << "Error while calulating the size" << std::endl;
				data.directorySize = 0.0;
				data.filesCount = 0;
				return data;
			}
		}
		else if (fs::is_regular_file(path, ec))	
		{
			data.directorySize += fs::file_size(path);
			data.filesCount++;
			if (ec)
			{
				std::cerr << "Error while calulating the size" << std::endl;
				return data;
			}
		}

		return data;
	}

	const std::string get_last_modified_since(const fs::path& path)
	{
		if (!fs::exists(path)) return "";

		fs::file_time_type time = fs::last_write_time(path);

		auto sys_time = std::chrono::clock_cast<std::chrono::system_clock>(time);

		std::string res = std::format("{:%Y-%m-%d %H:%M}", sys_time);

		return res;

	}

	void load_entries_from_specific_path(fs::path path) 
	{
		if (!entries.empty())
		{
			entries.clear();
		}
		if (path.empty()) return;

		std::error_code ec;
		if (fs::is_regular_file(path, ec)) return;
		if (!fs::exists(path, ec)) return;

		for (const auto& entry : fs::directory_iterator(path, ec))
		{
			DWORD attr = GetFileAttributesW(entry.path().c_str());

			if (attr == INVALID_FILE_ATTRIBUTES)
				continue; // couldn't stat it, skip

			if (attr & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM))
				continue; // skip hidden/system

			Entry e;
			e.entry = entry;
			e.textureId = M_texturesManager.decide_icon(entry);
			auto u8_str = (entry.path() == entry.path().root_path()) ? entry.path().u8string() : entry.path().filename().u8string();
			std::string utf8_path(u8_str.begin(), u8_str.end());
			e.name = utf8_path;

			entries.emplace_back(std::move(e));

		}
		return;
	}

	void refresh(const fs::path& current_path)
	{
		if (current_path.empty())
			load_partitions();
		else 
		{
			load_entries_from_specific_path(current_path);
		} 
	}

	void navigate_to(const fs::path& path, const fs::path& current_path) 
	{
		if (path.empty()) load_partitions();
		else if (path == "THIS PC") load_partitions();
		else load_entries_from_specific_path(path);
		
		undo_stack.push(current_path);

		if (!redo_stack.empty()) 
		{	
			while (!redo_stack.empty()) 
			{
				redo_stack.pop();
			}
		}
		return;
	}

	void to_lower(std::string& word)
	{
		for (auto& c : word) 
		{
			c = std::tolower(c);
		}
		return;
	}

	std::vector<Entry>& search(const fs::path& path, std::string keyword)
	{
		if (path.empty())
		{
			return entries;
		}
		if (!entries.empty())
		{
			entries.clear();
		}
		for (const auto& entry : fs::directory_iterator(path))
		{
			DWORD attr = GetFileAttributesW(entry.path().c_str());

			if (attr == INVALID_FILE_ATTRIBUTES)
				continue; // couldn't stat it, skip

			if (attr & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM))
				continue; // skip hidden/system

			std::string entrypath = entry.path().filename().string();

			to_lower(entrypath);
			to_lower(keyword);

			if (entrypath.find(keyword) != std::string::npos)
			{
				Entry e;
				e.entry = entry;
				e.textureId = M_texturesManager.decide_icon(entry);
				auto u8_str = (entry.path() == entry.path().root_path()) ? entry.path().u8string() : entry.path().filename().u8string();
				std::string utf8_path(u8_str.begin(), u8_str.end());
				e.name = utf8_path;
				entries.emplace_back(std::move(e));	
			}
		}
		return entries;
	}

	const std::vector<Entry>& get_entries()
	{
		return entries;
	}

	void create_folder(fs::path& to_create, const std::string& name, const NewType& type) 
	{
		if (type == FOLDER) 
		{
			std::error_code ec;
			fs::path newFolder = to_create / name;
			if (fs::create_directory(newFolder, ec)) return;
			if (ec) 
			{
				std::cerr << "Error when creating " << name << std::endl;
			}
		}
		else if (type == REG_FILE) 
		{
			fs::path newFilePath = to_create / name;
			std::ofstream newfile(newFilePath);
		}

		return;
	}

	RenameReturn rename_file(fs::path& to_rename, fs::path new_name)
	{
		if (new_name.empty()) return { RenameReturn("",false) };

		if (fs::is_regular_file(to_rename)) 
		{
			if (new_name.has_extension())
			{
				if (new_name.extension() != to_rename.extension())
				{
					return { RenameReturn("",false) };
				}
			}
			else 
			{
				new_name.replace_extension(to_rename.extension());
			}
		}
		fs::path delted_path = to_rename;
		delted_path.remove_filename();
		
		if (fs::is_directory(delted_path / new_name))
		{
			new_name = Utils::generate_new_folder_name(to_rename.parent_path(), new_name.filename().string(), FOLDER);
		}
		else if (fs::is_regular_file(delted_path / new_name))
		{
			new_name = Utils::generate_new_folder_name(to_rename.parent_path(), new_name.filename().string(), REG_FILE);
		}

		fs::path newPath = delted_path / new_name;

		std::error_code ec;

		fs::rename(to_rename, newPath, ec);

		if (ec) 
		{
			std::cerr << "Error when renaming the file " << to_rename.filename().string() << std::endl;
			return { RenameReturn("",false) };
		}
		return { RenameReturn(newPath,true) };
	}

	void back(fs::path& current_path) 
	{
		if (undo_stack.empty()) return;
		fs::path path_to_undo = undo_stack.top(); undo_stack.pop();

		redo_stack.push(current_path);  

		if (path_to_undo.empty()) load_partitions();
		else load_entries_from_specific_path(path_to_undo);

		current_path = path_to_undo;   
	}

	bool can_go_forward() 
	{
		if (redo_stack.empty()) return false;
		return true;
	}

	void redo(fs::path& current_path)
	{
		if (redo_stack.empty()) return;
		fs::path path = redo_stack.top(); redo_stack.pop();

		undo_stack.push(current_path);

		load_entries_from_specific_path(path);
		current_path = path;
	}

	fs::path get_forward_path() 
	{
		if (!redo_stack.empty()) return redo_stack.top();
		else 
		{
			throw std::runtime_error("void redo(): there is no path to redo");
		}
	}

	void copy_paste(const fs::path& copied_path, const fs::path& current_path, std::atomic<double>& Sizeprogress, std::atomic<int>& filesCountProgress, std::atomic<bool>& finished) 
	{
		if (!fs::exists(copied_path))
		{
			std::cerr << "Error: source path doesnot exist: " << copied_path << std::endl;
			return;
		}
		std::error_code ec;

		fs::path dest = current_path / copied_path.filename();;

		// If the copied directory is a folder 
		if (fs::is_directory(copied_path))
		{
			std::string name_of_the_folder = Utils::generate_new_folder_name(current_path, copied_path.filename().string(), FOLDER);
			dest = current_path / name_of_the_folder;

			fs::create_directories(dest);

			for (auto entry = fs::recursive_directory_iterator(copied_path, fs::directory_options::skip_permission_denied, ec); entry != fs::recursive_directory_iterator(); entry.increment(ec))
			{
				if (finished) return;
				fs::path relativePath = fs::relative(entry->path(), copied_path);
				fs::path targetPath = dest / relativePath;

				if (entry->is_directory())
				{
					fs::create_directories(targetPath);
				}
				else if (entry->is_regular_file())
				{
					fs::create_directories(targetPath.parent_path());

					std::error_code ec2;

					fs::copy_file(entry->path(), targetPath, fs::copy_options::overwrite_existing, ec2);

					if (ec2) continue;

					Sizeprogress += entry->file_size();
					filesCountProgress++;
				}

				// Temporary for debugging, TODO: remove (note: this will slow the coping function and it will get removed in the future)
				///*fileSize file_size = decide_file_size_format(progress);
				//std::cout << "Coping Directory (" << targetPath << "), Directory Size Count = " << file_size.size << " " << file_size.label << std::endl*/;
				// ========================================

				if (ec)
				{
					continue;
				}
			}
		}

		// If the copied directory is a regular file so it does not need the access to the for loop
		else if (fs::is_regular_file(copied_path))
		{
			std::string name_of_the_folder = Utils::generate_new_folder_name(current_path, copied_path.filename().string(), REG_FILE);
			dest = current_path / name_of_the_folder;

			std::error_code ec1;
			fs::copy_file(copied_path, dest, fs::copy_options::overwrite_existing, ec1);
			if (ec1) 
			{
				std::cerr << "Error while coping " << dest << "...!" << std::endl;
			}
			Sizeprogress += fs::directory_entry(copied_path).file_size();
			filesCountProgress++;
		}

		if (ec) 
		{
			std::cerr << "Copy failed: " << ec.message() << '\n';
		}
		
		return;
	}

	void remove(fs::path& path_to_delete) 
	{
		if (!fs::exists(path_to_delete)) 
		{
			std::cerr << "Error: source path doesnot exist.....! " << std::endl;
			return;
		}
		std::error_code ec;

		int deleted_files = fs::remove_all(path_to_delete, ec);
		if (ec) 
		{
			std::cerr << "Failed to wipe directory: " << ec.message() << '\n';
		}
	}

	std::string form_breadcrumb_navigation_string(const fs::path& path_to_format) 
	{
		std::string result = "";
	
		for (const auto& part : path_to_format) 
		{
			if (result.empty()) 
			{
				result += part.string();
				continue;
			}
			result += part.string();
			result += " > ";
		}
		if ((int)result.size() > 50) 
		{
			int max = (int)result.size() - 50;
			int index_pos = 0;
			for (size_t i = max - 1; i < (int)result.size(); ++i) 
			{
				if (result[i] == '>')
				{
					index_pos = i + 2;
					break;
				} 
			}
			result = "... > " + result.substr(index_pos, (int)result.size());
		}
		return result;
	}

	void sort_entries(const std::function<bool(Entry&, Entry&)>& comp)
	{
		std::sort(entries.begin(), entries.end(), comp);
		return;
	}

	void print_entries() 
	{
		for (const Entry& entry : entries) 
		{
			std::cout << entry.entry.path() << std::endl;
		}
	}

};