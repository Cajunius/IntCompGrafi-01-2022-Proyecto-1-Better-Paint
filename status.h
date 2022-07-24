#pragma once
#include <iostream>
#include <list>
#include <string>
#include <memory>

#include <shape.h>
#include <line.h>
//#include <triangle.h>

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
static ImVec4 vertex_color = ImVec4(1.f, 0.01f, 0.1f, 1.00f);
static ImVec4 vertex_color_unseted = ImVec4(0.01f, 0.1f, 1.0f, 1.00f);
static ImVec4 vertex_color_selected = ImVec4(0.5f, 1.0f, 1.0f, 1.00f);

// Auxiliar objects
shared_ptr<CLine> aux;

// Shapes
list <shared_ptr<CShape>> shapes;
shared_ptr<CShape> current_shape;

// Modes
static int DrawingMode = 0;
static int FigureClicked = 0;
static bool isDrawingFigure = false;
static bool isLastVertex = false;

const int vertex_per_figure[] = { 1, 2, 2, 3, 2, 3, 20, 1, 0, NULL };

// Events
int clicks_on_buffer;
shared_ptr<Vertex2D> buffer[20];

static shared_ptr <Vertex2D> lastClickedVertex;

static bool isborder = true;
static bool isfill = true;
// TO DO: Change to Modify Selected Figure colors
static ImVec4 new_border_color = ImVec4(1.f, 1.f, 1.f, 1.f);
static ImVec4 new_fill_color = ImVec4(0.01f, 0.71f, 0.31f, 0.5f);



int drawingMode() {
	return DrawingMode;
}

void INIT_STATUS() {

	// INIT BUFFER
	clicks_on_buffer = 0;
	for (int i = 0; i < 20; i++) {
		buffer[i] = make_shared <Vertex2D>(0, 0);
	}
	aux = make_shared <CLine>(new_border_color);
}


