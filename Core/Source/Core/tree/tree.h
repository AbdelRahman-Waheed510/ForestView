#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <iostream>

namespace fs = std::filesystem;


struct Node
{
	fs::path path;
	std::vector<Node*> children;
	std::string virtualName;
	bool is_loaded = false;

	Node() = default;

	Node(fs::path p)
	{
		path = p;
	}

	Node(const Node& other)
	{
		path = other.path;
		for (const Node* it : other.children)
		{
			if (it != nullptr)
			{
				children.emplace_back(new Node(*it));
			}
		}
	};

	Node& operator =(const Node& other)
	{
		if (this == &other) return *this;
		path = other.path;
		for (Node* it : children)
		{
			if (it != nullptr)
			{
				delete it;
			}
		}
		children.clear();
		children.reserve(other.children.size());
		for (const Node* it : other.children)
		{
			if (it != nullptr)
			{
				children.emplace_back(new Node(*it));
			}
		}
		return *this;
	};

	~Node()
	{
		for (Node* it : children)
		{
			if (it != nullptr)
			{
				delete it;
			}
		}
	}

};

class tree 
{
private:


	Node* root;

	void AddNode_helper(const fs::path& value, Node*& root);

	void print_helper(Node* root, std::wstring prefix, bool is_last = true);

	void DeleteNode_helper(fs::path& value, Node* root);

public:

	tree()
	{
		root = nullptr;
	}

	bool isEmpty();

	void AddNode(const fs::path& value);

	void AddChildrenOf(const fs::path& value);

	void DeleteNode(fs::path value);

	void BuildTree(const fs::path& value);

	Node* get_root();

	void update_tree(const std::vector<fs::path>& partitions);

	Node* TraverseToNode_helper(const fs::path& value, Node* root);

	Node* TraverseToNode(const fs::path& value, Node* root);

	void iniThisPc(const std::string& label);
	
	void fillThisPcWithDrives(const std::vector<fs::path>& drives);

	void print();
};
