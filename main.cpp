
// 1. Bajar freeglut http://www.lighthouse3d.com/tutorials/glut-tutorial/
// 2. Bajar AntTweakBar http://anttweakbar.sourceforge.net/doc/
// 3. Bajar "Open File Dialog", "open"
#define FREEGLUT_STATIC
#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif
// FreeGlut
#include "./GL/freeglut.h"
// ImGui
#include "./imgui/imgui.h"
//#include "imgui/imgui_draw.cpp"
#include "./imgui/backends/imgui_impl_glut.h"
#include "./imgui/backends/imgui_impl_opengl2.h"
// Utils
#include <list>
#include <iostream>
#include <memory>
#include <line.h>

using namespace std;

// Forward Declarations
static void ShowAppMainMenuBar();
static void ShowMenuFile();
static void ShowConfigWindow();
static void ShowWindowMisc();
static void ShowAboutWindow();
static void ShowStatsOverlay(bool* p_open);
void my_display_code();
void renderScene(void);
void changeSize(int w, int h);

int width = 1080, height = 720;

list <shared_ptr<CShape>> shapes;
shared_ptr<CShape> current_shape;

// Our state
static bool show_demo_window = false;
static bool show_another_window = true;
static ImVec4 clear_color = ImVec4(0.01f, 0.17f, 0.31f, 1.00f);

//-----------------------------------------------------------------------------
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
//-----------------------------------------------------------------------------
// - ShowExampleAppMainMenuBar()
// - ShowExampleMenuFile()
//-----------------------------------------------------------------------------

// Demonstrate creating a "main" fullscreen menu bar and populating it.
// Note the difference between BeginMainMenuBar() and BeginMenuBar():
// - BeginMenuBar() = menu-bar inside current window (which needs the ImGuiWindowFlags_MenuBar flag!)
// - BeginMainMenuBar() = helper to create menu-bar-sized window at the top of the main viewport + call BeginMenuBar() into it.
static void ShowAppMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Config"))
		{
			ShowConfigWindow();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About"))
		{
			ShowAboutWindow();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

// Note that shortcuts are currently provided for display only
// (future version will add explicit flags to BeginMenu() to request processing shortcuts)
static void ShowMenuFile()
{
	//IMGUI_DEMO_MARKER("Examples/Menu");
	ImGui::MenuItem("(demo menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowMenuFile();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As..")) {}

	ImGui::Separator();
	//IMGUI_DEMO_MARKER("Examples/Menu/Options");
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::EndMenu();
	}

	//IMGUI_DEMO_MARKER("Examples/Menu/Colors");
	if (ImGui::BeginMenu("Colors"))
	{
		float sz = ImGui::GetTextLineHeight();
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
			ImGui::Dummy(ImVec2(sz, sz));
			ImGui::SameLine();
			ImGui::MenuItem(name);
		}
		ImGui::EndMenu();
	}

	// Here we demonstrate appending again to the "Options" menu (which we already created above)
	// Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
	// In a real code-base using it would make senses to use this feature from very different code locations.
	if (ImGui::BeginMenu("Options")) // <-- Append!
	{
		//IMGUI_DEMO_MARKER("Examples/Menu/Append to an existing menu");
		static bool b = true;
		ImGui::Checkbox("SomeOption", &b);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true)) {}
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

