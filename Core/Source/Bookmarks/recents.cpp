#include "recents.h"

const std::deque<fs::directory_entry> Recent::get_recent_history()
{
	return recent_history;
}

void Recent::visit(const fs::directory_entry& entry_to_visit)
{
	fs::directory_entry directory = entry_to_visit;
	if (entry_to_visit.path().empty())
	{
		directory = fs::directory_entry("THIS PC");
	}

	if (is_exist(directory).success)
	{
		remove(directory);
	}

	if (recent_history.size() >= limit)
	{
		recent_history.pop_front();
		recent_history.push_back(directory);
	}
	else
	{
		recent_history.push_back(directory);
	}
}

bool Recent::remove(const fs::directory_entry& entry_to_remove)
{
	if (recent_history.empty()) return false;

	is_exist_return check = is_exist(entry_to_remove);

	if (check.success)
	{
		auto it = recent_history.begin() + check.index;
		recent_history.erase(it);
		return true;
	}
	return false;
}

is_exist_return Recent::is_exist(const fs::directory_entry& entry)
{
	if (recent_history.empty()) return is_exist_return(false, -1);

	for (size_t i = 0; i < recent_history.size(); ++i)
	{
		if (recent_history[i] == entry)
		{
			return is_exist_return(true, i);
		}
	}
	return is_exist_return(false, -1);
}

bool Recent::update_name(const fs::directory_entry& entry_to_update, const fs::path& newPath)
{
	if (recent_history.empty()) return false;

	is_exist_return check = is_exist(entry_to_update);

	if (check.success)
	{
		recent_history[check.index].assign(newPath);
		return true;
	}
	return false;
}