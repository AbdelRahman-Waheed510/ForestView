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
#include <array>

namespace fs = std::filesystem;

constexpr static int s_bufferSize = 10000;

inline fs::path ToLongPath(const fs::path& p)
{
	std::wstring s = p.wstring();
	if (s.rfind(LR"(\\?\)", 0) == 0)
		return p;

	std::error_code ec;
	fs::path abs = fs::absolute(p, ec);
	if (ec) return p;

	abs = abs.lexically_normal();
	return fs::path(LR"(\\?\)" + abs.wstring());
}

struct FileInfo 
{
	fs::path path;
	uintmax_t size;
};

class Trie
{
private:
	struct Node
	{
		wchar_t val;
		std::unordered_map<wchar_t, Node*> children;
		bool isLeaf;
		std::vector<uint32_t> Paths;

		Node() 
		{
			isLeaf = false;
		}

		Node(const wchar_t& value)
		{
			val = value;
			isLeaf = false;
		}

		~Node()
		{
			for (const auto& [key, value] : children)
			{
				delete value;
			}
		}
	};

	Node* m_root;

private:

	bool RemoveHelper(Node* curr, const std::wstring& word, size_t depth)
	{
		if (curr == nullptr) return false;

		if (depth == word.length())
		{
			if (!curr->isLeaf) return false;
			curr->isLeaf = false;

			return curr->children.empty();
		}

		wchar_t ch = word[depth];
		auto it = curr->children.find(ch);
		if (it == curr->children.end()) return false;

		bool shouldDeleteChild = RemoveHelper(it->second, word, depth + 1);

		if (shouldDeleteChild)
		{
			Node* childToDelete = it->second;
			curr->children.erase(it);
			delete childToDelete; 

			return !curr->isLeaf && curr->children.empty();
		}

		return false;
	}

	void CollectPaths(Node* node, std::vector<uint32_t>& results) const
	{
		if (node == nullptr) return;

		// If this node represents a complete filename, append its stored path IDs
		if (node->isLeaf)
		{
			results.insert(results.end(), node->Paths.begin(), node->Paths.end());
		}

		// Traverse all child branches
		for (const auto& [ch, childNode] : node->children)
		{
			CollectPaths(childNode, results);
		}
	}

public:

	Trie()
	{
		m_root = new Node();
	}

	~Trie() 
	{
		delete m_root;
	}

	static std::wstring ToLowerW(std::wstring s)
	{
		if (!s.empty())
			CharLowerBuffW(s.data(), static_cast<DWORD>(s.size()));
		return s;
	}

	bool Insert(const std::wstring& word, uint32_t path)
	{
		std::wstring key = ToLowerW(word);
		bool createdNewNode = false;
		Node* curr = m_root;
		for (const wchar_t& x : key)
		{
			if (curr->children.find(x) == curr->children.end())
			{
				curr->children[x] = new Node(x);
				createdNewNode = true;
			}
			curr = curr->children[x];
		}
		curr->Paths.emplace_back(path);
		curr->isLeaf = true;
		return createdNewNode;
	}

	bool Remove(const std::wstring& word)
	{
		if (m_root == nullptr || word.empty()) return false;
		return RemoveHelper(m_root, word, 0);
	}

	std::vector<uint32_t> SearchPrefix(const std::wstring& prefix) const
	{
		std::wstring key = ToLowerW(prefix);
		Node* curr = m_root;
		for (const wchar_t& x : key)
		{
			auto it = curr->children.find(x);
			if (it == curr->children.end())
			{
				return {};
			}
			curr = it->second;
		}

		std::vector<uint32_t> result;
		CollectPaths(curr, result);
		return result;
	}
};

class Index 
{
private:

	uint32_t Id = 0;

	Trie trie;
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

	std::atomic<uint64_t> m_lockWaitNs{ 0 };
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

		std::cout << "Time Taken by look [m_lockWaitNs.load() / 1000000]" << m_lockWaitNs.load() / 1000000 << std::endl;

