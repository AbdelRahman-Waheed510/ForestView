#pragma once

#include <iostream>
#include <unordered_map>
#include <windows.h>
#include <filesystem>
#include <thread>
#include <mutex>
#include <ostream>
#include <istream>

namespace fs = std::filesystem;

struct FileInfo 
{
	fs::path path;
	uintmax_t size;
};

class Index 
{
private:

	uint32_t Id = 0;

	std::vector<FileInfo> m_dataBase;
	std::unordered_map<std::string, std::vector<uint32_t>> m_extentionsIndex;
	std::unordered_map<std::wstring, std::vector<uint32_t>> m_filenameIndex;
	bool m_indexingDone = false;
	uint32_t m_filesCount = 0;
	uint32_t m_directoriesCount = 0;
	std::mutex m_lock;
public:
	
	void index() 
	{
		wchar_t buffer[256];
		DWORD buffer_len = GetLogicalDriveStringsW(ARRAYSIZE(buffer), buffer);
		if (buffer_len > 0 && buffer_len < ARRAYSIZE(buffer)) {
			wchar_t* drive = buffer;
			while (*drive) {
				std::error_code ec;
				fs::directory_entry partition(fs::path(drive), ec);
				if (ec)
				{
					std::cerr << "Error while loading partitions" << std::endl;
					drive += wcslen(drive) + 1;
					continue;
				}
				
				std::error_code ec1;
				for (fs::recursive_directory_iterator it(partition.path(), fs::directory_options::skip_permission_denied, ec1), end; it != end && !ec1; it.increment(ec1))
				{
					const fs::directory_entry& entry = *it;
					std::error_code entryEc;
					std::error_code dirEc;
					if (entry.is_regular_file(entryEc) && !entryEc)
					{
						FileInfo info;
						info.path = entry.path();

						std::error_code sizeEc;
						info.size = entry.file_size(sizeEc);
						if (sizeEc)
						{
							continue;
						}

						std::lock_guard<std::mutex> m_UpadtingTheIndexGuard(m_lock);

						m_dataBase.emplace_back(info);
						m_filesCount++;

						m_extentionsIndex[entry.path().extension().string()].emplace_back(Id);
						m_filenameIndex[entry.path().filename().wstring()].emplace_back(Id);
						Id++;
					}
					else if (entry.is_directory(dirEc) && !dirEc)
					{
						FileInfo info;
						info.path = entry.path();

						std::lock_guard<std::mutex> m_UpadtingTheIndexGuard(m_lock);

						m_dataBase.emplace_back(info);
						m_directoriesCount++;

						m_filenameIndex[entry.path().filename().wstring()].emplace_back(Id);
						Id++;
					}
				}
				if (ec1)
				{
					std::cerr << "Scanning error in "
						<< partition.path()
						<< ": "
						<< ec1.message()
						<< '\n';
				}
				drive += wcslen(drive) + 1;
			}
			m_indexingDone = true;
		}
	}

	std::string to_utf8(const std::wstring& wstr) {
		if (wstr.empty()) return {};

		// Set up the conversion facet for UTF-8
		std::locale loc("en_US.UTF-8"); // Use "" for system default or explicit UTF-8
		auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);

		std::mbstate_t state = std::mbstate_t();
		std::vector<char> buffer(wstr.size() * facet.max_length());

		const wchar_t* from_next;
		char* to_next;

		facet.out(state,
			wstr.data(), wstr.data() + wstr.size(), from_next,
			buffer.data(), buffer.data() + buffer.size(), to_next);

		return std::string(buffer.data(), to_next);
	}

	void printIndex() 
	{
		std::lock_guard<std::mutex> guard(m_lock);

		std::ofstream write("IndexLog.txt", std::ios::app);
		if (write.is_open()) 
		{
			for (const auto& info : m_dataBase) 
			{
				write << to_utf8(info.path) << " Size: " << info.size << "\n";
			}
			for (const auto& [key, value] : m_extentionsIndex) 
			{
				write << "\n" << key << "\n==============================================\n";
			
				bool first = true;
				for (const auto& x : value)
				{
					if (!first) write << ", ";
					write << x;
					first = false;
				}
				write << "\n";
			}
			for (const auto& [key, value] : m_filenameIndex)
			{
				write << "\n" << to_utf8(key) << "\n==============================================\n";

				bool first = true;
				for (const auto& x : value)
				{
					if (!first) write << ", ";
					write << x;
					first = false;
				}
				write << "\n";
			}
		
		}
	}

	const bool isDone() 
	{
		return m_indexingDone;
	}
	
	const uint32_t getFilesCount() 
	{
		std::lock_guard<std::mutex> readDirectoriesCountGuard(m_lock);
		return m_filesCount;
	}

	const uint32_t getDirectoriesCount() 
	{
		std::lock_guard<std::mutex> readDirectoriesCountGuard(m_lock);
		return m_directoriesCount;
	}
};