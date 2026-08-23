#pragma once

#include <iostream>
#include <unordered_map>
#include <windows.h>
#include <filesystem>
#include <thread>
#include <mutex>
#include <ostream>
#include <istream>
#include <future>

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
	std::unordered_map<std::wstring, std::vector<uint32_t>> m_extentionsIndex;
	std::unordered_map<std::wstring, std::vector<uint32_t>> m_filenameIndex;
	std::unordered_map<std::wstring, std::vector<uint32_t>> m_foldersIndex;
	std::atomic<bool> m_indexingDone{false};
	std::atomic<uint32_t> m_filesCount{ 0 };
	std::atomic<uint32_t> m_directoriesCount = { 0 };
	std::mutex m_lock;
	std::atomic<bool> m_cancel_request{ false };

	std::thread m_indexingWorker;
public:
	Index() = default;

	~Index() 
	{
		if (!m_indexingDone) 
		{
			m_cancel_request.store(true, std::memory_order_relaxed);
		}
		if (m_indexingWorker.joinable()) 
		{
			m_indexingWorker.join();
		}
	}

	void startIdexing()
	{
		m_indexingWorker = std::thread([this]() {paralaizedIndex();});
	}

	void paralaizedIndex()
	{
		auto start = std::chrono::steady_clock::now();

		wchar_t buffer[256];
		DWORD buffer_len = GetLogicalDriveStringsW(ARRAYSIZE(buffer), buffer);

		std::vector<std::future<void>> futures;

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

				futures.emplace_back(std::async(std::launch::async, [this, partition]() 
					{
						indexPartition(partition);
					}));
				
				drive += wcslen(drive) + 1;
			}
		}

		for (auto& f : futures) f.get();

		auto end = std::chrono::steady_clock::now();
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		std::cout << "Indexing took: " << seconds << "s\n";
		m_indexingDone.store(true);
	}

	void indexPartition(const fs::directory_entry& partition) 
	{
		auto start = std::chrono::steady_clock::now();
		constexpr int bufferSize = 1000;
		std::array<FileInfo, bufferSize> filesBuffer;
		std::array<FileInfo, bufferSize> foldersBuffer;
		size_t localFilesCount = 0;
		size_t localFoldersCount = 0;

		auto flush = [&]() {
			std::lock_guard<std::mutex> lock(m_lock);

			for (size_t i = 0; i < localFilesCount; ++i)
			{
				auto& f = filesBuffer[i];
				if (m_dataBase.size() == m_dataBase.capacity())
					m_dataBase.reserve(m_dataBase.capacity() + 100000);

				m_dataBase.emplace_back(f);
				m_extentionsIndex[f.path.extension().wstring()].emplace_back(Id);
				m_filenameIndex[f.path.filename().wstring()].emplace_back(Id);
				m_filesCount.fetch_add(1, std::memory_order_relaxed);
				Id++;
			}

			for (size_t i = 0; i < localFoldersCount; ++i)
			{
				auto& d = foldersBuffer[i];
				if (m_dataBase.size() == m_dataBase.capacity())
					m_dataBase.reserve(m_dataBase.capacity() + 100000);

				m_dataBase.emplace_back(d);
				m_foldersIndex[d.path.filename().wstring()].emplace_back(Id);
				m_directoriesCount.fetch_add(1, std::memory_order_relaxed);
				Id++;
			}

			localFilesCount = 0;
			localFoldersCount = 0;
			};

		std::error_code ec1;
		for (fs::recursive_directory_iterator it(partition.path(), fs::directory_options::skip_permission_denied, ec1), end; it != end && !ec1; it.increment(ec1))
		{
			if (m_cancel_request.load(std::memory_order_relaxed)) 
			{
				flush();
				auto end = std::chrono::steady_clock::now();
				std::wcout << partition.path().wstring() << L": "
					<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
					<< L"ms\n";
				return;
			}
			const fs::directory_entry& entry = *it;
			const fs::path path = entry.path();
	
			std::error_code entryEc;
			std::error_code dirEc;

			if (entry.is_regular_file(entryEc) && !entryEc)
			{
				FileInfo info;
				info.path = path;
				std::error_code sizeEc;
				info.size = entry.file_size(sizeEc);
				if (sizeEc) continue;

				filesBuffer[localFilesCount++] = std::move(info);
			}
			else if (entry.is_directory(dirEc) && !dirEc)
			{
				if (shouldSkip(path))
				{
					it.disable_recursion_pending();
					continue;
				}
				FileInfo info;
				info.path = path;
				foldersBuffer[localFoldersCount++] = std::move(info);
			}

			if (localFoldersCount >= bufferSize || localFilesCount >= bufferSize) 
			{
				flush();
			}
		}
		flush();

		auto end = std::chrono::steady_clock::now();
		std::wcout << partition.path().wstring() << L": "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			<< L"ms\n";

		if (ec1)
		{
			std::cerr << "Scanning error in "
				<< partition.path()
				<< ": "
				<< ec1.message()
				<< '\n';
		}
	}

	const bool shouldSkip(const fs::path& path) 
	{
		DWORD attrs = GetFileAttributesW(path.c_str());
		if (attrs == INVALID_FILE_ATTRIBUTES)
			return true;

		return (attrs & FILE_ATTRIBUTE_HIDDEN) || (attrs & FILE_ATTRIBUTE_SYSTEM);
	}

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
					const fs::path path = entry.path();
					std::error_code entryEc;
					std::error_code dirEc;

					if (m_dataBase.size() == m_dataBase.capacity())
					{
						m_dataBase.reserve(m_dataBase.capacity() + 100000);
					}
					if (entry.is_regular_file(entryEc) && !entryEc)
					{
						FileInfo info;
						info.path = path;

						std::error_code sizeEc;
						info.size = entry.file_size(sizeEc);
						if (sizeEc)
						{
							continue;
						}

						std::lock_guard<std::mutex> m_UpadtingTheIndexGuard(m_lock);

						m_dataBase.emplace_back(info);
						m_filesCount++;

						m_extentionsIndex[path.extension().wstring()].emplace_back(Id);
						m_filenameIndex[path.filename().wstring()].emplace_back(Id);
						Id++;
					}
					else if (entry.is_directory(dirEc) && !dirEc)
					{
						FileInfo info;
						info.path = path;

						std::lock_guard<std::mutex> m_UpadtingTheIndexGuard(m_lock);

						m_dataBase.emplace_back(info);
						m_directoriesCount++;

						m_foldersIndex[path.filename().wstring()].emplace_back(Id);
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
		}
		m_indexingDone = true;
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
			for (const auto& [key, value] : m_foldersIndex)
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
		return m_indexingDone.load();
	}
	
	const uint32_t getFilesCount() 
	{
		return m_filesCount.load();
	}

	const uint32_t getDirectoriesCount() 
	{
		return m_directoriesCount.load();
	}
};