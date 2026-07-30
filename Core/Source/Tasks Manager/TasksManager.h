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
	std::atomic<double> total = 0.0;
	std::atomic<double> progress = 0.0;
	std::atomic<bool> finished{ false };
};