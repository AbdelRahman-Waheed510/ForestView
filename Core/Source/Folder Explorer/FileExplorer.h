#pragma once 
#include "tree.h"
#include "Texture.h"
#include "SHA256.h"
#include <fribidi/fribidi.h>

namespace fs = std::filesystem;

class Timer
{
private:
	using Clock = std::chrono::high_resolution_clock;
	using Microseconds = std::chrono::microseconds;

	Clock::time_point m_startTimePoint;
	Microseconds m_elapsedTime{ 0 };
	bool m_running = false;

public:
	Timer(bool startTheTimer = false)
	{
		if (startTheTimer)
		{
			Restart();
		}
	}

	void Restart()
	{
		m_elapsedTime = Microseconds(0);
		m_startTimePoint = Clock::now();
		m_running = true;
	}

	void Stop()
	{
		if (m_running)
		{
			auto now = Clock::now();
			m_elapsedTime += std::chrono::duration_cast<Microseconds>(now - m_startTimePoint);
			m_running = false;
		}
	}

	void Resume()
	{
		if (!m_running)
		{
			m_startTimePoint = Clock::now();
			m_running = true;
		}
	}

	const bool IsRunning() const { return m_running; }

	float GetElapsedMicroseconds() const
	{
		if (m_running)
		{
			auto now = Clock::now();
			return static_cast<float>((m_elapsedTime + std::chrono::duration_cast<Microseconds>(now - m_startTimePoint)).count());
		}
		return static_cast<float>(m_elapsedTime.count());
	}

	float GetElapsedSeconds() const
	{
		return GetElapsedMicroseconds() * 0.000001f;
	}

	float GetElapsedMilliseconds() const
	{
		return GetElapsedMicroseconds() * 0.001f;
	}

	void load_to_txt_file()
	{
		float us = GetElapsedMicroseconds();
		std::ofstream speedLog("Log.txt", std::ios::app);
		if (speedLog.is_open())
		{
			speedLog << us << " MicroSeconds\n";
			speedLog << (us * 0.001f) << " MilliSeconds\n";
			speedLog << (us * 0.000001f) << " Seconds\n";
			speedLog << "=============================================================\n";
		}
	}
};

struct Entry 
{
	fs::directory_entry entry;
	GLuint textureId;
	std::string name;
	uint32_t id;
};

struct fileSize
{
	double size;
	std::string label;
};

enum class NewType
{
	FOLDER, 
	REG_FILE,
	TEXT_DOCUMENT,           // .txt
	POWER_POINT,			 // .pptx
	WORD,					 // .docx
	RAR,					 // .rar
	ZIP,					 // .zip
	EXCEL,					 // .xlsx
	BITMAP,					 // .bmp
	PUBLISHER_DOCUMENT,		 // .pub
	ACCESS_DATABASE          // .accdb
};

enum class SortType 
{
	NAME,
	SIZE,
	LAST_MODIFIED,
};

enum class SortOrder
{
	ASCENDING,
	DESCENDING
};

enum class AccessResult
{
	OK,
	NOT_FOUND,
	ACCESS_DENIED,
	LOCKED_OR_IN_USE, // This is only for files
	NOT_A_DIRECTORY,
	NOT_READY,         
	SYMLINK_LOOP,
	OTHER
};

namespace Utils 
{
	std::string generate_new_folder_name(const fs::path& path, const std::string& name, const NewType& type);

	std::string to_lower(std::string str);

	std::wstring to_lowerW(std::wstring s);

	std::string reform_string(const std::string& str, const int& count);
	
	const std::string file_type_to_string(const fs::file_type& type);

	inline fs::path utf8_to_path(const std::string& s);

	inline std::string path_to_utf8(const fs::path& p);

	std::string to_display_sring(const std::string& utf8);
};

struct RenameReturn 
{
	std::optional<fs::path> finalPath;
	bool success;
	std::optional<std::string> msg;
};

struct CopyReturn 
{
	std::optional<fs::path> finalPath;
	std::optional<std::string> msg;
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

struct Duplicate 
{
	fs::path path;
	std::uintmax_t size;
	std::string name;
	int groupId = 0;

	Duplicate(const fs::path& path, const std::uintmax_t sze, const int gid)
	{
		update_path(path);
		size = sze;
		groupId = gid;
	}

