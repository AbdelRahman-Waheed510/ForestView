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