static void ShowConfigWindow() {
	// Examples Apps (accessible from the "Examples" menu)
	static bool show_app_main_menu_bar = false;
	//static bool show_app_documents = false;

	//static bool show_app_console = false;
	//static bool show_app_log = false;
	//static bool show_app_layout = false;
	//static bool show_app_property_editor = false;
	//static bool show_app_long_text = false;
	//static bool show_app_auto_resize = false;
	//static bool show_app_constrained_resize = false;
	static bool show_app_simple_overlay = false;
	//static bool show_app_fullscreen = false;
	//static bool show_app_window_titles = false;
	//static bool show_app_custom_rendering = false;

	if (show_app_main_menu_bar)       ShowAppMainMenuBar();
	//if (show_app_documents)           ShowExampleAppDocuments(&show_app_documents);

	//if (show_app_console)             ShowExampleAppConsole(&show_app_console);
	//if (show_app_log)                 ShowExampleAppLog(&show_app_log);
	//if (show_app_layout)              ShowExampleAppLayout(&show_app_layout);
	//if (show_app_property_editor)     ShowExampleAppPropertyEditor(&show_app_property_editor);
	//if (show_app_long_text)           ShowExampleAppLongText(&show_app_long_text);
	//if (show_app_auto_resize)         ShowExampleAppAutoResize(&show_app_auto_resize);
	//if (show_app_constrained_resize)  ShowExampleAppConstrainedResize(&show_app_constrained_resize);
	if (show_app_simple_overlay)      ShowStatsOverlay(&show_app_simple_overlay);
	//if (show_app_fullscreen)          ShowExampleAppFullscreen(&show_app_fullscreen);
	//if (show_app_window_titles)       ShowExampleAppWindowTitles(&show_app_window_titles);
	//if (show_app_custom_rendering)    ShowExampleAppCustomRendering(&show_app_custom_rendering);

	// Dear ImGui Apps (accessible from the "Tools" menu)
	static bool show_app_metrics = false;
	static bool show_app_debug_log = false;
	static bool show_app_stack_tool = false;
	static bool show_app_about = false;
	static bool show_app_style_editor = false;

	if (show_app_metrics)
		ImGui::ShowMetricsWindow(&show_app_metrics);
	if (show_app_debug_log)
		ImGui::ShowDebugLogWindow(&show_app_debug_log);
	if (show_app_stack_tool)
		ImGui::ShowStackToolWindow(&show_app_stack_tool);
	if (show_app_about)
		ImGui::ShowAboutWindow(&show_app_about);
	if (show_app_style_editor)
	{
		ImGui::Begin("Dear ImGui Style Editor", &show_app_style_editor);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	// Demonstrate the various window flags. Typically you would just use the default!
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
	//if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

	// We specify a default position/size in case there's no data in the .ini file.
	// We only do it to make the demo applications a little more welcoming, but typically this isn't required.
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	// Main body of the Demo window starts here.
	ImGui::Begin("Config");
	
	ImGui::End();
	return;
	

	// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.

	// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
	//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);

	// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	// Menu Bar
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			//IMGUI_DEMO_MARKER("Menu/File");
			//ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Examples"))
		{
			//IMGUI_DEMO_MARKER("Menu/Examples");
			ImGui::MenuItem("Main menu bar", NULL, &show_app_main_menu_bar);
			//ImGui::MenuItem("Console", NULL, &show_app_console);
			//ImGui::MenuItem("Log", NULL, &show_app_log);
			//ImGui::MenuItem("Simple layout", NULL, &show_app_layout);
			//ImGui::MenuItem("Property editor", NULL, &show_app_property_editor);
			//ImGui::MenuItem("Long text display", NULL, &show_app_long_text);
			//ImGui::MenuItem("Auto-resizing window", NULL, &show_app_auto_resize);
			//ImGui::MenuItem("Constrained-resizing window", NULL, &show_app_constrained_resize);
			ImGui::MenuItem("Simple overlay", NULL, &show_app_simple_overlay);
			//ImGui::MenuItem("Fullscreen window", NULL, &show_app_fullscreen);
			//ImGui::MenuItem("Manipulating window titles", NULL, &show_app_window_titles);
			//ImGui::MenuItem("Custom rendering", NULL, &show_app_custom_rendering);
			//ImGui::MenuItem("Documents", NULL, &show_app_documents);
			ImGui::EndMenu();
		}
		//if (ImGui::MenuItem("MenuItem")) {} // You can also use MenuItem() inside a menu bar!
		if (ImGui::BeginMenu("Tools"))
		{
			//IMGUI_DEMO_MARKER("Menu/Tools");
#ifndef IMGUI_DISABLE_DEBUG_TOOLS
			const bool has_debug_tools = true;
#else
			const bool has_debug_tools = false;
#endif
			ImGui::MenuItem("Metrics/Debugger", NULL, &show_app_metrics, has_debug_tools);
			ImGui::MenuItem("Debug Log", NULL, &show_app_debug_log, has_debug_tools);
			ImGui::MenuItem("Stack Tool", NULL, &show_app_stack_tool, has_debug_tools);
			ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
			ImGui::MenuItem("About Dear ImGui", NULL, &show_app_about);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Text("dear imgui says hello! (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
	ImGui::Spacing();

	//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("Help"))
	{
		ImGui::Text("ABOUT THIS DEMO:");
		ImGui::BulletText("Sections below are demonstrating many aspects of the library.");
		ImGui::BulletText("The \"Examples\" menu above leads to more demo contents.");
		ImGui::BulletText("The \"Tools\" menu above gives access to: About Box, Style Editor,\n"
			"and Metrics/Debugger (general purpose Dear ImGui debugging tool).");
		ImGui::Separator();

		ImGui::Text("PROGRAMMER GUIDE:");
		ImGui::BulletText("See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");
		ImGui::BulletText("See comments in imgui.cpp.");
		ImGui::BulletText("See example applications in the examples/ folder.");
		ImGui::BulletText("Read the FAQ at http://www.dearimgui.org/faq/");
		ImGui::BulletText("Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.");
		ImGui::BulletText("Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.");
		ImGui::Separator();

		ImGui::Text("USER GUIDE:");
		ImGui::ShowUserGuide();
	}

	//IMGUI_DEMO_MARKER("Configuration");
	if (ImGui::CollapsingHeader("Configuration"))
	{
		ImGuiIO& io = ImGui::GetIO();

		if (ImGui::TreeNode("Configuration##2"))
		{
			ImGui::CheckboxFlags("io.ConfigFlags: NavEnableKeyboard", &io.ConfigFlags, ImGuiConfigFlags_NavEnableKeyboard);
			ImGui::SameLine(); //HelpMarker("Enable keyboard controls.");
			ImGui::CheckboxFlags("io.ConfigFlags: NavEnableGamepad", &io.ConfigFlags, ImGuiConfigFlags_NavEnableGamepad);
			ImGui::SameLine(); //HelpMarker("Enable gamepad controls. Require backend to set io.BackendFlags |= ImGuiBackendFlags_HasGamepad.\n\nRead instructions in imgui.cpp for details.");
			ImGui::CheckboxFlags("io.ConfigFlags: NavEnableSetMousePos", &io.ConfigFlags, ImGuiConfigFlags_NavEnableSetMousePos);
			ImGui::SameLine(); //HelpMarker("Instruct navigation to move the mouse cursor. See comment for ImGuiConfigFlags_NavEnableSetMousePos.");
			ImGui::CheckboxFlags("io.ConfigFlags: NoMouse", &io.ConfigFlags, ImGuiConfigFlags_NoMouse);
			if (io.ConfigFlags & ImGuiConfigFlags_NoMouse)
			{
				// The "NoMouse" option can get us stuck with a disabled mouse! Let's provide an alternative way to fix it:
				if (fmodf((float)ImGui::GetTime(), 0.40f) < 0.20f)
				{
					ImGui::SameLine();
					ImGui::Text("<<PRESS SPACE TO DISABLE>>");
				}
				if (ImGui::IsKeyPressed(ImGuiKey_Space))
					io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			}
			ImGui::CheckboxFlags("io.ConfigFlags: NoMouseCursorChange", &io.ConfigFlags, ImGuiConfigFlags_NoMouseCursorChange);
			ImGui::SameLine(); //HelpMarker("Instruct backend to not alter mouse cursor shape and visibility.");
			ImGui::Checkbox("io.ConfigInputTrickleEventQueue", &io.ConfigInputTrickleEventQueue);
			ImGui::SameLine(); //HelpMarker("Enable input queue trickling: some types of events submitted during the same frame (e.g. button down + up) will be spread over multiple frames, improving interactions with low framerates.");
			ImGui::Checkbox("io.ConfigInputTextCursorBlink", &io.ConfigInputTextCursorBlink);
			ImGui::SameLine(); //HelpMarker("Enable blinking cursor (optional as some users consider it to be distracting).");
			ImGui::Checkbox("io.ConfigDragClickToInputText", &io.ConfigDragClickToInputText);
			ImGui::SameLine(); //HelpMarker("Enable turning DragXXX widgets into text input with a simple mouse click-release (without moving).");
			ImGui::Checkbox("io.ConfigWindowsResizeFromEdges", &io.ConfigWindowsResizeFromEdges);
			ImGui::SameLine(); //HelpMarker("Enable resizing of windows from their edges and from the lower-left corner.\nThis requires (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors) because it needs mouse cursor feedback.");
			ImGui::Checkbox("io.ConfigWindowsMoveFromTitleBarOnly", &io.ConfigWindowsMoveFromTitleBarOnly);
			ImGui::Checkbox("io.MouseDrawCursor", &io.MouseDrawCursor);
			ImGui::SameLine(); //HelpMarker("Instruct Dear ImGui to render a mouse cursor itself. Note that a mouse cursor rendered via your application GPU rendering path will feel more laggy than hardware cursor, but will be more in sync with your other visuals.\n\nSome desktop applications may use both kinds of cursors (e.g. enable software cursor only when resizing/dragging something).");
			ImGui::Text("Also see Style->Rendering for rendering options.");
			ImGui::TreePop();
			ImGui::Separator();
		}

		//IMGUI_DEMO_MARKER("Configuration/Backend Flags");
		if (ImGui::TreeNode("Backend Flags"))
		{
			/*
			HelpMarker(
				"Those flags are set by the backends (imgui_impl_xxx files) to specify their capabilities.\n"
				"Here we expose them as read-only fields to avoid breaking interactions with your backend.");
			*/
			ImGui::Text("Those flags are set by the backends (imgui_impl_xxx files) to specify their capabilities.\n");
			ImGui::Text("Here we expose them as read-only fields to avoid breaking interactions with your backend.");

			// Make a local copy to avoid modifying actual backend flags.
			// FIXME: We don't use BeginDisabled() to keep label bright, maybe we need a BeginReadonly() equivalent..
			ImGuiBackendFlags backend_flags = io.BackendFlags;
			ImGui::CheckboxFlags("io.BackendFlags: HasGamepad", &backend_flags, ImGuiBackendFlags_HasGamepad);
			ImGui::CheckboxFlags("io.BackendFlags: HasMouseCursors", &backend_flags, ImGuiBackendFlags_HasMouseCursors);
			ImGui::CheckboxFlags("io.BackendFlags: HasSetMousePos", &backend_flags, ImGuiBackendFlags_HasSetMousePos);
			ImGui::CheckboxFlags("io.BackendFlags: RendererHasVtxOffset", &backend_flags, ImGuiBackendFlags_RendererHasVtxOffset);
			ImGui::TreePop();
			ImGui::Separator();
		}

		//IMGUI_DEMO_MARKER("Configuration/Style");
		if (ImGui::TreeNode("Style"))
		{
			//HelpMarker("The same contents can be accessed in 'Tools->Style Editor' or by calling the ShowStyleEditor() function.");
			
			ImGui::Text("The same contents can be accessed in 'Tools->Style Editor' or by calling the ShowStyleEditor() function.");
			ImGui::ShowStyleEditor();
			ImGui::TreePop();
			ImGui::Separator();
		}

		//IMGUI_DEMO_MARKER("Configuration/Capture, Logging");
		if (ImGui::TreeNode("Capture/Logging"))
		{
			/*
			HelpMarker(
				"The logging API redirects all text output so you can easily capture the content of "
				"a window or a block. Tree nodes can be automatically expanded.\n"
				"Try opening any of the contents below in this window and then click one of the \"Log To\" button.");
			*/
			ImGui::Text("The logging API redirects all text output so you can easily capture the content of ");
			ImGui::Text("a window or a block. Tree nodes can be automatically expanded.\n");
			ImGui::Text("Try opening any of the contents below in this window and then click one of the \"Log To\" button.");
			
			ImGui::LogButtons();

			//HelpMarker("You can also call ImGui::LogText() to output directly to the log without a visual output.");
			ImGui::Text("You can also call ImGui::LogText() to output directly to the log without a visual output.");

			if (ImGui::Button("Copy \"Hello, world!\" to clipboard"))
			{
				ImGui::LogToClipboard();
				ImGui::LogText("Hello, world!");
				ImGui::LogFinish();
			}
			ImGui::TreePop();
		}
	}

	//IMGUI_DEMO_MARKER("Window options");
	if (ImGui::CollapsingHeader("Window options"))
	{
		if (ImGui::BeginTable("split", 3))
		{
			ImGui::TableNextColumn(); ImGui::Checkbox("No titlebar", &no_titlebar);
			ImGui::TableNextColumn(); ImGui::Checkbox("No scrollbar", &no_scrollbar);
			ImGui::TableNextColumn(); ImGui::Checkbox("No menu", &no_menu);
			ImGui::TableNextColumn(); ImGui::Checkbox("No move", &no_move);
			ImGui::TableNextColumn(); ImGui::Checkbox("No resize", &no_resize);
			ImGui::TableNextColumn(); ImGui::Checkbox("No collapse", &no_collapse);
			ImGui::TableNextColumn(); ImGui::Checkbox("No close", &no_close);
			ImGui::TableNextColumn(); ImGui::Checkbox("No nav", &no_nav);
			ImGui::TableNextColumn(); ImGui::Checkbox("No background", &no_background);
			ImGui::TableNextColumn(); ImGui::Checkbox("No bring to front", &no_bring_to_front);
			ImGui::TableNextColumn(); ImGui::Checkbox("Unsaved document", &unsaved_document);
			ImGui::EndTable();
		}
	}

	// All demo contents
	//ShowDemoWindowWidgets();
	//ShowDemoWindowLayout();
	//ShowDemoWindowPopups();
	//ShowDemoWindowTables();
	ShowWindowMisc();

	// End of ShowDemoWindow()
	ImGui::PopItemWidth();
	ImGui::End();
}

static void ShowWindowMisc()
{
	
	//IMGUI_DEMO_MARKER("Inputs, Navigation & Focus");
	if (ImGui::CollapsingHeader("Inputs, Navigation & Focus"))
	{
		ImGuiIO& io = ImGui::GetIO();

		// Display ImGuiIO output flags
		//IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Output");
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Output"))
		{
			ImGui::Text("io.WantCaptureMouse: %d", io.WantCaptureMouse);
			ImGui::Text("io.WantCaptureMouseUnlessPopupClose: %d", io.WantCaptureMouseUnlessPopupClose);
			ImGui::Text("io.WantCaptureKeyboard: %d", io.WantCaptureKeyboard);
			ImGui::Text("io.WantTextInput: %d", io.WantTextInput);
			ImGui::Text("io.WantSetMousePos: %d", io.WantSetMousePos);
			ImGui::Text("io.NavActive: %d, io.NavVisible: %d", io.NavActive, io.NavVisible);
			ImGui::TreePop();
		}

		// Display Mouse state
		//IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Mouse State");
		if (ImGui::TreeNode("Mouse State"))
		{
			if (ImGui::IsMousePosValid())
				ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
			else
				ImGui::Text("Mouse pos: <INVALID>");
			ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);

			int count = IM_ARRAYSIZE(io.MouseDown);
			ImGui::Text("Mouse down:");         for (int i = 0; i < count; i++) if (ImGui::IsMouseDown(i)) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
			ImGui::Text("Mouse clicked:");      for (int i = 0; i < count; i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d (%d)", i, ImGui::GetMouseClickedCount(i)); }
			ImGui::Text("Mouse released:");     for (int i = 0; i < count; i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
			ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
			ImGui::Text("Pen Pressure: %.1f", io.PenPressure); // Note: currently unused
			ImGui::TreePop();
		}

		// Display Keyboard/Mouse state
		//IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Keyboard, Gamepad & Navigation State");
		if (ImGui::TreeNode("Keyboard, Gamepad & Navigation State"))
		{
			// We iterate both legacy native range and named ImGuiKey ranges, which is a little odd but this allow displaying the data for old/new backends.
			// User code should never have to go through such hoops: old code may use native keycodes, new code may use ImGuiKey codes.
#ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
			struct funcs { static bool IsLegacyNativeDupe(ImGuiKey) { return false; } };
			const ImGuiKey key_first = ImGuiKey_NamedKey_BEGIN;
#else
			struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exists in the array
			const ImGuiKey key_first = 0;
			//ImGui::Text("Legacy raw:");       for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++) { if (io.KeysDown[key]) { ImGui::SameLine(); ImGui::Text("\"%s\" %d", ImGui::GetKeyName(key), key); } }
#endif
			//ImGui::Text("Keys down:");          for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++) { if (funcs::IsLegacyNativeDupe(key)) continue; if (ImGui::IsKeyDown(key)) { ImGui::SameLine(); ImGui::Text("\"%s\" %d (%.02f secs)", ImGui::GetKeyName(key), key, ImGui::GetKeyData(key)->DownDuration); } }
			ImGui::Text("Keys pressed:");       for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++) { if (funcs::IsLegacyNativeDupe(key)) continue; if (ImGui::IsKeyPressed(key)) { ImGui::SameLine(); ImGui::Text("\"%s\" %d", ImGui::GetKeyName(key), key); } }
			ImGui::Text("Keys released:");      for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++) { if (funcs::IsLegacyNativeDupe(key)) continue; if (ImGui::IsKeyReleased(key)) { ImGui::SameLine(); ImGui::Text("\"%s\" %d", ImGui::GetKeyName(key), key); } }
			ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
			ImGui::Text("Chars queue:");        for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.
			ImGui::Text("NavInputs down:");     for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputs[i] > 0.0f) { ImGui::SameLine(); ImGui::Text("[%d] %.2f (%.02f secs)", i, io.NavInputs[i], io.NavInputsDownDuration[i]); }
			ImGui::Text("NavInputs pressed:");  for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputsDownDuration[i] == 0.0f) { ImGui::SameLine(); ImGui::Text("[%d]", i); }

			// Draw an arbitrary US keyboard layout to visualize translated keys
			{
				const ImVec2 key_size = ImVec2(35.0f, 35.0f);
				const float  key_rounding = 3.0f;
				const ImVec2 key_face_size = ImVec2(25.0f, 25.0f);
				const ImVec2 key_face_pos = ImVec2(5.0f, 3.0f);
				const float  key_face_rounding = 2.0f;
				const ImVec2 key_label_pos = ImVec2(7.0f, 4.0f);
				const ImVec2 key_step = ImVec2(key_size.x - 1.0f, key_size.y - 1.0f);
				const float  key_row_offset = 9.0f;

				ImVec2 board_min = ImGui::GetCursorScreenPos();
				ImVec2 board_max = ImVec2(board_min.x + 3 * key_step.x + 2 * key_row_offset + 10.0f, board_min.y + 3 * key_step.y + 10.0f);
				ImVec2 start_pos = ImVec2(board_min.x + 5.0f - key_step.x, board_min.y);

				struct KeyLayoutData { int Row, Col; const char* Label; ImGuiKey Key; };
				const KeyLayoutData keys_to_display[] =
				{
					{ 0, 0, "", ImGuiKey_Tab },      { 0, 1, "Q", ImGuiKey_Q }, { 0, 2, "W", ImGuiKey_W }, { 0, 3, "E", ImGuiKey_E }, { 0, 4, "R", ImGuiKey_R },
					{ 1, 0, "", ImGuiKey_CapsLock }, { 1, 1, "A", ImGuiKey_A }, { 1, 2, "S", ImGuiKey_S }, { 1, 3, "D", ImGuiKey_D }, { 1, 4, "F", ImGuiKey_F },
					{ 2, 0, "", ImGuiKey_LeftShift },{ 2, 1, "Z", ImGuiKey_Z }, { 2, 2, "X", ImGuiKey_X }, { 2, 3, "C", ImGuiKey_C }, { 2, 4, "V", ImGuiKey_V }
				};

				// Elements rendered manually via ImDrawList API are not clipped automatically.
				// While not strictly necessary, here IsItemVisible() is used to avoid rendering these shapes when they are out of view.
				ImGui::Dummy(ImVec2(board_max.x - board_min.x, board_max.y - board_min.y));
				if (ImGui::IsItemVisible())
				{
					ImDrawList* draw_list = ImGui::GetWindowDrawList();
					draw_list->PushClipRect(board_min, board_max, true);
					for (int n = 0; n < IM_ARRAYSIZE(keys_to_display); n++)
					{
						const KeyLayoutData* key_data = &keys_to_display[n];
						ImVec2 key_min = ImVec2(start_pos.x + key_data->Col * key_step.x + key_data->Row * key_row_offset, start_pos.y + key_data->Row * key_step.y);
						ImVec2 key_max = ImVec2(key_min.x + key_size.x, key_min.y + key_size.y);
						draw_list->AddRectFilled(key_min, key_max, IM_COL32(204, 204, 204, 255), key_rounding);
						draw_list->AddRect(key_min, key_max, IM_COL32(24, 24, 24, 255), key_rounding);
						ImVec2 face_min = ImVec2(key_min.x + key_face_pos.x, key_min.y + key_face_pos.y);
						ImVec2 face_max = ImVec2(face_min.x + key_face_size.x, face_min.y + key_face_size.y);
						draw_list->AddRect(face_min, face_max, IM_COL32(193, 193, 193, 255), key_face_rounding, ImDrawFlags_None, 2.0f);
						draw_list->AddRectFilled(face_min, face_max, IM_COL32(252, 252, 252, 255), key_face_rounding);
						ImVec2 label_min = ImVec2(key_min.x + key_label_pos.x, key_min.y + key_label_pos.y);
						draw_list->AddText(label_min, IM_COL32(64, 64, 64, 255), key_data->Label);
						if (ImGui::IsKeyDown(key_data->Key))
							draw_list->AddRectFilled(key_min, key_max, IM_COL32(255, 0, 0, 128), key_rounding);
					}
					draw_list->PopClipRect();
				}
			}
			ImGui::TreePop();
		}
		
		//IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Dragging");
		if (ImGui::TreeNode("Dragging"))
		{
			ImGui::TextWrapped("You can use ImGui::GetMouseDragDelta(0) to query for the dragged amount on any widget.");
			for (int button = 0; button < 3; button++)
			{
				ImGui::Text("IsMouseDragging(%d):", button);
				ImGui::Text("  w/ default threshold: %d,", ImGui::IsMouseDragging(button));
				ImGui::Text("  w/ zero threshold: %d,", ImGui::IsMouseDragging(button, 0.0f));
				ImGui::Text("  w/ large threshold: %d,", ImGui::IsMouseDragging(button, 20.0f));
			}

			ImGui::Button("Drag Me");
			if (ImGui::IsItemActive())
				ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f); // Draw a line between the button and the mouse cursor

			// Drag operations gets "unlocked" when the mouse has moved past a certain threshold
			// (the default threshold is stored in io.MouseDragThreshold). You can request a lower or higher
			// threshold using the second parameter of IsMouseDragging() and GetMouseDragDelta().
			ImVec2 value_raw = ImGui::GetMouseDragDelta(0, 0.0f);
			ImVec2 value_with_lock_threshold = ImGui::GetMouseDragDelta(0);
			ImVec2 mouse_delta = io.MouseDelta;
			ImGui::Text("GetMouseDragDelta(0):");
			ImGui::Text("  w/ default threshold: (%.1f, %.1f)", value_with_lock_threshold.x, value_with_lock_threshold.y);
			ImGui::Text("  w/ zero threshold: (%.1f, %.1f)", value_raw.x, value_raw.y);
			ImGui::Text("io.MouseDelta: (%.1f, %.1f)", mouse_delta.x, mouse_delta.y);
			ImGui::TreePop();
		}

		//IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Mouse cursors");
		if (ImGui::TreeNode("Mouse cursors"))
		{
			const char* mouse_cursors_names[] = { "Arrow", "TextInput", "ResizeAll", "ResizeNS", "ResizeEW", "ResizeNESW", "ResizeNWSE", "Hand", "NotAllowed" };
			IM_ASSERT(IM_ARRAYSIZE(mouse_cursors_names) == ImGuiMouseCursor_COUNT);

			ImGuiMouseCursor current = ImGui::GetMouseCursor();
			ImGui::Text("Current mouse cursor = %d: %s", current, mouse_cursors_names[current]);
			ImGui::Text("Hover to see mouse cursors:");
			/*
			ImGui::SameLine(); HelpMarker(
				"Your application can render a different mouse cursor based on what ImGui::GetMouseCursor() returns. "
				"If software cursor rendering (io.MouseDrawCursor) is set ImGui will draw the right cursor for you, "
				"otherwise your backend needs to handle it.");
			*/
			ImGui::Text("Your application can render a different mouse cursor based on what ImGui::GetMouseCursor() returns. ");
			ImGui::Text("If software cursor rendering (io.MouseDrawCursor) is set ImGui will draw the right cursor for you, ");
			ImGui::Text("otherwise your backend needs to handle it.");
			
			for (int i = 0; i < ImGuiMouseCursor_COUNT; i++)
			{
				char label[32];
				//sprintf(label, "Mouse cursor %d: %s", i, mouse_cursors_names[i]);
				ImGui::Bullet(); ImGui::Selectable(label, false);
				if (ImGui::IsItemHovered())
					ImGui::SetMouseCursor(i);
			}
			ImGui::TreePop();
		}
	}
}

