#pragma once

#include "favorites.h"
#include "recents.h"
#include "tree.h"

namespace fs = std::filesystem;

#include "FileExplorer.h"
#include "TasksManager.h"
#include "Index.h"

static int id = 1;

struct KeyPressedData {
	ImGuiKey key;
	wchar_t englishUpper;
	wchar_t englishLower;
	wchar_t arabicChar;
};

static constexpr KeyPressedData s_alphabetMappings[] = {
	{ ImGuiKey_A, L'A', L'a', L'\u0634' }, // SHEEN
	{ ImGuiKey_B, L'B', L'b', L'\u0644' }, // LAM 
	{ ImGuiKey_C, L'C', L'c', L'\u0624' }, // WAW WITH HAMZA ABOVE
	{ ImGuiKey_D, L'D', L'd', L'\u064A' }, // YEH
	{ ImGuiKey_E, L'E', L'e', L'\u062B' }, // THEH
	{ ImGuiKey_F, L'F', L'f', L'\u0628' }, // BEH
	{ ImGuiKey_G, L'G', L'g', L'\u0644' }, // LAM
	{ ImGuiKey_H, L'H', L'h', L'\u0627' }, // ALEF
	{ ImGuiKey_I, L'I', L'i', L'\u0647' }, // HEH
	{ ImGuiKey_J, L'J', L'j', L'\u062A' }, // TEH
	{ ImGuiKey_K, L'K', L'k', L'\u0646' }, // NOON
	{ ImGuiKey_L, L'L', L'l', L'\u0645' }, // MEEM
	{ ImGuiKey_M, L'M', L'm', L'\u0629' }, // TEH MARBUTA
	{ ImGuiKey_N, L'N', L'n', L'\u0649' }, // ALEF MAKSURA
	{ ImGuiKey_O, L'O', L'o', L'\u062E' }, // KHAH
	{ ImGuiKey_P, L'P', L'p', L'\u062D' }, // HAH
	{ ImGuiKey_Q, L'Q', L'q', L'\u0636' }, // DAD
	{ ImGuiKey_R, L'R', L'r', L'\u0642' }, // QAF
	{ ImGuiKey_S, L'S', L's', L'\u0633' }, // SEEN
	{ ImGuiKey_T, L'T', L't', L'\u0641' }, // FEH
	{ ImGuiKey_U, L'U', L'u', L'\u0639' }, // AIN
	{ ImGuiKey_V, L'V', L'v', L'\u0631' }, // REH
	{ ImGuiKey_W, L'W', L'w', L'\u0635' }, // SAD
	{ ImGuiKey_X, L'X', L'x', L'\u0621' }, // HAMZA
	{ ImGuiKey_Y, L'Y', L'y', L'\u063A' }, // GHAIN
	{ ImGuiKey_Z, L'Z', L'z', L'\u0626' }, // YEH WITH HAMZA ABOVE
};

enum viewType
{
	LARGE_THUMBNAILS,
	ROWS,
};

enum openTapType 
{
NEW, 
COPY
};

struct State 
{
	int tabID;
	std::unique_ptr<Explorer> fileExplorer;
	float icons_width = 24.0f;
	float icons_height = 24.0f;
	float padding = 32.0f;

	viewType view_type;

	Favorites fav;
	Recent recents;


	bool show_create_folder = false;
	bool show_selected_folder_options = false;
	bool show_rename_file = false;

	bool entries_need_update = false;

	bool need_file_size_call = false;
	bool need_modified_since_call = false;

	bool show_deletion_warning = false;

	bool global_key_need_update = false;

	bool tree_need_update = false;
	bool is_calulating_size = false;
	bool is_coping_structure = false;

	bool is_searching = false;

	bool need_folder_right_click_call = false;
	bool need_file_right_click_call = false;

	fs::path selected_path;
	fs::path current_path;
	std::vector<fs::path> copied_paths;
	fs::path copied_path;
	std::string copied_structure = "";
	fs::path deleted_path;

	std::string message;

	std::string globalKeyWord;
	std::string ThisDirectoryKeyWord;

	std::vector<Entry> entries;

	std::vector<std::future<fileSize>> file_size_future;

	fileSize file_size;
	std::string last_modified;

	std::string utf8_search_bar_hint = "This PC";

	std::future<std::string> folder_structure;

	tree t;
	tree structure_tree;

	std::atomic<double> copying_progress;

	std::vector<fs::path> selectedEntries;
	std::vector<fs::path> selectedEntriesSnapShot;
	bool isSelecting = false;

	int lastPress = -1;
	State() 
	{
		tabID = id;
		id++;
		copied_structure.reserve(1024 * 512);
		view_type = ROWS;
	}
};

enum class Theme 
{
DARK_MODE,
DARK_MODE_V2,
LIGHT_MODE,
TEST,
};

class App 
{
private:

	GLFWwindow* window = nullptr;

	Theme m_activeTheme = Theme::LIGHT_MODE;

	virtual void on_start() {};

	virtual void on_update() = 0;

