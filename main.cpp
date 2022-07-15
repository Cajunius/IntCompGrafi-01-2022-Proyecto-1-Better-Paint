
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
#include <string>
#include <memory>
#include <line.h>

using namespace std;

int width = 1280, height = 720;

list <shared_ptr<CShape>> shapes;
shared_ptr<CShape> current_shape;

// Our state
static bool show_config_window = true; //Set true when Press "0"
static bool show_another_window = true;
static ImVec4 clear_color = ImVec4(0.01f, 0.17f, 0.31f, 1.00f);

static int DrawingMode = 0;
static int FigureClicked = 0;
void my_display_code()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_config_window)
		ImGui::ShowDemoWindow(&show_config_window);

	// 2. Show ToolBAR. We use a Begin/End pair to created a named window.
	{
		//static float f = 0.0f;
		//static int counter = 0;

		ImGui::Begin("ToolBar!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is the useful toolbar!.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Config Window", &show_config_window);      // Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &show_another_window);

		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear (background) color", (float*)&clear_color); // Edit 3 floats representing a color

		/*
		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		*/
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		ImGui::Separator();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Mode Selector:");
		ImGui::SameLine();
		ImGui::RadioButton("Hardware", &DrawingMode, 0); ImGui::SameLine();
		ImGui::RadioButton("Software", &DrawingMode, 1);
		//cout << "Modo: " << FigureClicked << endl;

		ImGui::Separator();

		const char* figures[] = { "None", "Line", "Circle", "Elipse", "Rectangle", "Triangle", "Bezier", "DELETE", "EXTRA", NULL };
		ImGui::Text("Figure Selector: %s", figures[FigureClicked]);
		
		const char* figure;
		// Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style.
		for (int i = 0; i < 8; i++)
		{
			figure = *figures + i;
			if (i > 0)
				ImGui::SameLine();
			ImGui::PushID(i);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
			if (ImGui::Button(figures[i])) {
				FigureClicked = i;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
		//cout << "FigureClicked: " << FigureClicked << endl;
		// Check if we need to draw a figure
		if (FigureClicked != 0) {
			// Select which figure to draw
		}
		else {
			// handle Deselect
		}

		static bool border = true;
		static bool fill = true;
		ImGui::Checkbox("Border", &border);
		ImGui::SameLine();
		ImGui::Checkbox("Fill", &fill);
		

		ImGui::Separator();

		static int position = 0; // TO DO: Sustitute this fetching the current figureposition...

		ImGui::Text("Selected Figure:  %d", position); // TO DO: Add Selected Figure Type
		float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		// See 'Demo->Layout->Text Baseline Alignment' for details.
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Figure Position: %d", position);
		ImGui::SameLine();

		// Arrow buttons with Repeater
		if (ImGui::Button("To Back"))
		{
			position = 0;
		}
		ImGui::SameLine(0.0f, spacing);
		ImGui::PushButtonRepeat(true);
		if (ImGui::ArrowButton("##back", ImGuiDir_Left)) { position--; } // TO DO: update with update figure position
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::ArrowButton("##front", ImGuiDir_Right)) { position++; } // TO DO: update with update figure position
		ImGui::PopButtonRepeat();
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("To Front"))
		{
			position = shapes.size();
		}
		
		// TO DO: Change to Modify Selected Figure colors
		static ImVec4 border_color = ImVec4(0.1f, 0.17f, 0.13f, 1.00f);
		static ImVec4 fill_color = ImVec4(0.01f, 0.71f, 0.31f, 1.00f);

		ImGui::ColorEdit4("Border color", (float*)&border_color);
		ImGui::ColorEdit4("Fill color", (float*)&fill_color);

		// Trying to give it color
		ImGui::PushID(11);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7 / 7.0f, 0.8f, 0.8f));
		ImGui::PopStyleColor(3);

		if (ImGui::Button("Delete"))
		{
			//Handle Delete
		}
		ImGui::PopID();

		ImGui::Separator();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Extras:");
		ImGui::SameLine();

		// Trying to give it color
		ImGui::PushID(12);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7 / 7.0f, 0.8f, 0.8f));
		ImGui::PopStyleColor(3);

		if (ImGui::Button("Clear All"))
		{
			//Handle Clear All
		}
		ImGui::PopID();
		ImGui::End();
	}

	//if (show_another_window) {
		//ShowExampleAppMainMenuBar();
	//}
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
		//cout << "render shape " << i << endl;
		i++;
	}

	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	glutPostRedisplay();
}

