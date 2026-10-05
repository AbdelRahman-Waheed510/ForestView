#pragma once

#include "favorites.h"
#include "recents.h"
#include "tree.h"

namespace fs = std::filesystem;

#include "FileExplorer.h"
#include "TasksManager.h"
#include "Index.h"
#include "Selection Manager/SelectionManager.h"

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
	SelectionManager selectionManager;

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
	bool need_selection_right_click_call = false;

	fs::path selected_path;
	fs::path current_path;
	std::vector<fs::path> copied_paths;
	fs::path copied_path;
	std::string copied_structure = "";
	fs::path deleted_path;

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

	std::vector<SelectionRange> selectedEntriesSnapShot;
	bool isSelecting = false;

	float entriesContentStartY = 0.0f;
	float measuredRowHeight = 0.0f;
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
	const ImVec4 unique_color_2 = ImVec4(1.000f, 0.624f, 0.000f, 1.000f);

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
	std::vector<EmptyFolder> m_emptyFolders;
	bool is_searching_for_empty_folders = false;

	std::unique_ptr<Task> m_duplicateFilesTask;
	std::vector<Duplicate> m_duplicateFiles;
	bool is_searching_for_duplicates_files = false;

	std::unique_ptr<Task> m_largestFilesTask;
	std::vector<LargestFile> m_largestFiles;
	bool is_searching_for_largest_files = false;

	std::vector<fs::path> m_founds;
	bool m_showSearchWindow = false;
	bool m_isSearcing = false;
	bool m_search_need_update = false;

	std::atomic<int> m_activeCopyingTasks{ 0 };
	std::atomic<int> m_activeDeleteTasks{ 0 };

	ImVec2 m_SelectionRecStartingPos;
	bool m_isDraggingSelection = false;
	bool m_ctrlPressd = false;
	bool m_shiftPressed = false;
	bool m_altPressed = false;

	Timer m_timer;
	std::string m_message;
	ImVec4 m_msgColor = ImVec4(0.0, 0.0, 0.0, 1.0);
	float m_numberOfSeconds = 2.0;

	float actualWidth;
	int columnsCount;

	bool m_showSelectionCursor = false;
	bool m_showSelectionAnchor = true;
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

	void copy_helper();

	void create_new_folder_helper();

	void create_new_file_helper(const NewType& type);

	void paste_helper();
	
	void go_back_helper();

	void go_forward_helper();

	void call_timed_tooltip(const std::string message, const ImVec4& color, const int8_t numberOfseconds);

	void timed_tooltip();

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

	void need_to_scroll(int selectedIndex);

	int find_accessible_index(int start, int direction);
	
	int click_accessible_index_largeTumbnails(int start, int x, int y);

	int find_accessible_index_largeTumbnails(int start, int x, int y, const int factor);

	void handle_entries_mouse_click(const fs::directory_entry& entry, int idx = -1);

	void decide_correct_rightClick_popup();

	void folders_right_click_popup(const fs::directory_entry& FolderEntry);

	void files_right_click_popup(const fs::directory_entry& entry);

	void selection_right_click_popup(const std::vector<SelectionRange>& selecitons);

	void left_double_click(const fs::directory_entry& entry);

	bool left_click(const fs::directory_entry& entry);

	bool check_access(const fs::directory_entry& entry);

	// Stats Bar
	void stats_bar();

	void icons_size_slider();

	// Progress Bar
	void show_progress_bar(const std::string& overlay);

	void shutdown_background_tasks();

	// tabs Bar
	void tab_bar();

	void create_tab_helper();

	std::unique_ptr<State> open_new_tab(std::unique_ptr<State>& newTab);

	std::unique_ptr<State> open_copy_tab(std::unique_ptr<State>& newTab);

	std::string WStringToUTF8(const std::wstring& wstr) {
		if (wstr.empty()) return "";
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(wstr);
	}

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

	void run_file(const fs::path& path) const 
	{
		std::wstring parameters = L"/c start \"\" \"" + path.wstring() + L"\"";
		HINSTANCE result = ShellExecuteW(nullptr, L"open", L"cmd.exe", parameters.c_str(), nullptr, SW_HIDE);
	}

};