	virtual void on_shutdown() {};

	bool initialize_GLFW();

	bool create_window();

	bool initialize_GLAD();

	void initialize_ImGui();

	void render();

	void clean_up();

	void dark_mode();

	void dark_mode_v2();

	void light_mode();

	void spacing_and_padding_and_stuff(ImGuiStyle& style);

	void test_mode();

	bool m_isRunning = true;
protected:
	ImFont* default_font = nullptr;
	ImFont* arabic_font = nullptr;

	const ImVec4 unique_color = ImVec4(0.38f, 0.60f, 0.95f, 1.0f);

	void Shut_Down();
public:

	App() = default;

	~App() 
	{
		clean_up();
	}

	void change_theme(const Theme& theme);

	const Theme get_active_theme();
	

	void run_loop();

};

class ForestView : public App 
{
private:
	TexturesManager& M_texturesManager;
	Index m_index;

	float folder_icons_horizontel_indent = 20.0f;
	float folder_icons_vertical_indent = 20.0f;

	std::vector<std::unique_ptr<State>> tabs;

	int current_tab;

	openTapType m_openTabType = NEW;

	int open_new_tab_factor = -1; // to auto switch to the new opend tab

	int drop_source = -1;
	int drop_target = -1;

	bool need_access_denied_popup = false;

	fileSize size_count;

	bool need_delete_confirmation = false;
	bool show_icons_slider = false;

	std::vector<std::unique_ptr<Task>> tasks;
	std::deque<std::unique_ptr<Task>> m_queuedTasks;

	std::unique_ptr<Task> m_emptyFoldersTask;
	std::vector<fs::path> m_emptyFolders;
	bool is_searching_for_empty_folders = false;

	std::unique_ptr<Task> m_duplicateFilesTask;
	std::vector<Duplicate> m_duplicateFiles;
	bool is_searching_for_duplicates_files = false;

	std::unique_ptr<Task> m_largestFilesTask;
	std::vector<fs::directory_entry> m_largestFiles;
	bool is_searching_for_largest_files = false;

	std::vector<fs::path> m_founds;
	bool m_showSearchWindow = false;
	bool m_isSearcing = false;

	std::atomic<int> m_activeCopyingTasks{ 0 };
	std::atomic<int> m_activeDeleteTasks{ 0 };

	ImVec2 m_SelectionRecStartingPos;
	bool m_isDraggingSelection = false;
	bool m_ctrlPressd = false;
public:

	ForestView(TexturesManager& texture_manager) : M_texturesManager(texture_manager)
	{
		tabs.push_back(std::make_unique<State>());		

		current_tab = 0;
		size_count.size = 0.0;

		m_index.startIdexing();
	}

	~ForestView() 
	{
	}

	void render_msg();

	void on_start() override;

	// Main update function (each frame gets called)

	void on_update();

	void refocus_reload();

	// shutdown 

	void on_shutdown();

	// Search Bar
	void search_bar();

	// Tool Bar
	void tool_bar();

	// Popups
	void view_settings();

	void new_whatever();

	void view_sort_options();

	void delete_warning();

	void filter_by();

	void largest_files_slider();

	void advanced_search();

	void access_denied();
	// Folders Tree 
	void folders_tree(const float& remainingHeight);

	void draw_tree(Node* node_to_show);

	// Files Navigation Panel
	void files_navigation_panel(const float& remainingHeight);

	void render_entries();
	
	void render_debug_panel();

	void handle_entries_mouse_click(const fs::directory_entry& entry);

	void decide_correct_rightClick_popup();

	void folders_right_click_popup(const fs::directory_entry& FolderEntry);

	void files_right_click_popup(const fs::directory_entry& entry);

	void left_double_click(const fs::directory_entry& entry);

	void left_click(const fs::directory_entry& entry);

	// Stats Bar
	void stats_bar();

	void icons_size_slider();

	// Progress Bar
	void show_progress_bar(const std::string& overlay);

	void shutdown_background_tasks();

	// tabs Bar
	void tab_bar();

	std::unique_ptr<State> open_new_tab(std::unique_ptr<State>& newTab);

	std::unique_ptr<State> open_copy_tab(std::unique_ptr<State>& newTab);

	std::string WStringToUTF8(const std::wstring& wstr) {
		if (wstr.empty()) return "";
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(wstr);
	}

	// Convert UTF-8 std::string to std::wstring
	std::wstring UTF8ToWString(const std::string& str) {
		if (str.empty()) return L"";
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(str);
	}

	bool InputTextWString(const char* label, std::wstring& wstr, ImGuiInputTextFlags flags = 0) {
		std::string utf8_str = WStringToUTF8(wstr);

		// 2. Pass the UTF-8 string to ImGui
		if (ImGui::InputText(label, &utf8_str, flags)) {
			// 3. If modified, translate the output back to the original wstring
			wstr = UTF8ToWString(utf8_str);
			return true; // Returns true exactly when text changes
		}
		return false;
	}

	ImVec2 GetSelectionStartScreenPos() const;
};