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

namespace fs = std::filesystem;

class Explorer 
{
private:
	std::vector<fs::path> entries;
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
				entries.push_back(fs::path(drive));
				drive += wcslen(drive) + 1; 
			}
		}
		return;
	}

	void load_entries_from_specific_path(const fs::path& path) 
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
		return;
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

	void create_folder(fs::path& to_create, const fs::path& name) 
	{
		
		fs::path newFolder = to_create / name;
		if (fs::exists(newFolder)) return;
		fs::create_directory(newFolder);
		return;
	}

	void rename_file(fs::path& to_rename, const fs::path new_name) 
	{
		fs::path delted_path = to_rename;
		delted_path.remove_filename();
		fs::path newPath = delted_path / new_name;
		fs::rename(to_rename, newPath);
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