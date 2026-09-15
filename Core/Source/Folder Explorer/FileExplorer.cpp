#include "pchCore.h"
#include "FileExplorer.h"


std::string Utils::generate_new_folder_name(const fs::path& path, const std::string& name, const NewType& type)
{
    int count = 1;
    if (!fs::exists(path / name)) return name;

    if (type == FOLDER)
    {
        std::string res_name = name;
        while (fs::exists(path / fs::path(res_name)))
        {
            res_name = name + " (" + std::to_string(count) + ")";
            count++;
        }
        return res_name;
    }
    else if (type == REG_FILE)
    {
        fs::path file_path(name);
        std::string stem = file_path.stem().string();
        std::string extension = file_path.extension().string();

        std::string res_name = std::format("{} ({}){}", stem, count++, extension);
        while (fs::exists(path / fs::path(res_name)))
        {
            res_name = std::format("{} ({}){}", stem, count, extension);
            count++;
        }
        return res_name;
    }
    return name;
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