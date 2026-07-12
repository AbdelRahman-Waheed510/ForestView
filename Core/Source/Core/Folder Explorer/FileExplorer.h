#pragma once 

#include <filesystem>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <windows.h>
#include <format>
#include <stack>
#include <stdexcept>
#include "tree.h"
#include <functional>
#include <chrono>

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

class Explorer 
{
private:
	tree* t;
	std::vector<fs::directory_entry> entries;
	std::stack<fs::path> paths_undo;

public:
	
	Explorer() 
	{
		load_partitions();
	}
	
	~Explorer() {};

	void load_partitions() 
	{
		if (!entries.empty()) entries.clear();
		wchar_t buffer[256];
		DWORD buffer_len = GetLogicalDriveStringsW(ARRAYSIZE(buffer), buffer);
		if (buffer_len > 0 && buffer_len < ARRAYSIZE(buffer)) {
			wchar_t* drive = buffer;
			while (*drive) {
				std::error_code ec;
				entries.emplace_back(fs::path(drive), ec);
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

	std::pair<double, std::string> get_total_files_size(const fs::path& path)
	{
		double count = 0.0;
		std::error_code ec;
		if (fs::is_directory(path, ec))
		{
			for (const auto& entry : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied, ec))
			{
				if (fs::is_regular_file(entry.status()))
				{
					count += entry.file_size();
				}
			}
			if (ec) 
			{
				std::cerr << "Error while calulating the size" << std::endl;
				return {0.0,""};
			}
		}
		else if (fs::is_regular_file(path, ec))	
		{
			count += fs::file_size(path);
			if (ec)
			{
				std::cerr << "Error while calulating the size" << std::endl;
				return { 0.0,"" };
			}
		}
		if (count > (1024.0 * 1024.0 * 1024.0))
			return { count / (1024.0 * 1024.0 * 1024.0), " GB" };
		else if (count > (1024.0 * 1024.0))
			return { count / (1024.0 * 1024.0), " MB" };
		else if (count > 1024.0)
			return { count / (1024.0), " KB" };

		
		return {(double)count, " Bytes"};
	}

	const std::string get_last_modified_since(const fs::path& path)
	{
		if (!fs::exists(path)) return "";

		fs::file_time_type time = fs::last_write_time(path);

		auto sys_time = std::chrono::clock_cast<std::chrono::system_clock>(time);

		std::string res = std::format("{:%Y-%m-%d %H:%M}", sys_time);

		return res;

	}
	void load_entries_from_specific_path(const fs::path& path) 
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

			entries.emplace_back(entry);
		}
		return;
	}

	void refresh(const fs::path& current_path)
	{
		if (current_path.empty())
			load_partitions();
		else navigate_to(current_path);
	}

	void navigate_to(const fs::path& path) 
	{
		load_entries_from_specific_path(path);

		if (!paths_undo.empty()) 
		{
		
			while (!paths_undo.empty()) 
			{
				paths_undo.pop();
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

	void search(const fs::path& path, std::string& keyword) 
	{
		if (path.empty())
		{
			return;
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

			if (entrypath.find(keyword) != std::string::npos) entries.emplace_back(entry);
		}
		return;
	}

	std::vector<fs::directory_entry> get_entries()
	{
		return entries;
	}

	void create_folder(fs::path& to_create, const fs::path& name) 
	{
		
		fs::path newFolder = to_create / name;
		if (fs::exists(newFolder)) return;
		fs::create_directory(newFolder);
		return;
	}

	bool rename_file(fs::path& to_rename, fs::path new_name)	 
	{
		if (new_name.empty()) return true;

		if (fs::is_regular_file(to_rename)) 
		{
			if (new_name.has_extension())
			{
				if (new_name.extension() != to_rename.extension())
				{
					return false;
				}
			}
			else 
			{
				new_name.replace_extension(to_rename.extension());
			}
		}
		fs::path delted_path = to_rename;
		delted_path.remove_filename();
		
		for (const auto& entry : fs::directory_iterator(delted_path)) 
		{
			if (entry.path().filename() == new_name) return false;;
		}

		fs::path newPath = delted_path / new_name;

		fs::rename(to_rename, newPath);
		return true;
	}

	void back(fs::path& current_path) 
	{
		if (current_path.empty()) return;
		paths_undo.push(current_path);

		if (current_path == current_path.root_path())
		{
			load_partitions();
			
			current_path.clear();
		}
		else if (current_path.has_parent_path())
		{
			load_entries_from_specific_path(current_path.parent_path());

			current_path = current_path.parent_path();


		}

		return;
	}

	bool can_go_forward() 
	{
		if (paths_undo.empty()) return false;
		return true;
	}

	void redo(fs::path& current_path)
	{
		if (paths_undo.empty()) 
		{
			// throw std::runtime_error("void redo(): there is no path to redo");
			return;
		}
		fs::path path = paths_undo.top(); paths_undo.pop();
		load_entries_from_specific_path(path);
		current_path = path;
		return;
	}

	fs::path get_forward_path() 
	{
		if (!paths_undo.empty()) return paths_undo.top();
		else 
		{
			throw std::runtime_error("void redo(): there is no path to redo");
		}
	}

	void copy_paste(const fs::path& from, const fs::path& to) 
	{
		if (!fs::exists(from)) 
		{
			std::cerr << "Error: source path doesnot exist: " << from << std::endl;
			return;
		}
		std::error_code ec;

		fs::copy_options options = fs::copy_options::recursive | fs::copy_options::overwrite_existing;

		fs::path dest = to / from.filename();

		fs::copy(from, dest, options, ec);

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

	void sort_entries() 
	{
		std::sort(entries.begin(), entries.end(), [&](fs::path& a, fs::path& b) { return a < b; });
		return;
	}

	void print_entries() 
	{
		for (const fs::directory_entry& entry : entries) 
		{
			std::cout << entry.path() << std::endl;
		}
	}

};