		m_indexingDone.store(true);
	}

	void indexPartition(const fs::directory_entry& Partition)
	{
		auto start = std::chrono::steady_clock::now();
		std::vector<std::future<void>> FoldersFuture;
		// ###########################################################################################################
		//                                              Windows APIs Way
		// ###########################################################################################################
		WIN32_FIND_DATAW Entry;

		fs::path par = ToLongPath(Partition.path()) / L"*";

		HANDLE searchToken = FindFirstFileExW(par.c_str(), FindExInfoBasic, &Entry, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);

		if (searchToken == INVALID_HANDLE_VALUE) return;
		
		do
		{
			if (wcscmp(Entry.cFileName, L".") == 0 || wcscmp(Entry.cFileName, L"..") == 0)
			{
				continue;
			}

			if (m_cancel_request.load(std::memory_order_relaxed))
			{
				auto end = std::chrono::steady_clock::now();
				std::wcout << Partition.path().wstring() << L": "
					<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
					<< L"ms\n";
				break;
			}
			const fs::path path = Partition.path() / Entry.cFileName;

			if (Entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (shouldSkip(Entry.dwFileAttributes)) continue;
				if (Entry.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) continue;

				FoldersFuture.emplace_back(std::async(std::launch::async, [this, path]() {
					indexFolder(path);
					}));
			}
			else if (!(Entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(Entry.dwFileAttributes & FILE_ATTRIBUTE_DEVICE))
			{
				FileInfo info;
				info.path = path;
				info.size = (static_cast<uint64_t>(Entry.nFileSizeHigh) << 32) | Entry.nFileSizeLow;

				auto t0 = std::chrono::steady_clock::now();
				std::lock_guard<std::mutex> lock(m_lock);
				auto t1 = std::chrono::steady_clock::now();
				m_lockWaitNs.fetch_add(
					std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count(),
					std::memory_order_relaxed);

				if (m_dataBase.size() == m_dataBase.capacity())
					m_dataBase.reserve(m_dataBase.capacity() + 100000);
				m_dataBase.emplace_back(info);
				m_extentionsIndex[path.extension().wstring()].emplace_back(Id);
				m_filenameIndex[path.filename().wstring()].emplace_back(Id);
				trie.Insert(path.filename().wstring(), Id);
				m_filesCount.fetch_add(1, std::memory_order_relaxed);
				Id++;
			}
		}
		while (FindNextFileW(searchToken, &Entry));
		
		FindClose(searchToken);

		// ###########################################################################################################

		for (auto& f : FoldersFuture) f.get();

		auto end = std::chrono::steady_clock::now();
		std::wcout << Partition.path().wstring() << L": "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			<< L"ms\n";
	}

	void indexFolder(const fs::path& Folder)
	{
		auto start = std::chrono::steady_clock::now();

		// 1. Index the current root folder entry
		{
			FileInfo info;
			info.path = Folder;
			auto t0 = std::chrono::steady_clock::now();
			std::lock_guard<std::mutex> lock(m_lock);
			auto t1 = std::chrono::steady_clock::now();
			m_lockWaitNs.fetch_add(
				std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count(),
				std::memory_order_relaxed);

			if (m_dataBase.size() == m_dataBase.capacity())
				m_dataBase.reserve(m_dataBase.capacity() + 100000);

			m_dataBase.emplace_back(info);
			m_foldersIndex[Folder.filename().wstring()].emplace_back(Id);
			trie.Insert(Folder.filename().wstring(), Id);
			m_directoriesCount.fetch_add(1, std::memory_order_relaxed);
			Id++;
		}

		std::array<FileInfo, s_bufferSize> filesBuffer;
		std::array<FileInfo, s_bufferSize> foldersBuffer;
		size_t localFilesCount = 0;
		size_t localFoldersCount = 0;

		auto flush = [&]() {
			auto t0 = std::chrono::steady_clock::now();
			std::lock_guard<std::mutex> lock(m_lock);
			auto t1 = std::chrono::steady_clock::now();
			m_lockWaitNs.fetch_add(
				std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count(),
				std::memory_order_relaxed);

			for (size_t i = 0; i < localFilesCount; ++i)
			{
				auto& f = filesBuffer[i];
				if (m_dataBase.size() == m_dataBase.capacity())
					m_dataBase.reserve(m_dataBase.capacity() + 100000);

				m_dataBase.emplace_back(f);
				m_extentionsIndex[f.path.extension().wstring()].emplace_back(Id);
				m_filenameIndex[f.path.filename().wstring()].emplace_back(Id);
				m_filesCount.fetch_add(1, std::memory_order_relaxed);
				trie.Insert(f.path.filename().wstring(), Id);
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
				trie.Insert(d.path.filename().wstring(), Id);
				Id++;
			}

			localFilesCount = 0;
			localFoldersCount = 0;
			};

		// 2. Recursive Win32 directory scanner
		auto scanDirectoryRecursive = [&](auto& self, const fs::path& currentDir) -> void
			{
				if (m_cancel_request.load(std::memory_order_relaxed)) return;

				fs::path searchPath = ToLongPath(currentDir) / L"*";
				WIN32_FIND_DATAW entry;
				HANDLE searchToken = FindFirstFileExW(searchPath.c_str(), FindExInfoBasic, &entry, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);

				if (searchToken == INVALID_HANDLE_VALUE) return;

				do
				{
					if (m_cancel_request.load(std::memory_order_relaxed)) break;

					if (wcscmp(entry.cFileName, L".") == 0 || wcscmp(entry.cFileName, L"..") == 0)
					{
						continue;
					}

					fs::path childPath = currentDir / entry.cFileName;
					const DWORD attr = entry.dwFileAttributes;

					if (attr & FILE_ATTRIBUTE_DIRECTORY)
					{
						if (shouldSkip(attr)) continue;

						FileInfo info;
						info.path = childPath;
						foldersBuffer[localFoldersCount++] = std::move(info);

						if (localFoldersCount >= s_bufferSize || localFilesCount >= s_bufferSize)
						{
							flush();
						}

						// Recurse into directory if not a reparse point (symlink/junction)
						if (!(attr & FILE_ATTRIBUTE_REPARSE_POINT))
						{
							self(self, childPath);
						}
					}

					else if (!(attr & FILE_ATTRIBUTE_DEVICE))
					{
						FileInfo info;
						info.path = childPath;
						info.size = (static_cast<uint64_t>(entry.nFileSizeHigh) << 32) | entry.nFileSizeLow;

						filesBuffer[localFilesCount++] = std::move(info);

						if (localFoldersCount >= s_bufferSize || localFilesCount >= s_bufferSize)
						{
							flush();
						}
					}
				} while (FindNextFileW(searchToken, &entry));

				FindClose(searchToken);
			};

		// Run the Win32 scan start from target folder
		scanDirectoryRecursive(scanDirectoryRecursive, Folder);

		flush();

		auto end = std::chrono::steady_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		if (ms > 100)
			std::wcout << Folder.wstring() << L": " << ms << L"ms\n";
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

						auto t0 = std::chrono::steady_clock::now();
						std::lock_guard<std::mutex> m_UpadtingTheIndexGuard(m_lock);
						auto t1 = std::chrono::steady_clock::now();
						m_lockWaitNs.fetch_add(
							std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count(),
							std::memory_order_relaxed);

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


						auto t0 = std::chrono::steady_clock::now();
						std::lock_guard<std::mutex> m_UpadtingTheIndexGuard(m_lock);
						auto t1 = std::chrono::steady_clock::now();
						m_lockWaitNs.fetch_add(
							std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count(),
							std::memory_order_relaxed);

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

	void searchForExtention(std::vector<fs::path>& founds, const std::wstring& ext) 
	{
		if (!founds.empty()) founds.clear();
		if (!ext.empty()) 
		{
			std::wstring extentionToSearchFor = ext;
			if (ext[0] != L'.')
			{
				extentionToSearchFor = L"." + extentionToSearchFor;
			}
			if (m_extentionsIndex.contains(extentionToSearchFor))
			{
				founds.reserve(m_extentionsIndex[extentionToSearchFor].size());

				for (const int& idx : m_extentionsIndex[extentionToSearchFor])
				{
					founds.emplace_back(m_dataBase[idx].path);
				}
			}
		}
	}

	void printIndexStats() 
	{
		std::lock_guard<std::mutex> guard(m_lock);
		std::cout << "Files Scanned: " << m_filesCount.load()
			<< ", Directories Scanned: " << m_directoriesCount.load() << "\n";

		std::cout << "Unique extensions: " << m_extentionsIndex.size() << "\n";
		std::cout << "Unique filenames: " << m_filenameIndex.size() << "\n";
		std::cout << "Unique folder names: " << m_foldersIndex.size() << "\n";

		// top 10 most common extensions, e.g., useful sanity check
		std::vector<std::pair<std::wstring, size_t>> extCounts;
		for (auto& [key, value] : m_extentionsIndex)
			extCounts.emplace_back(key, value.size());
		std::sort(extCounts.begin(), extCounts.end(),
			[](auto& a, auto& b) { return a.second > b.second; });
		for (size_t i = 0; i < std::min<size_t>(10, extCounts.size()); ++i)
			std::wcout << extCounts[i].first << L": " << extCounts[i].second << L"\n";
	}

	void printExtensionSample(const std::wstring& ext, size_t count = 10)
	{
		std::wcout << L"Extention sample for: [" << ext << L"]\n";
		std::lock_guard<std::mutex> guard(m_lock);
		auto it = m_extentionsIndex.find(ext);
		if (it == m_extentionsIndex.end()) { std::cout << "not found\n"; return; }

		size_t shown = 0;
		for (uint32_t id : it->second)
		{
			if (shown++ >= count) break;
			std::wcout << m_dataBase[id].path.wstring() << L"\n";
		}
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


	// Utils
	constexpr bool shouldSkip(DWORD attrs) 
	{
		if (attrs == INVALID_FILE_ATTRIBUTES)
			return true;

		return (attrs & FILE_ATTRIBUTE_HIDDEN) || (attrs & FILE_ATTRIBUTE_SYSTEM);
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

	// Getters
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

	void fastNameLookUp(std::vector<fs::path>& founds, const std::wstring& filename) 
	{
		if (!founds.empty()) founds.clear();
		std::vector<uint32_t> indexes = trie.SearchPrefix(filename);

		for (const auto& idx : indexes)
		{
			founds.emplace_back(m_dataBase[idx].path);
		}
	}
};

