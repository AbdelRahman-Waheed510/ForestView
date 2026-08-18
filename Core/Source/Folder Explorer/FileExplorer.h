#pragma once 

#include <filesystem>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <windows.h>
#include <format>
#include <stack>
#include <stdexcept>
#include <ostream>
#include <fstream>
#include <istream>
#include "tree.h"
#include <functional>
#include <chrono>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Texture.h"
#include "SHA256.h"

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

enum class DirAccessResult {
	OK,
	ACCESS_DENIED,
	NOT_FOUND,
	NOT_A_DIRECTORY,
	OTHER
};

namespace Utils 
{
	std::string generate_new_folder_name(const fs::path& path, const std::string& name, const NewType& type);

	std::string to_lower(std::string str);

	std::string reform_string(const std::string& str, const int& count);
	
	const std::string file_type_to_string(const fs::file_type& type);
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

enum class FilterOptions 
{
NONE,
FILES,
FOLDERS,
ALL
};

class Explorer 
{
private:

	struct Guard 
	{
		std::atomic<bool>& m_finished;
		Guard(std::atomic<bool>& finished) : m_finished(finished)
		{
		}
		~Guard()
		{
			m_finished = true;
		}
	};

	TexturesManager& M_texturesManager;	
	tree* t;
	std::vector<Entry> entries;
	std::stack<fs::path> redo_stack;
	std::stack<fs::path> undo_stack;
	
	FilterOptions filterBy = FilterOptions::ALL;
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

		auto local = std::chrono::zoned_time{ std::chrono::current_zone(), sys_time };

		std::string res = std::format("{:%Y-%m-%d %H:%M}", local);

		return res;

	}

