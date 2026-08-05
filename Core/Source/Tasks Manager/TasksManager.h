#include <queue>
#include <filesystem>
#include <vector>
#include <memory>
#include <functional>
#include <thread>

#include "FileExplorer.h"

namespace fs = std::filesystem;

enum class TaskType
{
NONE,
COPY_PASTE,
_DELETE,
};

struct Task 
{
	std::string sourceName;
	std::string destinationName;

	TaskType task_type;
	fs::path source;
	fs::path destination;
	std::atomic<double> totalSize = 0.0;
	int totalfilesCount = 0;
	std::atomic<double> sizeProgress = 0.0;
	std::atomic<int> filesCountProgress = 0.0;
	std::atomic<bool> finished{ false };
	std::atomic<bool> cancel_request{ false };
	bool is_closing = false;

	// Attributes for the speed calculations
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
	double sizeProgressTemp = 0.0;
	double currentSpeed = 0.0;

	std::thread worker;

};