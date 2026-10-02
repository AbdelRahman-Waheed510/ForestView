#include "pchCore.h"

#include "FileExplorer.h"


inline fs::path Utils::utf8_to_path(const std::string& s)
{
	return fs::path(std::u8string(s.begin(), s.end()));
}

inline std::string Utils::path_to_utf8(const fs::path& p)
{
	auto s = p.u8string();
	return std::string(s.begin(), s.end());
}

std::string Utils::generate_new_folder_name(const fs::path& dir, const std::string& name, const NewType& type)
{
	std::error_code ec;
	if (!fs::exists(dir / utf8_to_path(name), ec)) return name;

	std::string stem = name;
	std::string ext;

	if (type == NewType::REG_FILE)
	{
		const fs::path namePath = utf8_to_path(name);
		stem = path_to_utf8(namePath.stem());
		ext = path_to_utf8(namePath.extension());
	}

	for (int count = 1;; ++count)
	{
		std::string candidate = std::format("{} ({}){}", stem, count, ext);
		if (!fs::exists(dir / utf8_to_path(candidate), ec))
			return candidate;
	}
}
std::string Utils::to_lower(std::string str)
{
    {
        for (char& x : str)
        {
            x = std::tolower(x);
        }
        return str;
    }
}

std::string Utils::reform_string(const std::string& str, const int& count)
{
    if (count > str.size()) return str;
    if (count < 0) return str;

    std::string res = str.substr(0, count);

    for (size_t i = 0; i < 3; ++i) 
    {
        res += '.';
    }
    return res;
}

const std::string Utils::file_type_to_string(const fs::file_type& type)
{
	switch (type)
	{
	case fs::file_type::directory:
	{
		return "File Folder";
		break;
	}
	case fs::file_type::regular:
	{
		return "Regular File";
		break;
	}
	case fs::file_type::symlink:
	{
		return "Symbolic Link";
		break;
	}
	case fs::file_type::block:
	{
		return "Block";
		break;
	}
	case fs::file_type::fifo:
	{
		return "Fifo";
		break;
	}
	case fs::file_type::character:
	{
		return "Character";
		break;
	}
	case fs::file_type::not_found:
	{
		return "Not Found";
		break;
	}
	case fs::file_type::unknown:
	{
		return "Unknown File";
		break;
	}
	default:
		return "Default";
		break;
	}
}

void Explorer::sort_entries()
{
	switch (m_activeSortType)
	{
	case SortType::NAME:
	{
		if (m_activeSortOrder == SortOrder::ASCENDING) 
		{
			std::sort(entries.begin(), entries.end(),
				[](const Entry& a, const Entry& b)
				{
					const bool aIsDir = a.entry.is_directory();
					const bool bIsDir = b.entry.is_directory();

					if (aIsDir != bIsDir)
						return aIsDir;

					const std::wstring aName = a.entry.path().filename().wstring();
					const std::wstring bName = b.entry.path().filename().wstring();

					return StrCmpLogicalW(aName.c_str(), bName.c_str()) < 0;
				});
		}
		else
		{
			std::sort(entries.begin(), entries.end(),
				[](const Entry& a, const Entry& b)
				{
					const bool aIsFile = a.entry.is_regular_file();
					const bool bIsFile = b.entry.is_regular_file();

					if (aIsFile != bIsFile)
						return aIsFile;

					const std::wstring aName = a.entry.path().filename().wstring();
					const std::wstring bName = b.entry.path().filename().wstring();

					return StrCmpLogicalW(aName.c_str(), bName.c_str()) > 0;
				});
		} 
		break;
	}
	case SortType::SIZE:
	{
		if (m_activeSortOrder == SortOrder::ASCENDING)
		{
			std::sort(entries.begin(), entries.end(),
				[](const Entry& a, const Entry& b)
				{
					const bool aIsDir = a.entry.is_directory();
					const bool bIsDir = b.entry.is_directory();

					if (aIsDir != bIsDir)
						return aIsDir;

					if (aIsDir && bIsDir)
					{
						const std::wstring aName = a.entry.path().filename().wstring();
						const std::wstring bName = b.entry.path().filename().wstring();

						return StrCmpLogicalW(aName.c_str(), bName.c_str()) < 0;
					}

					const auto aSize = a.entry.file_size();
					const auto bSize = b.entry.file_size();

					if (aSize != bSize)
						return aSize < bSize;

					const std::wstring aName = a.entry.path().filename().wstring();
					const std::wstring bName = b.entry.path().filename().wstring();

					return StrCmpLogicalW(aName.c_str(), bName.c_str()) < 0;
				});
		}
		else 
		{
			std::sort(entries.begin(), entries.end(),
				[](const Entry& a, const Entry& b)
				{
					const bool aIsFile = a.entry.is_regular_file();
					const bool bIsFile = b.entry.is_regular_file();

					if (aIsFile != bIsFile)
						return aIsFile;

					if (a.entry.is_directory() && b.entry.is_directory())
					{
						const std::wstring aName = a.entry.path().filename().wstring();
						const std::wstring bName = b.entry.path().filename().wstring();

						return StrCmpLogicalW(aName.c_str(), bName.c_str()) < 0;
					}

					const auto aSize = a.entry.file_size();
					const auto bSize = b.entry.file_size();
					if (aSize != bSize)
						return aSize > bSize;

					const std::wstring aName = a.entry.path().filename().wstring();
					const std::wstring bName = b.entry.path().filename().wstring();

					return StrCmpLogicalW(aName.c_str(), bName.c_str()) < 0;
				});
		}
		break;
	}
	case SortType::LAST_MODIFIED:
	{
		if (m_activeSortOrder == SortOrder::ASCENDING)
		{
			std::sort(entries.begin(), entries.end(),
				[](const Entry& a, const Entry& b)
				{
					const bool aIsDir = a.entry.is_directory();
					const bool bIsDir = b.entry.is_directory();

					if (aIsDir != bIsDir)
						return aIsDir;

					const auto& aTime = a.entry.last_write_time();
					const auto& bTime = b.entry.last_write_time();

					if (aTime != bTime) return aTime < bTime;

					const std::wstring aName = a.entry.path().filename().wstring();
					const std::wstring bName = b.entry.path().filename().wstring();

					return StrCmpLogicalW(aName.c_str(), bName.c_str()) < 0;
				});
		}
		else 
		{
			std::sort(entries.begin(), entries.end(),
				[](const Entry& a, const Entry& b)
				{
					const bool aIsFile = a.entry.is_regular_file();
					const bool bIsFile = b.entry.is_regular_file();

					if (aIsFile != bIsFile)
						return aIsFile;

					const auto& aTime = a.entry.last_write_time();
					const auto& bTime = b.entry.last_write_time();

					if (aTime != bTime) return aTime > bTime;

					const std::wstring aName = a.entry.path().filename().wstring();
					const std::wstring bName = b.entry.path().filename().wstring();

					return StrCmpLogicalW(aName.c_str(), bName.c_str()) < 0;
				});
		}
		break;
	}
	default:
		break;
	}

	return;
}