	DirAccessResult check_directory_access(const fs::path& path)
	{
		std::error_code ec;
		auto it = fs::directory_iterator(path, ec);


		if (!ec) return DirAccessResult::OK;

		if (ec == std::errc::permission_denied)
			return DirAccessResult::ACCESS_DENIED;
		if (ec == std::errc::no_such_file_or_directory)
			return DirAccessResult::NOT_FOUND;
		if (ec == std::errc::not_a_directory)
			return DirAccessResult::NOT_A_DIRECTORY;

		return DirAccessResult::OTHER;
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

			if (attr & (/*FILE_ATTRIBUTE_HIDDEN | */FILE_ATTRIBUTE_SYSTEM))
				continue; // skip hidden/system

			switch (filterBy)
			{
			case FilterOptions::FILES: 
			{
				if (entry.is_regular_file()) 
				{
					Entry entryToAdd = setup_entry(entry);
					entries.emplace_back(entryToAdd);
				}
				break;
			}
			case FilterOptions::FOLDERS:
			{
				if (entry.is_directory())
				{
					Entry entryToAdd = setup_entry(entry);
					entries.emplace_back(entryToAdd);
				}
				break;
			}
			case FilterOptions::ALL:
			{
				Entry entryToAdd = setup_entry(entry);
				entries.emplace_back(entryToAdd);
				break;
			}
			case FilterOptions::NONE:
			{
				Entry entryToAdd = setup_entry(entry);
				entries.emplace_back(entryToAdd);
				break;
			}
			default:
				break;
			}
		}
		return;
	}

	Entry setup_entry(const fs::directory_entry& entry) 
	{
		Entry e;
		e.entry = entry;
		e.textureId = M_texturesManager.decide_icon(entry);
		auto u8_str = (entry.path() == entry.path().root_path()) ? entry.path().u8string() : entry.path().filename().u8string();
		std::string utf8_path(u8_str.begin(), u8_str.end());
		e.name = utf8_path;
		return e;
	}

	void set_filter_by(const FilterOptions& option) 
	{
		filterBy = option;
		return;
	}

	const FilterOptions get_filter_by() 
	{
		return filterBy;
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

		fs::path path_to_undo;
		while (!undo_stack.empty()) 
		{
			if (undo_stack.top().empty()) 
			{
				path_to_undo = ""; undo_stack.pop();
				redo_stack.push(current_path);
				break;
			}
			else if (check_directory_access(undo_stack.top()) == DirAccessResult::NOT_FOUND) undo_stack.pop();
			else
			{
				path_to_undo = undo_stack.top(); undo_stack.pop();
				redo_stack.push(current_path);
				break;
			}
		}
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
		fs::path path_to_redo;

		while (!redo_stack.empty())
		{
			DirAccessResult state = check_directory_access(redo_stack.top());
			if (state == DirAccessResult::NOT_FOUND) redo_stack.pop();
			else
			{
				path_to_redo = redo_stack.top(); redo_stack.pop();
				undo_stack.push(current_path);
				load_entries_from_specific_path(path_to_redo);
				current_path = path_to_redo;
				break;
			}
		}
	}

	fs::path get_forward_path() 
	{
		if (!redo_stack.empty()) return redo_stack.top();
		else 
		{
			throw std::runtime_error("void redo(): there is no path to redo");
		}
	}

	void copy_paste(const fs::path& source, const fs::path& destination, std::atomic<double>& Sizeprogress, std::atomic<int>& filesCountProgress, std::atomic<bool>& cancel_request, std::atomic<bool>& finished)
	{
		Guard guard(finished);

		if (!fs::exists(source))
		{
			std::cerr << "Error: source path doesnot exist: " << source << std::endl;
			return;
		}
		std::error_code ec;

		// If the copied directory is a folder 
		if (fs::is_directory(source))
		{
			std::string name_of_the_folder = Utils::generate_new_folder_name(destination, source.filename().string(), FOLDER);
			fs::path dest = destination / name_of_the_folder;

			std::error_code ec_longPath;

			fs::create_directories(dest, ec_longPath);

			if (ec_longPath) 
			{
				std::cerr << "Failed to Create" << dest << std::endl;
			}

			for (auto entry = fs::recursive_directory_iterator(source, fs::directory_options::skip_permission_denied, ec); entry != fs::recursive_directory_iterator(); entry.increment(ec))
			{
				if (cancel_request)
				{
					return;
				}
				fs::path relativePath = fs::relative(entry->path(), source);
				fs::path targetPath = dest / relativePath;

				if (entry->is_directory())
				{
					std::error_code ec_longPath;
					fs::create_directories(targetPath,ec_longPath);
					if (ec_longPath)
					{
						std::cerr << "Failed to Create" << targetPath << std::endl;
					}
				}

				else if (entry->is_regular_file())
				{
					copy_in_chuncks(entry->path(), targetPath, Sizeprogress, filesCountProgress, cancel_request);
					if (cancel_request) return;

				}

				if (ec)
				{
					continue;
				}
			}
		}

		// If the copied directory is a regular file so it does not need the access to the for loop
		else if (fs::is_regular_file(source))
		{
			std::string name_of_the_folder = Utils::generate_new_folder_name(destination, source.filename().string(), REG_FILE);
			fs::path dest = destination / name_of_the_folder;

			copy_in_chuncks(source, dest, Sizeprogress, filesCountProgress, cancel_request);
		}

		if (ec) 
		{
			std::cerr << "Copy failed: " << ec.message() << '\n';
		}
		
		return;
	}

	void copy_in_chuncks(const fs::path& source, const fs::path& destination, std::atomic<double>& Sizeprogress, std::atomic<int>& filesCountProgress, std::atomic<bool>& cancel_request)
	{
		constexpr int BufferSize = 1024 * 1024; // 1 MB

		std::ifstream readFrom(source, std::ios::binary);

		if (!readFrom.is_open()) return;

		std::ofstream destionationFile(destination, std::ios::binary);

		if (!destionationFile.is_open()) return;

		std::vector<char> buffer(BufferSize);

		while (readFrom)
		{
			if (cancel_request)
			{
				readFrom.close();
				destionationFile.close();
				std::filesystem::remove(destination);
				return;
			}

			readFrom.read(buffer.data(), BufferSize);

			std::streamsize bytes = readFrom.gcount();

			if (bytes > 0)
			{
				destionationFile.write(buffer.data(), bytes);

				Sizeprogress += bytes;

				if (!destionationFile) return;
			}

		}
		filesCountProgress++;
	}

	void remove(const fs::path& path_to_delete) 
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
#ifdef _DEBUG
		std::cout << "Number of files deleted: " << deleted_files << std::endl;
