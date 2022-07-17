#pragma once
#include <iostream>
#include <list>
#include <string>
#include <memory>

#include <shape.h>
#include <line.h>

// FreeGlut
#include "./GL/freeglut.h"
// ImGui
#include "./imgui/imgui.h"
//#include "imgui/imgui_draw.cpp"
#include "./imgui/backends/imgui_impl_glut.h"
#include "./imgui/backends/imgui_impl_opengl2.h"

// Windows config
int width = 1280, height = 720;

// Our state
static bool show_config_window = true; //Set true when Press "0"
static bool show_another_window = true;
static ImVec4 clear_color = ImVec4(0.01f, 0.17f, 0.31f, 1.00f);

// Shapes
list <shared_ptr<CShape>> shapes;
shared_ptr<CShape> current_shape;

// Modes
static int DrawingMode = 0;
static int FigureClicked = 0;
static bool isDrawingFigure = false;
static bool isLastVertex = false;

static shared_ptr <Vertex2D> lastClickedVertex;

static bool isborder = true;
static bool isfill = true;
// TO DO: Change to Modify Selected Figure colors
static ImVec4 new_border_color = ImVec4(0.1f, 0.17f, 0.13f, 1.00f);
static ImVec4 new_fill_color = ImVec4(0.01f, 0.71f, 0.31f, 1.00f);

// Figures
shared_ptr<CLine> new_line;


int drawingMode() {
	return DrawingMode;
}