static void ShowAboutWindow() {}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Simple overlay / ShowExampleAppSimpleOverlay()
//-----------------------------------------------------------------------------

// Demonstrate creating a simple static window with no decoration
// + a context-menu to choose which corner of the screen to use.
static void ShowStatsOverlay(bool* p_open)
{
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (corner != -1)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Example: Simple overlay", p_open, window_flags))
	{
		//IMGUI_DEMO_MARKER("Examples/Simple Overlay");
		ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
		ImGui::Separator();
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			if (p_open && ImGui::MenuItem("Close")) *p_open = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void my_display_code()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (show_demo_window)
		//ImGui::ShowDemoWindow(&show_demo_window);

	/*
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
	*/

	/*
	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
	*/
	ShowAppMainMenuBar();
}

void renderScene(void) 
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	my_display_code();

	/*
	ImGui::Begin("DIOS AYUDA"); 
	ImGui::Text("This is some useful text.");
	ImGui::End();
	*/

	// Rendering
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	//glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	// glClearColor(0, 0, 0, 1);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT); // For some unknown reason this clears all
	glColor3f(1.0f, 0.5f, 0.25f);
	
	// Aplication Code
	int i = 0;
	for (auto const& x : shapes) {
		x->render();
		cout << "render shape " << i << endl;
		i++;
	}
	
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	glutPostRedisplay();
}