void onClickCanvas(int button, int state, int x, int y) {
	//Do Somenting
}

void onClick(int button, int state, int x, int y)
{
	//Button: 0 left, 1 middle, 2 right
	//State: 0 press down, 1 up
	ImGuiIO& io = ImGui::GetIO();

	//printf("click button: %d, state: %d, x: %d, y: %d\n", button, state, x, y);

	if (io.WantCaptureMouse) {
		ImGui_ImplGLUT_MouseFunc(button, state, x, y);
	}
	else {
		onClickCanvas(button, state, x, y);
	}
}

void onMotionCanvas(int x, int y) {
	//Do Somenting
	//printf("Motion x: %d, y: %d\n", x, y);
}

void onMotion(int x, int y)
{

	ImGuiIO& io = ImGui::GetIO();
	const int maxRadius = 1; // in pixels

	//printf("Motion x: %d, y: %d\n", x, y);

	if (io.WantCaptureMouse)
	{
		ImGui_ImplGLUT_MotionFunc( x, y);
	}

	else {
		onMotionCanvas(x, y);
	}

}

void onPassiveMotionCanvas(int x, int y) {
	//Do Somenting
}

void onPassiveMotion(int x, int y)
{

	ImGuiIO& io = ImGui::GetIO();
	io.AddMousePosEvent((float)x, (float)y);
	const int maxRadius = 1; // in pixels

	//printf("Passive Motion x: %d, y: %d\n", x, y);

	if (io.WantCaptureMouse) //IDK which event correspond
	{
		//printf("Passive Motion x: %d, y: %d\n", x, y);
		ImGui_ImplGLUT_MotionFunc(x, y);
	}

	else {
		onPassiveMotionCanvas(x, y);
	}

}

void onKeyboardEntryCanvas(unsigned char key, int x, int y) {
	//Do Somenting
}

void onKeyboardEntry(unsigned char key, int x, int y)
{

	ImGuiIO& io = ImGui::GetIO();
	const int maxRadius = 1; // in pixels

	//printf("char_down_func %d '%c'\n", key, key);

	if (io.WantCaptureKeyboard)
	{
		ImGui_ImplGLUT_KeyboardFunc(key, x, y);
	}

	else {
		onKeyboardEntryCanvas(key, x, y);
	}

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
	glutMouseFunc(onClick);
	/*
		glutMotionFunc and glutPassiveMotionFunc set the motion and passive motion callback respectively 
		for the current window. The motion callback for a window is called when the mouse moves within the 
		window while one or more mouse buttons are pressed. The passive motion callback for a window is called
		when the mouse moves within the window while no mouse buttons are pressed.
	*/
	glutMotionFunc(onMotion);
	glutPassiveMotionFunc(onPassiveMotion); //IDK which event correspond

	/*
		glutKeyboardFunc sets the keyboard callback for the current window. When a user types into the window,
		each key press generating an ASCII character will generate a keyboard callback. The key callback 
		parameter is the generated ASCII character. The state of modifier keys such as Shift cannot be 
		determined directly; their only effect will be on the returned ASCII data. The x and y callback 
		parameters indicate the mouse location in window relative coordinates when the key was pressed. 
		When a new window is created, no keyboard callback is initially registered, and ASCII key strokes 
		in the window are ignored. Passing NULL to glutKeyboardFunc disables the generation of keyboard callbacks. 
	*/
	glutKeyboardFunc(onKeyboardEntry);
	glutReshapeFunc(changeSize);

	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	cout << "Bye Bye..." << endl;
	return 0;
}