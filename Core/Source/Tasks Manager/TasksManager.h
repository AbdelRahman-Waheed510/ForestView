#pragma once

#include "FileExplorer.h"

namespace fs = std::filesystem;

enum class TaskType
{
NONE,
COPY_PASTE,
_DELETE,
FIND_EMPTY_DIRECTORIES,
FIND_DUPLICATE_FILES,
FIND_LARGEST_FILES
};

static std::atomic<int> s_CopyPasteTasksCount{ 0 };
static std::atomic<int> s_DeleteTasksCount{ 0 };

static int get_copy_paste_Tasks_count() 
{
	return s_CopyPasteTasksCount.load();
}
static int get_delete_Tasks_count()
{
	return s_DeleteTasksCount.load();
}

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

	Task(TaskType type) : task_type(type)
	{
		if (task_type == TaskType::COPY_PASTE) ++s_CopyPasteTasksCount;
		else if (task_type == TaskType::_DELETE) ++s_DeleteTasksCount;
	}
	~Task() 
	{
		if (task_type == TaskType::COPY_PASTE) --s_CopyPasteTasksCount;
		else if (task_type == TaskType::_DELETE) --s_DeleteTasksCount;
	}
};