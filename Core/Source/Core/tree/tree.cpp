#include "tree.h"
    

bool tree::isEmpty()
{
    return (root == nullptr) ? true : false;
}

void tree::AddNode_helper(const fs::path& value, Node*& root)
{
    if (root == nullptr) 
    {
        if (value == value.root_path()) root = new Node(value);
        else root = new Node(value);
    }

    else if (root->children.empty())
    {
        Node* newnode = new Node(value);
        root->children.emplace_back(newnode);
    }
    else 
    {
        Node* parentNode = TraverseToNode(value.parent_path(), root);
        if (parentNode != nullptr) parentNode->children.emplace_back(new Node(value));
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
    if (!isEmpty())
    {
        delete root;
        root = nullptr;
    }
    
    if (!fs::exists(value)) return;

    fs::path base_path = value.has_parent_path() ? value.parent_path() : value;

    if (value == base_path) 
    {
        AddNode(value);
    }
    else AddNode(fs::relative(value, base_path));

    for (const auto& entry : fs::recursive_directory_iterator(value))
    {
        if (fs::is_directory(entry))
        {
            if (value == base_path) 
            {
                AddNode(entry.path());
            }
            else AddNode(fs::relative(entry.path(), base_path));
        }
    }

}

tree::Node* tree::TraverseToNode_helper(const fs::path& absolute_target, Node* current)
{
    if (current == nullptr) return nullptr;
    if (current->path == absolute_target) return current;

    std::wstring target_str = absolute_target.wstring();

    for (const auto& child : current->children)
    {
        std::wstring child_str = child->path.wstring();

        if (target_str.rfind(child_str, 0) == 0)
        {
            if (target_str.size() > child_str.size() && target_str[child_str.size()] == fs::path::preferred_separator)
            {
                return TraverseToNode_helper(absolute_target, child);
            }
        }
    }

    return nullptr;
}

tree::Node* tree::TraverseToNode(const fs::path& value, Node* root)
{
    return TraverseToNode_helper(value, root);
}

void tree::print_helper(Node* root, std::wstring prefix, bool is_last)
{
    /*if (isEmpty()) 
    {
        std::cout << "The directory does not exist...." << std::endl;
        return;
    }*/
    if (root == nullptr) return;

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
    if (isEmpty() || root == nullptr)
    {
        std::wcout << L"The directory does not exist...." << std::endl;
        return;
    }
    print_helper (root,L"", true);

    return;
}
