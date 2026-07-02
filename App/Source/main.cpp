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
#include "ForestView.h"

#include <Windows.h>

#include "FileExplorer.h"

#include <io.h>
#include <fcntl.h>

namespace fs = std::filesystem;
using namespace std;

int main() 
{
	ForestView f;
	f.run_loop();
	//Explorer exp;
	//exp.start_the_explorer();
	//exp.print_entries();

	//exp.load_entries_from_specific_path(R"(D:\)");
	//exp.print_entries();

//	_setmode(_fileno(stdout), _O_U16TEXT);
//
//	tree t;
//
//	fs::path targetDir = R"(D:\GAMES\Zuma)";
//
//	t.BuildTree(targetDir);
//
//	t.print();
//	
//	fs::path toDelete = "D:\\GAMES\\Zuma\\Luxor";
//
//	t.DeleteNode(toDelete);
//
//	t.print();
	return 0;
}