#pragma once
#include <iostream>
#include <list>
#include <string>
#include <memory>

#include <colors.h>
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


