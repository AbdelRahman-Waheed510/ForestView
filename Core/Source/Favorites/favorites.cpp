#include "favorites.h"

const bool Favorites::is_empty() const
{
	return favorites.empty();
}

const bool Favorites::add_fav(const fs::directory_entry& entry)
{
	if (favorites.size() == 20) 
	{
		return false;
	}
	favorites.emplace_back(entry);
	return true;
}

void Favorites::delete_fav(const fs::directory_entry& entry)
{
	std::erase(favorites, entry);
}

const bool Favorites::is_exist(const fs::path& entry_path) const 
{
	if (favorites.empty()) return false;
	for (const fs::directory_entry& favorite : favorites) 
	{
		if (favorite.path() == entry_path)
		{
			return true;
		}
	}
	return false;
}

const std::vector<fs::directory_entry> Favorites::get_favorites() const
{
	return favorites;
}

void Favorites::update_name(const fs::directory_entry& entry, const fs::path& newPath)
{
	for (fs::directory_entry& _entry : favorites) 
	{
		if (_entry == entry) 
		{
			_entry.assign(newPath);
			break;
		}
	}
}
