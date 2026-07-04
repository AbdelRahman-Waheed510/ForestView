#pragma once 

#include <filesystem>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <windows.h>
#include <format>
namespace fs = std::filesystem;

class Explorer 
{
private:

	std::vector<fs::path> entries;
	std::stack<fs::path> paths_undo;

public:
	
	Explorer() 
	{
		start_the_explorer();
	}
	
	~Explorer() {};

	void start_the_explorer() 
	{
		if (!entries.empty()) entries.clear();
		for (int i = 0; i < 26; ++i) {
			char drive_letter = static_cast<char>('A' + i);
			fs::path drive_str = std::format("{}:\\", drive_letter);

			if (fs::exists(drive_str)) {
				entries.emplace_back(drive_str);
			}
		}
	}

	void load_entries_from_specific_path(const fs::path& path ) 
	{
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

			entries.emplace_back(entry.path());
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

	std::vector<fs::path> get_entries()
	{
		return entries;
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
			throw std::runtime_error("void redo(): there is no path to redo");
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
	void print_entries() 
	{
		for (const fs::path& path : entries) 
		{
			std::cout << path << std::endl;
		}
	}
};