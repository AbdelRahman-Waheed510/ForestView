#pragma once

#include <iostream>
#include <unordered_map>
#include <windows.h>
#include <filesystem>
#include <thread>
#include <mutex>
#include <ostream>
#include <fstream>
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

	bool RemoveHelper(Node* curr, const std::wstring& word, size_t depth);

	void CollectPaths(Node* node, std::vector<uint32_t>& results, const std::vector<FileInfo>& m_database) const;

public:

	Trie()
	{
		m_root = new Node();
	}

	~Trie() 
	{
		auto start = std::chrono::high_resolution_clock::now();

		//delete m_root;

		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> duration = end - start;
		double durationToShow = duration.count();

		if (durationToShow < 0.001) std::cout << "Time taken to shutdown the Trie: " << durationToShow * 1000 * 1000 << " Microseconds" << std::endl;
		else if (durationToShow < 1) std::cout << "Time taken to shutdown the Trie: " << durationToShow * 1000 << " Milliseconds" << std::endl;
		else std::cout << "Time taken to shutdown the Trie: " << std::endl << duration.count() << " Seconds" << std::endl;
	}

	static std::wstring ToLowerW(std::wstring s);

	bool Insert(const std::wstring& word, uint32_t path);

	bool Remove(const std::wstring& word);

	std::vector<uint32_t> SearchPrefix(const std::wstring& prefix, const std::vector<FileInfo>& m_database) const;
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
	std::atomic<bool> m_indexingDone{ false };
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
		auto start = std::chrono::high_resolution_clock::now();
		if (!m_indexingDone)
		{
			m_cancel_request.store(true, std::memory_order_relaxed);
		}
		if (m_indexingWorker.joinable())
		{
			m_indexingWorker.join();
		}
		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> duration = end - start;
		double durationToShow = duration.count();

		if (durationToShow < 0.001) std::cout << "Time taken to shutdown the Index: " << durationToShow * 1000 * 1000 << " Microseconds" << std::endl;
		else if (durationToShow < 1) std::cout << "Time taken to shutdown the Index: " << durationToShow * 1000 << " Milliseconds" << std::endl;
		else std::cout << "Time taken to shutdown the Index: " << std::endl << duration.count() << " Seconds" << std::endl;
	}

	void startIdexing();

	void paralaizedIndex();

	void indexPartition(const fs::directory_entry& Partition);

	void indexFolder(const fs::path& Folder);

	void index();

	void printIndexStats();

	void printExtensionSample(const std::wstring& ext, size_t count = 10);

	void printIndex();

	// Search
	void searchForExtention(std::vector<fs::path>& founds, const std::wstring& ext);

	void searchForExtention(std::vector<fs::path>& founds, const std::wstring& ext, const fs::path& root_path);

	void fastNameLookUp(std::vector<fs::path>& founds, const std::wstring& filename);

	void fastNameLookUp(std::vector<fs::path>& founds, const std::wstring& filename, const fs::path& root_path);

	// Utils
	constexpr bool shouldSkip(DWORD attrs);

	std::string to_utf8(const std::wstring& wstr);

	// Getters
	const bool isDone();

	const uint32_t getFilesCount();

	const uint32_t getDirectoriesCount();

	void removeFromDataBase(const fs::path& path_to_delete) 
	{
		for (FileInfo& file : m_dataBase) 
		{
			if (file.path == path_to_delete) 
			{
				file.path.clear();
				file.size = 0;
				break;
			}
		}
	}

	void renameFromDataBase(const fs::path& current_name, const fs::path& new_name) 
	{
		for (FileInfo& file : m_dataBase)
		{
			if (file.path == current_name)
			{
				file.path = new_name;
				break;
			}
		}
	}

	void addToDataBase(const fs::path& newItem) 
	{
		FileInfo f;
		f.path = newItem;
		f.size = fs::file_size(newItem);
		if (!m_indexingDone) 
		{
			std::lock_guard<std::mutex> lock(m_lock);

			m_dataBase.emplace_back(f);
			if (fs::is_directory(newItem))
			{
				if (m_dataBase.size() == m_dataBase.capacity())
					m_dataBase.reserve(m_dataBase.capacity() + 50);
				m_foldersIndex[f.path.filename().wstring()].emplace_back(Id);
				m_directoriesCount.fetch_add(1, std::memory_order_relaxed);
				trie.Insert(f.path.filename().wstring(), Id);
			}
			else
			{
				if (m_dataBase.size() == m_dataBase.capacity())
					m_dataBase.reserve(m_dataBase.capacity() + 50);
				m_filenameIndex[f.path.filename().wstring()].emplace_back(Id);
				m_extentionsIndex[f.path.extension().wstring()].emplace_back(Id);
				m_filesCount.fetch_add(1, std::memory_order_relaxed);
				trie.Insert(f.path.filename().wstring(), Id);
			}

			Id++;
		}
		else 
		{
			m_dataBase.emplace_back(f);
			if (fs::is_directory(newItem))
			{
				if (m_dataBase.size() == m_dataBase.capacity())
					m_dataBase.reserve(m_dataBase.capacity() + 50);
				m_foldersIndex[f.path.filename().wstring()].emplace_back(Id);
				m_directoriesCount.fetch_add(1, std::memory_order_relaxed);
				trie.Insert(f.path.filename().wstring(), Id);
			}
			else
			{
				if (m_dataBase.size() == m_dataBase.capacity())
					m_dataBase.reserve(m_dataBase.capacity() + 50);
				m_filenameIndex[f.path.filename().wstring()].emplace_back(Id);
				m_extentionsIndex[f.path.extension().wstring()].emplace_back(Id);
				m_filesCount.fetch_add(1, std::memory_order_relaxed);
				trie.Insert(f.path.filename().wstring(), Id);
			}

			Id++;
		}
	}
};

