#include <queue>
#include <filesystem>
#include <vector>
#include <memory>
#include <functional>
#include <thread>

#include "FileExplorer.h"

namespace fs = std::filesystem;

enum TaskType 
{
NONE,
COPY_PASTE,
};

struct Task 
{
	TaskType task_type;
	fs::path source;
	fs::path destination;
	std::atomic<double> totalSize = 0.0;
	int totalfilesCount = 0;
	std::atomic<double> sizeProgress = 0.0;
	std::atomic<int> filesCountProgress = 0.0;
	std::atomic<bool> finished{ false };
	bool is_closing = false;
	std::thread worker;

};