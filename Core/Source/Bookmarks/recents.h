#pragma once

#include <vector>
#include <iostream>
#include <filesystem>
#include <stack>

namespace fs = std::filesystem;


struct is_exist_return
{
	bool success;
	int index;

	is_exist_return(bool s, int i)
	{
		success = s;
		index = i;
	}
};

class Recent
{
private:
	int limit = 10;
	std::deque<fs::directory_entry> recent_history;

public:
	const bool is_empty();
	const std::deque<fs::directory_entry>& get_recent_history();
	void visit(const fs::directory_entry& entry_to_visit);
	bool remove(const fs::directory_entry& entry_to_remove);
	is_exist_return is_exist(const fs::directory_entry& entry);
	bool update_name(const fs::directory_entry& entry_to_update, const fs::path& newPath);
};