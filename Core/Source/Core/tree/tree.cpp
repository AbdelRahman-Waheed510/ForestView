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

void tree::DeleteNode_helper(fs::path& value, Node* root)
{
    Node* parent = TraverseToNode(value.parent_path(), root);
    for (auto it = parent->children.begin(); it != parent->children.end(); ++it)
    {
        if ((*it)->path == value.filename())
        {
            delete* it;
            parent->children.erase(it);
            break;
        }
    }
    parent = nullptr;
    return;
}

void tree::DeleteNode(fs::path value)
{
    if (value.has_parent_path()) 
    {
        fs::path to_delete = fs::relative(value, value.parent_path());
        DeleteNode_helper(to_delete, root);
    }
    else 
    {
        DeleteNode_helper(value, root);
    }
    return;
}

void tree::BuildTree(const fs::path& value)
{
    if (!isEmpty()) delete root;
    
    AddNode(value);
    
    if (value.has_parent_path()) 
    {
        fs::path parent = value.parent_path();
        for (const auto& entry : fs::recursive_directory_iterator(value))
        {
            if (!fs::is_regular_file(entry))
            {
                AddNode(fs::relative(entry, parent));
            }
        }
    }
    else 
    {
        for (const auto& entry : fs::recursive_directory_iterator(value))
        {
            if (!fs::is_regular_file(entry))
            {
                AddNode(entry);
            }
        }
    }

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

void tree::print_helper(Node* root, std::wstring prefix, bool is_last)
{
    if (isEmpty()) 
    {
        std::cout << "The directory does not exist...." << std::endl;
        return;
    }
    if (is_last) 
    {

        std::wcout << prefix << L"└── " << root->path.wstring() << L'\n';
        prefix += L"    ";
    }
    else 
    {
        std::wcout << prefix << L"├── " << root->path.wstring() << L'\n';
        prefix += L"│   ";
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
    print_helper
    (root,L"", true);

    return;
}
