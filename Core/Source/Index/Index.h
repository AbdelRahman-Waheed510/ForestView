#pragma once

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

inline std::wstring pathKey(const fs::path& p)
{
	std::wstring s = p.lexically_normal().native();
	while (s.size() > 3 && (s.back() == L'\\' || s.back() == L'/'))
		s.pop_back();
	return s;
}

inline bool isSameOrDescendant(const std::wstring& pathToCheck, const std::wstring& root)
{
	if (root.empty() || pathToCheck.size() < root.size()) return false; // to see if the root is empty or the pathToCheck is smaller than the root 
	if (_wcsnicmp(pathToCheck.c_str(), root.c_str(), root.size()) != 0) return false; // compare the 2 paths till the end of the root 
	if (pathToCheck.size() == root.size()) return true; // compare the sizes after the last comparison if they equal so return true if not continue to the rest of the func
	return root.back() == L'\\' || pathToCheck[root.size()] == L'\\'; // return if the pathToCheck has \\ after the last part of the root
}

struct FileInfo 
{
	fs::path path;
	uintmax_t size;
	bool isDeleted = false;
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

	bool RemoveHelper(Node* curr, const std::wstring& word, size_t depth, uint32_t path);
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

	bool Remove(const std::wstring& word, uint32_t path);

	std::vector<uint32_t> SearchPrefix(const std::wstring& prefix, const std::vector<FileInfo>& m_database) const;
};

class Index
{
private:

	uint32_t Id = 0;

	Trie trie;
	std::vector<FileInfo> m_dataBase;
	std::unordered_map<std::wstring, std::vector<uint32_t>> m_extentionsIndex;
	std::unordered_map<fs::path, uint32_t> m_pathsIndex;
	//std::unordered_map<std::wstring, std::vector<uint32_t>> m_filenameIndex;
	//std::unordered_map<std::wstring, std::vector<uint32_t>> m_foldersIndex;
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
		std::lock_guard<std::mutex> lock(m_lock);

		const std::wstring key = pathKey(path_to_delete);
		if (key.empty()) return;

		size_t hits = 0;
		for (FileInfo& f : m_dataBase)
		{
			if (f.isDeleted || !isSameOrDescendant(f.path.native(), key)) continue;

			f.isDeleted = true;
			m_pathsIndex.erase(f.path);
			++hits;
		}

		if (hits == 0)
			std::wcerr << L"[Index] removeFromDataBase matched nothing for: " << key << L"\n";
	}

	void renameFromDataBase(const fs::path& current_name, const fs::path& new_name)
	{
		std::lock_guard<std::mutex> lock(m_lock);

		const std::wstring oldKey = pathKey(current_name);
		const std::wstring newKey = pathKey(new_name);
		if (oldKey.empty() || newKey.empty()) return;

		for (uint32_t i = 0; i < m_dataBase.size(); ++i)
		{
			FileInfo& f = m_dataBase[i];
			if (f.isDeleted || !isSameOrDescendant(f.path.native(), oldKey)) continue;

			const bool isRenamedItem = (f.path.native().size() == oldKey.size());
			const std::wstring oldExt = f.path.extension().wstring();

			const std::wstring oldFilename = f.path.filename().wstring();

			m_pathsIndex.erase(f.path);
			f.path = fs::path(newKey + f.path.native().substr(oldKey.size()));
			m_pathsIndex[f.path] = i;

			if (isRenamedItem)   // descendants keep their own filename/extension; only the renamed item changes keys
			{
				trie.Remove(oldFilename, i);
				trie.Insert(f.path.filename().wstring(), i);

				bool wasFile = false;
				if (auto it = m_extentionsIndex.find(oldExt); it != m_extentionsIndex.end())
					wasFile = std::find(it->second.begin(), it->second.end(), i) != it->second.end();
				if (wasFile)
					m_extentionsIndex[f.path.extension().wstring()].emplace_back(i);
			}
		}

	}

	void addToDataBase(const fs::path& newItem) 
	{
		FileInfo f;
		f.path = newItem.lexically_normal();
		std::error_code ec;
		f.size = fs::file_size(newItem, ec);
		if (!m_indexingDone) 
		{
			std::lock_guard<std::mutex> lock(m_lock);

			m_dataBase.emplace_back(f);
			if (fs::is_directory(newItem))
			{
				if (m_dataBase.size() == m_dataBase.capacity())
					m_dataBase.reserve(m_dataBase.capacity() + 50);
				//m_foldersIndex[f.path.filename().wstring()].emplace_back(Id);
				m_directoriesCount.fetch_add(1, std::memory_order_relaxed);
				trie.Insert(f.path.filename().wstring(), Id);
			}
			else
			{
				if (m_dataBase.size() == m_dataBase.capacity())
					m_dataBase.reserve(m_dataBase.capacity() + 50);
			//	m_filenameIndex[f.path.filename().wstring()].emplace_back(Id);
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
				//m_foldersIndex[f.path.filename().wstring()].emplace_back(Id);
				m_directoriesCount.fetch_add(1, std::memory_order_relaxed);
				trie.Insert(f.path.filename().wstring(), Id);
			}
			else
			{
				if (m_dataBase.size() == m_dataBase.capacity())
					m_dataBase.reserve(m_dataBase.capacity() + 50);
				//m_filenameIndex[f.path.filename().wstring()].emplace_back(Id);
				m_extentionsIndex[f.path.extension().wstring()].emplace_back(Id);
				m_filesCount.fetch_add(1, std::memory_order_relaxed);
				trie.Insert(f.path.filename().wstring(), Id);
			}

			Id++;
		}
	}

	void addRecuirselyToDataBase(const fs::path& newPath) 
	{
		addToDataBase(newPath);

		if (fs::is_directory(newPath))
		{
			std::error_code ec;
			for (const auto& entry : fs::recursive_directory_iterator(newPath, ec))
			{
				if (ec)
				{
					continue;
				}
				addToDataBase(entry.path());
			}
		}
	}
};

