#pragma once

#include <vector>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <stack>

namespace fs = std::filesystem;

class Favorites 
{
private:
	std::vector<fs::directory_entry> favorites;
public:
	Favorites()
	{
		favorites.reserve(20);
		const char* env =
#if defined(_WIN32)
			std::getenv("USERPROFILE");
#else
			std::getenv("HOME");
#endif
		if (env) 
		{
			add_fav(fs::directory_entry(fs::path(env) / "Desktop"));
			add_fav(fs::directory_entry(fs::path(env) / "Documents"));
			add_fav(fs::directory_entry(fs::path(env) / "Downloads"));
		}
	}

	const bool is_empty() const;

	const bool add_fav(const fs::directory_entry& entry);

	void delete_fav(const fs::directory_entry& entry);

	const bool is_exist(const fs::path& entry_path) const;

	const std::vector<fs::directory_entry>& get_favorites() const;

	void update_name(const fs::directory_entry& entry, const fs::path& newPath);
};