void changeSize(int w, int h) 
{	
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)w, (float)h);

	if (h == 0)
		return;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	glOrtho(0, w, 0, h, -1, 1);

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	width = w;
	height = h;
}

int main(int argc, char** argv)
{
	// init GLUT and create Window
	glutInit(&argc, argv);
#ifdef __FREEGLUT_EXT_H__
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	//glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Better Paint - Proyecto 1 - Leonardo Mendoza");

	cout << "Hi Hi :,D" << endl;

	/*
	glViewport(0, 0, width, height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	*/

	// Aplication Code
	shared_ptr<CLine> l1 = make_shared <CLine>(1, 1, 1);
	shared_ptr<CLine> l2 = make_shared <CLine>(1, 0, 0);
	
	l1->set(0, 0, 100, 100);
	l2->set(100, 100, 200, 100);

	shapes.push_back(l1);
	shapes.push_back(l2);


	// register callbacks
	glutDisplayFunc(renderScene);
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	// FIXME: Consider reworking this example to install our own GLUT funcs + forward calls ImGui_ImplGLUT_XXX ones, instead of using ImGui_ImplGLUT_InstallFuncs().
	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	//ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");
	//ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// Here is our new entry in the main function
	glutReshapeFunc(changeSize);
	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	cout << "Bye Bye..." << endl;
	return 0;
}