	void update_path(const fs::path& pa)
	{
		path = pa;
		name = Utils::to_display_sring(Utils::path_to_utf8(pa));
	}

	bool operator==(const Duplicate& other) const = default;

	bool operator==(const fs::path& otherPath) const {
		return path == otherPath;
	}

};

struct EmptyFolder 
{
	fs::path path;
	std::string name;

	EmptyFolder(const fs::path& path) 
	{
		update_path(path);
	}

	void update_path(const fs::path& pa) 
	{
		path = pa;
		name = Utils::to_display_sring(Utils::path_to_utf8(pa));
	}

	bool operator==(const EmptyFolder& other) const = default;

	bool operator==(const fs::path& otherPath) const {
		return path == otherPath;
	}
};

struct LargestFile 
{
	fs::path path;
	std::uintmax_t size;
	std::string name;

	LargestFile(const fs::path& path, const std::uintmax_t sze)
	{
		update_path(path);
		size = sze;
	}

	void update_path(const fs::path& pa)
	{
		path = pa;
		name = Utils::to_display_sring(Utils::path_to_utf8(pa));
	}

	bool operator==(const LargestFile& other) const = default;

	bool operator==(const fs::path& otherPath) const {
		return path == otherPath;
	}
};

enum class SearchBy 
{
NAME,
EXTENTION,
};

enum class SearchFor
{
FILE,
FOLDER,
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
	uint32_t ID = 0;
	TexturesManager& M_texturesManager;	
	tree* t;
	std::vector<Entry> entries;
	std::stack<fs::path> redo_stack;
	std::stack<fs::path> undo_stack;
	
	FilterOptions filterBy = FilterOptions::ALL;

	SortType m_activeSortType = SortType::NAME;
	SortOrder m_activeSortOrder = SortOrder::ASCENDING;

private:

	void sort_entries();

