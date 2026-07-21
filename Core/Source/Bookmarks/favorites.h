#pragma once

#include <vector>
#include <iostream>
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
	}

	const bool is_empty() const;

	const bool add_fav(const fs::directory_entry& entry);

	void delete_fav(const fs::directory_entry& entry);

	const bool is_exist(const fs::path& entry_path) const;

	const std::vector<fs::directory_entry>& get_favorites() const;

	void update_name(const fs::directory_entry& entry, const fs::path& newPath);
};

