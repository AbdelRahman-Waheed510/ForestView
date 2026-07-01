
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

class Utils
{
private:

public:

	Utils() = default;

	std::string reverseArabic(const std::string& text) {
		std::string reversed = "";
		for (int i = text.length() - 1; i >= 0; i--) {
			if (text[i] < 0 && i > 0) {
				reversed += text[i - 1];
				reversed += text[i];
				i--;
			}
			else {
				reversed += text[i];
			}
		}
		return reversed;
	}

	bool hasArabic(const std::string& text) {
		for (unsigned char c : text) {
			if (c == 0xD8 || c == 0xD9) return true;
		}
		return false;
	}

};

class tree 
{
private:

	Utils utils;

	struct Node
	{
		fs::path path;
		std::vector<Node*> children;

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

	Node* root;

	void AddNode_helper(const fs::path& value, Node*& root);

	void print_helper(Node* root, std::string prefix = "", bool is_last = true);

public:

	tree()
	{
		root = nullptr;
	}

	bool isEmpty();

	void AddNode(const fs::path& value);

	Node* DeleteNode(fs::path& value, Node* root);

	Node* TraverseToNode_helper(const fs::path& value, Node* root);

	Node* TraverseToNode(const fs::path& value, Node* root);

	void print();
};
