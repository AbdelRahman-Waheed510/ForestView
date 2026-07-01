// Data Structures & Containers
#include <vector>
#include <string>
#include <unordered_map>
#include <array>
#include <map>

// Input/Output & Filesystem
#include <iostream>
#include <fstream>
#include <filesystem>

// Memory Management & Utilities
#include <memory>
#include <algorithm>
#include <utility>
#include <functional>

// System, Types & Time Tracking
#include <cstdint>
#include <chrono>

#include <filesystem>
#include "tree.h"

#include <Windows.h>

namespace fs = std::filesystem;
using namespace std;

int main() 
{
	SetConsoleOutputCP(CP_UTF8);
	tree t;
	fs::path targetDir = "D:\\FAMILY";


	fs::path parent;
	if (targetDir.has_parent_path()) parent = targetDir.parent_path();

	t.AddNode(targetDir.filename());

	for (const auto& entry : fs::recursive_directory_iterator(targetDir))
	{
		if (!fs::is_regular_file(entry))
		{
			if (targetDir.has_parent_path()) t.AddNode(fs::relative(entry, parent));
			else  t.AddNode(entry);
		}

	}
	t.print();
	
	return 0;
}