#endif
		return;
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

	void find_empty_folders(const fs::path& source, std::vector<fs::path>& empty_folders, std::atomic<bool>& cancel_request, std::atomic<bool>& finished)
	{
		Guard guard(finished);
		if (!empty_folders.empty()) empty_folders.clear();
		std::error_code ec;
		if (!fs::is_directory(source, ec)) return;
		
		if (fs::is_empty(source, ec) && !ec) 
		{
			empty_folders.emplace_back(source);
			return;
		}
		
		for (const fs::directory_entry& entry : fs::recursive_directory_iterator(source, fs::directory_options::skip_permission_denied, ec))
		{
			if (cancel_request == true) break;
			if (entry.is_directory(ec) && !ec) 
			{
				if (fs::is_empty(entry.path(), ec) && !ec) 
				{
					empty_folders.emplace_back(entry.path());
				}
			}
		}
		return;
	}
	
	void print_entries() 
	{
		for (const Entry& entry : entries) 
		{
			std::cout << entry.entry.path() << std::endl;
		}
	}

	// Duplicates APIs
	void find_duplicates(std::vector<std::vector<fs::directory_entry>>& duplicates, const fs::path target, std::atomic<bool>& cancel_request, std::atomic<bool>& finished)
	{
		Guard guard(finished);
		if (cancel_request) 
		{
			return;
		}
		std::error_code ec;

		std::vector<fs::directory_entry> candidets;

		// putting the files in the candidtes vector [First Round]
		for (const auto& entry : fs::directory_iterator(target,fs::directory_options::skip_permission_denied, ec))
		{
			if (cancel_request)
			{
				return;
			}
			if (ec) 
			{
				std::cerr << "find_duplicates() Error 1st call for ec...." << std::endl;
				continue;
			}
			
			if (entry.is_regular_file(ec)) 
			{
				if (ec) 
				{
					std::cerr << "find_duplicates() Error 2nd call for ec...." << std::endl;
					continue;
				}
				candidets.emplace_back(entry);
			}
		
		}

		// [First Filter] Comparing the sizes of the files and remove the ones that has no similer size [Second Round]
		if (cancel_request)
		{
			return;
		}
		std::sort(candidets.begin(), candidets.end(), [](const fs::directory_entry& a, const fs::directory_entry& b)
			{
				return a.file_size() < b.file_size();
			});
		int count = 0;
		int iterator = 0;
		for (size_t i = 0; i < candidets.size(); ++i) 
		{
			if (cancel_request)
			{
				return;
			}
			if (i + 1 >= candidets.size())
			{
				if (count == 0)
				{
					continue;
				}
				else if (count != 0)
				{
					candidets[iterator] = candidets[i];
					iterator++;
					count = 0;
				}
				break;
			}
			if (candidets[i].file_size() == candidets[i + 1].file_size()) 
			{
				candidets[iterator] = candidets[i];
				count++;
				iterator++;
			}
			else 
			{
				if (count == 0)
				{
					continue;
				}
				else if (count != 0)
				{
					candidets[iterator] = candidets[i];
					iterator++;
					count = 0;
				}
			}
		}

		// [Second Filter] Compating the first 64 Bytes [Third Round]
		std::unordered_map<std::string, std::vector<fs::directory_entry>> groups;

		for (size_t i = 0; i < iterator; i++)
		{
			if (cancel_request)
			{
				return;
			}
			std::ifstream read(candidets[i].path(), std::ios::binary);
			if (!read)
			{
				continue;
			}
			std::array<uint8_t, 64> buffer;

			read.read(reinterpret_cast<char*>(buffer.data()), 64);
			std::streamsize bytesCount = read.gcount();

			HashForge::SHA256 Hasher;
			Hasher.Update(std::span<const uint8_t>(
				buffer.data(),
				static_cast<size_t>(bytesCount)
			));
			groups[Hasher.Finalize()].emplace_back(candidets[i]);
			read.close();
		}
		iterator = 0;
		for (const auto& [key, value] : groups) 
		{
			if (cancel_request)
			{
				return;
			}
			if (value.size() < 2) 
			{
				continue;
			}
			for (const auto& entry : value) 
			{
				candidets[iterator] = entry;
				iterator++;
			}
		}

		// [Third Filter] Hashing each remaining file form the last 2 filters and compare the hashes
		std::unordered_map<std::string, std::vector<fs::directory_entry>> groups2;
		for (size_t i = 0; i < iterator; i++)
		{
			if (cancel_request)
			{
				return;
			}
			HashForge::SHA256 Hasher;
			groups2[Hasher.HashFile(candidets[i], cancel_request)].emplace_back(candidets[i]);
		}

		for (const auto& [key, value] : groups2)
		{
			if (cancel_request)
			{
				return;
			}
			if (value.size() < 2)
			{
				continue;
			}
			duplicates.emplace_back(value);
		}
		return;
	}
};