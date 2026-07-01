#include "tree.h"
    

bool tree::isEmpty()
{
    return (root == nullptr) ? true : false;
}

void tree::AddNode_helper(const fs::path& value, Node*& root)
{
    if (root == nullptr) 
    {
        root = new Node(value.filename());
    }

    else if (root->children.empty())
    {
        Node* newnode = new Node(value.filename());
        root->children.emplace_back(newnode);
    }
    else 
    {
        Node* parentNode = TraverseToNode(value.parent_path(), root);
        if (parentNode != nullptr) parentNode->children.emplace_back(new Node(value.filename()));
    }
    return;
}

void tree::AddNode(const fs::path& value)
{
    AddNode_helper(value, root);
    return;
}

tree::Node* tree::DeleteNode(fs::path& value, Node* root)
{
    return nullptr;
}

tree::Node* tree::TraverseToNode_helper(const fs::path& value, Node* root)
{
    if (root == nullptr) return nullptr;

    if (root->path == value) return root;

    for (const auto& child : root->children) 
    {
        if (child->path == value) return child;
    }

    return nullptr;
}

tree::Node* tree::TraverseToNode(const fs::path& value, Node* root)
{
    Node* node = root;
    for (const fs::path& part : value) 
    {
        node = TraverseToNode_helper(part, node);
        if (node == nullptr) return nullptr;
    }
    return node;
}

void tree::print_helper(Node* root, std::string prefix, bool is_last)
{
    if (isEmpty()) 
    {
        std::cout << "The directory does not exist...." << std::endl;
        return;
    }
    if (is_last) 
    {
        std::u8string u8_path = (root->path).u8string();
        std::string path(u8_path.begin(), u8_path.end());
        if (utils.hasArabic(path))
        {
            std::string processedName = utils.reverseArabic(path);
            std::cout << prefix << "\\-- " << "\xE2\x80\x8F" << processedName << std::endl;
        }
        else {
            std::cout << prefix << "\\-- " << path << std::endl;
        }
        prefix += "    ";
    }
    else 
    {
        std::u8string u8_path = (root->path).u8string();
        std::string path(u8_path.begin(), u8_path.end());

        if (utils.hasArabic(path))
        {
            std::string processedName = utils.reverseArabic(path);
            std::cout << prefix << "|--  " << "\xE2\x80\x8F" << processedName << std::endl;
        }
        else {
            std::cout << prefix << "|--  " << path << std::endl;
        }
        prefix += "|   ";

    }


    for (size_t i = 0; i < root->children.size(); ++i) 
    {
        bool is_last_child = false;
        if (i == root->children.size() - 1) is_last_child = true;
        print_helper(root->children[i], prefix, is_last_child);
    }

}

void tree::print() 
{
    print_helper(root, "", true);

    return;
}
