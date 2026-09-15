#include "pchCore.h"
#include "tree.h"

bool tree::isEmpty()
{
    return (root == nullptr) ? true : false;
}

void tree::AddNode_helper(const fs::path& value, Node*& root)
{
    if (root == nullptr) 
    {
        root = new Node(value);
    }

    else if (value.parent_path() == root->path)
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

void tree::AddChildrenOf(const fs::path& value)
{
    for (const auto& entry : fs::directory_iterator(value, fs::directory_options::skip_permission_denied)) 
    {

        DWORD attr = GetFileAttributesW(entry.path().c_str());

        if (attr == INVALID_FILE_ATTRIBUTES)
            continue; // couldn't stat it, skip

        if (attr & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM))
            continue; // skip hidden/system

        if (fs::is_directory(entry)) 
        {
            AddNode(entry);
        }
    }
}

void tree::DeleteNode_helper(fs::path& value, Node* root)
{
    Node* parent = TraverseToNode(value.parent_path(), root);
    if (parent == nullptr) return;

    for (auto it = parent->children.begin(); it != parent->children.end(); it++)     
    {
        if ((*it)->path == value)
        {
            delete *it;
            parent->children.erase(it);
            break;
        }
    }
    return;
}

void tree::DeleteNode(fs::path value)
{
    DeleteNode_helper(value, root);
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

    AddNode(value);

    auto option = fs::directory_options::skip_permission_denied;

    for (const auto& entry : fs::recursive_directory_iterator(value, option))
    {
        if (fs::is_directory(entry))
        {
            AddNode(entry.path());
        }
    }

}

Node* tree::get_root()
{
    return root;
}

void tree::update_tree(const std::vector<fs::path>& partitions)
{
    if (root == nullptr) return;
    DeleteNode(root->path);
    iniThisPc("THIS PC");
    fillThisPcWithDrives(partitions);
}

Node* tree::TraverseToNode_helper(const fs::path& absolute_target, Node* current)
{
    if (current == nullptr) return nullptr;
    if (current->path == absolute_target) return current;

    for (const auto& child : current->children)
    {
        Node* foundParent = TraverseToNode_helper(absolute_target, child);

        if (foundParent != nullptr) return foundParent;
    }
    return nullptr;
}

Node* tree::TraverseToNode(const fs::path& value, Node* root)
{
    return TraverseToNode_helper(value, root);
}

void tree::iniThisPc(const std::string& label)
{
    if (!isEmpty())
    {
        delete root;
        root = nullptr;
    }
    root = new Node(fs::path());
    root->virtualName = label;
    root->is_loaded = true;
}

void tree::print_helper(Node* root, std::wstring prefix, bool is_last)
{
    if (root == nullptr) return;

    if (is_last) 
    {

        std::wcout << prefix << L"└── " << root->path.filename().wstring() << L'\n';
        prefix += L"    ";
    }
    else 
    {
        std::wcout << prefix << L"├── " << root->path.filename().wstring() << L'\n';
        prefix += L"│   ";
    }


    for (size_t i = 0; i < root->children.size(); ++i) 
    {
        bool is_last_child = false;
        if (i == root->children.size() - 1) is_last_child = true;
        print_helper(root->children[i], prefix, is_last_child);
    }

}

void tree::fillThisPcWithDrives(const std::vector<fs::path>& drives)
{
    if (root == nullptr) return;
    root->children.reserve((int)drives.size());
    for (const fs::path& drive : drives)
    {
        root->children.emplace_back(new Node(drive));
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

void tree::put_structure_into_string_helper(std::string& result, Node* root, std::string prefix, bool is_last)
{
    if (root == nullptr) return;

    auto u8Name = root->path.filename().u8string();
    std::string safeName(reinterpret_cast<const char*>(u8Name.data()), u8Name.size());

    if (is_last)
    {
        result += prefix + "\\-- " + safeName + '\n';
        prefix += "    ";
    }
    else
    {
        result += prefix + "+-- " + safeName + '\n';
        prefix += "|   ";
    }


    for (size_t i = 0; i < root->children.size(); ++i)
    {
        bool is_last_child = false;
        if (i == root->children.size() - 1) is_last_child = true;
        put_structure_into_string_helper(result, root->children[i], prefix, is_last_child);
    }

}

std::string tree::put_structure_into_string()
{
    std::string result = "";
    put_structure_into_string_helper(result, root, "", true);
    return result;
}