	CopyReturn copy_paste_helper(const fs::path& source,const fs::path& destination,std::atomic<double>& Sizeprogress,std::atomic<int>& filesCountProgress,std::atomic<bool>& cancel_request)
	{
		if (!fs::exists(source))
		{
			std::string e = std::format("Source path [{}] does not exist.", Utils::to_display_sring(Utils::path_to_utf8(source)));
			return CopyReturn(std::nullopt, e);
		}
		std::error_code ec;

		std::string name_of_the_folder;
		fs::path dest;
		// If the copied directory is a folder 
		if (fs::is_directory(source))
		{
			if (is_subpath(destination, source))
			{
				std::string e = std::format("Cannot copy [{}] \ninto its own subfolder [{}].",
					Utils::to_display_sring(Utils::path_to_utf8(source)),
					Utils::to_display_sring(Utils::path_to_utf8(destination)));
				return CopyReturn(std::nullopt, e);
			}

			auto u8_str1 = source.filename().u8string();
			std::string utf8_path1(u8_str1.begin(), u8_str1.end());

			name_of_the_folder = Utils::generate_new_folder_name(destination, utf8_path1, NewType::FOLDER);
			dest = destination / Utils::utf8_to_path(name_of_the_folder);
			std::error_code ec_longPath;

			fs::create_directories(dest, ec_longPath);

			if (ec_longPath)
			{
				std::string e = std::format("Failed to create target directory [{}]: {}", Utils::to_display_sring(Utils::path_to_utf8(dest)), ec_longPath.message());
				return CopyReturn(std::nullopt, e);
			}

			for (auto entry = fs::recursive_directory_iterator(source, fs::directory_options::skip_permission_denied, ec); entry != fs::recursive_directory_iterator(); entry.increment(ec))
			{
				if (cancel_request)
				{
					return CopyReturn(dest, std::nullopt);
				}
				fs::path relativePath = fs::relative(entry->path(), source);
				fs::path targetPath = dest / relativePath;

				if (entry->is_directory())
				{
					std::error_code ec_longPath;
					fs::create_directories(targetPath, ec_longPath);
					if (ec_longPath)
					{
						std::string e = std::format("Failed to create target directory [{}]: {}", Utils::to_display_sring(Utils::path_to_utf8(targetPath)), ec_longPath.message());
					}
				}

				else if (entry->is_regular_file())
				{
					copy_in_chuncks(entry->path(), targetPath, Sizeprogress, filesCountProgress, cancel_request);
					if (cancel_request) return CopyReturn(dest, std::nullopt);

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
			auto u8_str1 = source.filename().u8string();
			std::string utf8_path1(u8_str1.begin(), u8_str1.end());

			name_of_the_folder = Utils::generate_new_folder_name(destination, utf8_path1, NewType::REG_FILE);
			dest = destination / Utils::utf8_to_path(name_of_the_folder);

			copy_in_chuncks(source, dest, Sizeprogress, filesCountProgress, cancel_request);
		}

		if (ec)
		{
			return CopyReturn(dest, std::format("Copy completed with errors: {}", ec.message()));
		}

		return CopyReturn(dest, std::nullopt);
	}

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

	void sort_entries(const SortType& type) 
	{
		if (type == m_activeSortType) 
		{
			if (m_activeSortOrder == SortOrder::ASCENDING) m_activeSortOrder = SortOrder::DESCENDING;
			else m_activeSortOrder = SortOrder::ASCENDING;
		}
		m_activeSortType = type;
		sort_entries();
	}

	void sort_entries(const SortOrder& orderType) 
	{
		if (orderType == m_activeSortOrder) return;
		m_activeSortOrder = orderType;
		sort_entries();
	}

	const SortType get_active_sort_tyoe() 
	{
		return m_activeSortType;
	}

	const SortOrder get_active_sort_order()
	{
		return m_activeSortOrder;
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
				std::error_code statEc;
				if (fs::is_regular_file(entry.status(statEc)) && !statEc)
				{
					std::error_code ec2;
					data.directorySize += entry.file_size(ec2);
					if (ec2) 
					{
					}
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
			std::error_code sizeEc;
			auto sz = fs::file_size(path, sizeEc);
			if (!sizeEc) { data.directorySize += sz; data.filesCount++; }
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

	AccessResult check_access(const fs::path& path)
	{
		std::error_code ec;
		auto st = fs::status(path, ec);

		if (st.type() == fs::file_type::not_found)
			return AccessResult::NOT_FOUND;

		if (ec)
		{
			if (ec.value() == ERROR_ACCESS_DENIED || ec == std::errc::permission_denied)
				return AccessResult::ACCESS_DENIED;
			if (ec.value() == ERROR_NOT_READY)
				return AccessResult::NOT_READY;
			if (ec == std::errc::too_many_symbolic_link_levels)
				return AccessResult::SYMLINK_LOOP;
			return AccessResult::OTHER;
		}

		if (st.type() == fs::file_type::directory)
		{
			std::error_code dir_ec;
			fs::directory_iterator it(path, dir_ec);

			if (!dir_ec) return AccessResult::OK;

			if (dir_ec.value() == ERROR_ACCESS_DENIED || dir_ec == std::errc::permission_denied)
				return AccessResult::ACCESS_DENIED;
			if (dir_ec == std::errc::not_a_directory)
				return AccessResult::NOT_A_DIRECTORY;
			return AccessResult::OTHER;
		}

		std::ifstream file(path, std::ios::binary);
		if (!file.is_open())
		{
			if (errno == EACCES || ec == std::errc::permission_denied)
				return AccessResult::ACCESS_DENIED;
			return AccessResult::LOCKED_OR_IN_USE;
		}

		return AccessResult::OK;
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
		sort_entries();
		return;
	}

	Entry setup_entry(const fs::directory_entry& entry) 
	{
		Entry e;
		e.entry = entry;
		e.textureId = M_texturesManager.decide_icon(entry);
		e.id = ID++;
		auto u8_str = (entry.path() == entry.path().root_path()) ? entry.path().u8string() : entry.path().filename().u8string();
		std::string utf8_path(u8_str.begin(), u8_str.end());
		e.name = Utils::to_display_sring(utf8_path);
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

	void search(const fs::path& path, std::string keyword)
	{
		if (path.empty())
		{
			return ;
		}
			
		entries.clear();

		const std::wstring needle = Utils::to_lowerW(Utils::utf8_to_path(keyword).wstring());

		std::error_code ec;
		fs::directory_iterator it(path, fs::directory_options::skip_permission_denied, ec);
		if (ec) return ;
		for (; it != fs::directory_iterator(); it.increment(ec))
		{
			if (ec) break;

			const fs::directory_entry& entry = *it;

			DWORD attr = GetFileAttributesW(entry.path().c_str());

			if (attr == INVALID_FILE_ATTRIBUTES)
				continue; // couldn't stat it, skip

			if (attr & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM))
				continue; // skip hidden/system

			const std::wstring name = Utils::to_lowerW(entry.path().filename().wstring());;

			if (name.find(needle) != std::wstring::npos)
			{
				Entry e = setup_entry(entry);
				entries.emplace_back(std::move(e));
			}
		}
		return;
	}

	const std::vector<Entry>& get_entries()
	{
		return entries;
	}

	const fs::path create_folder(fs::path& to_create, const std::string& name, const NewType& type) 
	{
		std::error_code ec;
		if (!fs::exists(to_create,ec) || ec) 
		{
			return "";
		}
		if (type == NewType::FOLDER) 
		{
			std::error_code ec;
			fs::path newFolder = to_create / name;
			if (fs::create_directory(newFolder, ec)) return newFolder;
			if (ec) 
			{
				std::cerr << "Error when creating " << name << std::endl;
				return "";
			}
		}
		else
		{
			fs::path newFilePath = to_create / name;
			switch (type)
			{
			case NewType::RAR:
			{
				bool success = fs::copy_file("assets/templates/temp.rar", newFilePath);
				if (!success) return "";
				return newFilePath;
				break;
			}
			case NewType::ZIP:
			{
				bool success = fs::copy_file("assets/templates/temp.zip", newFilePath);
				if (!success) return "";
				return newFilePath;
				break;
			}
			case NewType::EXCEL:
			{
				bool success = fs::copy_file("assets/templates/temp.xlsx", newFilePath);
				if (!success) return "";
				return newFilePath;
				break;
			}
			case NewType::ACCESS_DATABASE:
			{
				bool success = fs::copy_file("assets/templates/temp.accdb", newFilePath);
				if (!success) return "";
				return newFilePath;
				break;
			}
			case NewType::PUBLISHER_DOCUMENT:
			{
				bool success = fs::copy_file("assets/templates/temp.pub", newFilePath);
				if (!success) return "";
				return newFilePath;
				break;
			}
			default: 
			{
				std::ofstream newfile(newFilePath);
				return newFilePath;
				break;
			}
			}
		}

		return "";
	}

	RenameReturn rename_file(fs::path& to_rename, fs::path new_name)
	{
		if (new_name.empty()) return RenameReturn(std::nullopt, false, "New name cannot be empty.");

		std::error_code ec;

		if (fs::is_regular_file(to_rename, ec))
		{
			if (new_name.has_extension())
			{
				if (new_name.extension() != to_rename.extension()) return RenameReturn(std::nullopt, false, "File extension cannot be changed during rename.");
			}
			else
			{
				new_name.replace_extension(to_rename.extension());
			}
		}

		const fs::path parent = to_rename.parent_path();
		const fs::path target = parent / new_name;

		// Renaming to the exact same path is a no-op (also avoids creating "name (1)")
		if (target == to_rename) return RenameReturn(std::nullopt, false, "New name is the same as the current name.");

		if (fs::is_directory(target, ec))
		{
			new_name = Utils::utf8_to_path(Utils::generate_new_folder_name(
				parent, Utils::path_to_utf8(new_name.filename()), NewType::FOLDER));
		}
		else if (fs::is_regular_file(target, ec))
		{
			new_name = Utils::utf8_to_path(Utils::generate_new_folder_name(
				parent, Utils::path_to_utf8(new_name.filename()), NewType::REG_FILE));
		}

		fs::path newPath = parent / new_name;

		fs::rename(to_rename, newPath, ec);
		if (ec)
		{
			if (ec == std::errc::permission_denied ||
				ec == std::errc::device_or_resource_busy ||
				ec.value() == 5 /* ERROR_ACCESS_DENIED */ ||
				ec.value() == 32 /* ERROR_SHARING_VIOLATION */)
			{
				const std::string error = "The folder is currently being used by another program";
				return RenameReturn(std::nullopt, false, error);
			}
			const std::string error = "Failed to rename file: " + ec.message();
			return RenameReturn(std::nullopt, false, error);
		}
		return RenameReturn(newPath, true, std::nullopt);
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
			else if (check_access(undo_stack.top()) == AccessResult::NOT_FOUND) undo_stack.pop();
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

	bool can_go_back() 
	{
		return !undo_stack.empty();
	}

	void redo(fs::path& current_path)
	{
		if (redo_stack.empty()) return;
		fs::path path_to_redo;

		while (!redo_stack.empty())
		{
			AccessResult state = check_access(redo_stack.top());
			if (state == AccessResult::NOT_FOUND) redo_stack.pop();
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

	bool is_subpath(const fs::path& child, const fs::path& parent)
	{
		std::error_code ec;

		auto childPath = fs::weakly_canonical(child, ec);
		if (ec) return false;

		auto parentPath = fs::weakly_canonical(parent, ec);
		if (ec) return false;

		auto [parentIt, childIt] = std::mismatch(parentPath.begin(), parentPath.end(), childPath.begin(), childPath.end());

		return parentIt == parentPath.end();
	}


	const CopyReturn copy_paste(const fs::path& source, const fs::path& destination, std::atomic<double>& Sizeprogress, std::atomic<int>& filesCountProgress, std::atomic<bool>& cancel_request, std::atomic<bool>& finished)
	{
		Guard guard(finished);
		return copy_paste_helper(source, destination, Sizeprogress, filesCountProgress, cancel_request);
	}

	const CopyReturn cut_paste(const fs::path& source, const fs::path& destination, std::atomic<double>& Sizeprogress, std::atomic<int>& filesCountProgress, std::atomic<bool>& cancel_request, std::atomic<bool>& finished)
	{
		Guard guard(finished);

		if (!fs::exists(source)) {
			std::string e = std::format("Source path [{}] does not exist.", Utils::to_display_sring(Utils::path_to_utf8(source)));
			return CopyReturn(std::nullopt, e);
		}

		std::error_code ec;

		auto u8_filename = source.filename().u8string();
		std::string utf8_name(u8_filename.begin(), u8_filename.end());
		NewType type = fs::is_directory(source) ? NewType::FOLDER : NewType::REG_FILE;

		std::string new_name = Utils::generate_new_folder_name(destination, utf8_name, type);
		fs::path target_dest = destination / Utils::utf8_to_path(new_name);

		// 2. Fast Path: Try atomic OS rename (same partition/drive)
		fs::rename(source, target_dest, ec);

		if (!ec) {
			// Fast path succeeded!
			return CopyReturn(target_dest, std::nullopt);
		}

		// 3. Fallback: Cross-drive move (Copy + Delete)
		// IMPORTANT: Bypass Guard in copy_paste to avoid double-guard issues
		CopyReturn copy_res = copy_paste_helper(source, target_dest, Sizeprogress, filesCountProgress, cancel_request);

		// If canceled by user, do NOT delete source!
		if (cancel_request) {
			return copy_res;
		}

		// If copy succeeded without errors, remove the source
		if (copy_res.finalPath.has_value() && !copy_res.msg.has_value()) {
			std::error_code remove_ec;
			fs::remove_all(source, remove_ec);

			if (remove_ec) {
				return CopyReturn(copy_res.finalPath, std::format("Failed to remove source after copying: {}", remove_ec.message()));
			}
		}

		return copy_res;
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

	void permanent_delete(const fs::path& path_to_delete) 
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

	void recycleBin_delete(const fs::path& path_to_delete)
	{
		if (!fs::exists(path_to_delete))
		{
			std::cerr << "Error: source path doesnot exist.....! " << std::endl;
			return;
		}
		// Initialize COM library for the current thread
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (FAILED(hr))
		{
			std::cerr << "Failed to initialize COM library. HRESULT: " << hr << std::endl;
			return;
		}

		IFileOperation* pfo = nullptr;
		bool success = false;

		// Create instance of IFileOperation
		hr = CoCreateInstance(CLSID_FileOperation, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pfo));
		if (SUCCEEDED(hr))
		{
			hr = pfo->SetOperationFlags(FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOERRORUI);

			if (SUCCEEDED(hr))
			{
				IShellItem* psiTarget = nullptr;
				// Create a Shell Item from the absolute path string
				std::wstring wPath = path_to_delete.wstring();
				hr = SHCreateItemFromParsingName(wPath.c_str(), NULL, IID_PPV_ARGS(&psiTarget));

				if (SUCCEEDED(hr))
				{
					// Queue the item for deletion
					hr = pfo->DeleteItem(psiTarget, nullptr);
					psiTarget->Release();

					if (SUCCEEDED(hr))
					{
						// Execute the actual move to Recycle Bin
						hr = pfo->PerformOperations();
						if (SUCCEEDED(hr))
						{
							success = true;
						}
					}
				}
			}
			pfo->Release();
		}

		// Clean up COM initialization
		CoUninitialize();

		// Error reporting matching your design
		if (!success)
		{
			std::cerr << "Failed to wipe directory using IFileOperation. HRESULT: " << hr << '\n';
		}

#ifdef _DEBUG
		if (success)
		{
			std::cout << "Directory successfully sent to Recycle Bin." << std::endl;
		}
#endif
		return;
	}

	std::string form_breadcrumb_navigation_string(const fs::path& path_to_format) 
	{
		const int LIMIT = 100;
		std::string result = "";

		for (const auto& part : path_to_format) 
		{
			if (result.empty()) 
			{
				auto u8_str = part.u8string();
				std::string utf8_path(u8_str.begin(), u8_str.end());

				result += Utils::to_display_sring(utf8_path);
				continue;
			}
			auto u8_str = part.u8string();
			std::string utf8_path(u8_str.begin(), u8_str.end());

			result += Utils::to_display_sring(utf8_path);
			result += " > ";
		}
		if ((int)result.size() > LIMIT)
		{
			int max = (int)result.size() - LIMIT;
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

	void find_empty_folders(const fs::path& source, std::vector<EmptyFolder>& empty_folders, std::atomic<bool>& cancel_request, std::atomic<bool>& finished)
	{
		Guard guard(finished);
		if (!empty_folders.empty()) empty_folders.clear();
		std::error_code ec;
		if (!fs::is_directory(source, ec)) return;
		
		if (fs::is_empty(source, ec) && !ec) 
		{
			empty_folders.emplace_back(EmptyFolder(source));
			return;
		}
		
		for (fs::recursive_directory_iterator it(
			source,
			fs::directory_options::skip_permission_denied,
			ec);
			it != fs::recursive_directory_iterator();
			it.increment(ec))
		{
			if (cancel_request)
				break;

			const auto& entry = *it;

			if (entry.is_directory(ec) && !ec)
			{
				DWORD attributes = GetFileAttributesW(entry.path().c_str());

				if (attributes != INVALID_FILE_ATTRIBUTES)
				{
					if (attributes & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM))
					{
						it.disable_recursion_pending();
						continue;
					}
				}

				if (fs::is_empty(entry.path(), ec) && !ec) 
				{
					empty_folders.emplace_back(EmptyFolder(entry.path()));
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
	void find_duplicates(std::vector<Duplicate>& duplicates, const fs::path target, std::atomic<bool>& cancel_request, std::atomic<bool>& finished)
	{
		Guard guard(finished);
		if (cancel_request) 
		{
			return;
		}
		std::error_code ec;

		std::vector<fs::directory_entry> candidets;

		// putting the files in the candidtes vector [First Round]
		for (const auto& entry : fs::recursive_directory_iterator(target,fs::directory_options::skip_permission_denied, ec))
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

		int groupIdTemp = 0;
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

			for (const fs::directory_entry& entry : value) 
			{
				duplicates.emplace_back(Duplicate(entry.path(),entry.file_size(), groupIdTemp));
			}
			groupIdTemp++;
		}
		return;
	}

	// Find Largerst files 
	void find_largest_files(std::vector<LargestFile>& largest_files, const fs::path& target, std::atomic<bool>&cancel_request, std::atomic<bool>& finished, const double& MaxSize = 10.0)
	{
		Guard guard(finished);
	
		if (!largest_files.empty()) largest_files.clear();
		std::error_code ec;

		for (fs::recursive_directory_iterator it(target, fs::directory_options::skip_permission_denied, ec), end; it != end && !ec; it.increment(ec))
		{
			if (cancel_request) 
			{
				return;
			}
			const auto& entry = *it;
			std::error_code entry_ec;
			if (entry.is_regular_file(entry_ec) && !entry_ec)
			{
				auto size = entry.file_size(entry_ec);
				if (!entry_ec && size >= MaxSize * 1024 * 1024)
				{
					largest_files.emplace_back(LargestFile(entry.path(), size));
					
				    LargestFile key = largest_files.back();
					int j = largest_files.size() - 1;

					
					while (j > 0 && key.size > largest_files[j - 1].size)
					{
						largest_files[j] = largest_files[j - 1];
						j--;
					}
					largest_files[j] = key;
				}
			}
		}
		return;
	}

};