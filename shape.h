#pragma once
#include <iostream>
#include <list>
#include <colors.h>
#include "vertex2d.h"

using namespace std;


class CShape
{
protected:
	
	
public:
	float border_color[4];
	float fill_color[4];
	float vertex_color[4];
	float vertex_color_original[4];
	bool drawing = false;
	list <shared_ptr<Vertex2D>> VERTEXS;
	int vertex;
	int selected_vertex;
	int MAX_VERTEXS;
	bool drawFill = true;
	bool drawBorder = true;
	bool drawVertex = true;
	bool isSelected = false;
	float borderWidth = 2;
	float borderWidthS = 1;
	float vertexSize = 4;
	int click_dist_tolerance = 10; //px

	CShape(float r, float g, float b)
	{
		border_color[0] = r;
		border_color[1] = g;
		border_color[2] = b;
		border_color[3] = 1.0; //alpha

		fill_color[0] = r;
		fill_color[1] = g;
		fill_color[2] = b;
		fill_color[3] = 0.0;

		vertex_color_original[0] = vertex_color_unselected.x;
		vertex_color_original[1] = vertex_color_unselected.y;
		vertex_color_original[2] = vertex_color_unselected.z;
		vertex_color_original[3] = vertex_color_unselected.w;

		vertex_color[0] = vertex_color_original[0];
		vertex_color[1] = vertex_color_original[1];
		vertex_color[2] = vertex_color_original[2];
		vertex_color[3] = vertex_color_original[3];
	}

	CShape(float r, float g, float b, float a)
	{
		border_color[0] = r;
		border_color[1] = g;
		border_color[2] = b;
		border_color[3] = a; //alpha

		fill_color[0] = r;
		fill_color[1] = g;
		fill_color[2] = b;
		fill_color[3] = 0.0;

		vertex_color_original[0] = vertex_color_unselected.x;
		vertex_color_original[1] = vertex_color_unselected.y;
		vertex_color_original[2] = vertex_color_unselected.z;
		vertex_color_original[3] = vertex_color_unselected.w;

		vertex_color[0] = vertex_color_original[0];
		vertex_color[1] = vertex_color_original[1];
		vertex_color[2] = vertex_color_original[2];
		vertex_color[3] = vertex_color_original[3];
	}

	CShape(ImVec4 border)
	{
		border_color[0] = border.x;
		border_color[1] = border.y;
		border_color[2] = border.z;
		border_color[3] = border.w;

		fill_color[0] = border.x;
		fill_color[1] = border.y;
		fill_color[2] = border.z;
		fill_color[3] = 0.0;

		vertex_color_original[0] = vertex_color_unselected.x;
		vertex_color_original[1] = vertex_color_unselected.y;
		vertex_color_original[2] = vertex_color_unselected.z;
		vertex_color_original[3] = vertex_color_unselected.w;

		vertex_color[0] = vertex_color_original[0];
		vertex_color[1] = vertex_color_original[1];
		vertex_color[2] = vertex_color_original[2];
		vertex_color[3] = vertex_color_original[3];
	}

	CShape(ImVec4 border, ImVec4 fill)
	{
		border_color[0] = border.x;
		border_color[1] = border.y;
		border_color[2] = border.z;
		border_color[3] = border.w;

		fill_color[0] = fill.x;
		fill_color[1] = fill.y;
		fill_color[2] = fill.z;
		fill_color[3] = fill.w;

		vertex_color_original[0] = vertex_color_unselected.x;
		vertex_color_original[1] = vertex_color_unselected.y;
		vertex_color_original[2] = vertex_color_unselected.z;
		vertex_color_original[3] = vertex_color_unselected.w;

		vertex_color[0] = vertex_color_original[0];
		vertex_color[1] = vertex_color_original[1];
		vertex_color[2] = vertex_color_original[2];
		vertex_color[3] = vertex_color_original[3];
	}
	

	virtual ~CShape()
	{
		cout << "Se destruyo un shape" << endl;
	}

	void putPixel(int x, int y)
	{
		glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();
	}
	void putPixel(int x, int y, float width)
	{
		glLineWidth(width);
		glPointSize(width);
		putPixel(x, y);
	}
	void putPixel(shared_ptr<Vertex2D> v)
	{
		int x = v->X();
		int y = v->Y();
		putPixel(x, y);
	}
	void putPixel(shared_ptr<Vertex2D> v, float width)
	{
		//glLineWidth(width);
		//glPointSize(width);
		int x = v->X();
		int y = v->Y();
		putPixel(x, y, width);
	}

	void setColor(float r, float g, float b)
	{
		glColor3f(r, g, b);
	}

	void setColor4(float r, float g, float b, float a)
	{
		glColor4f(r, g, b, a);
	}

	bool toogleDrawing() {
		drawing = !drawing;
		return drawing;
	}

	bool isDrawing() {
		return drawing;
	}

	virtual bool addVertex(shared_ptr <Vertex2D> v, bool isLastVertex) = 0;

	virtual void render(bool drawingMode) = 0;
	
	// recibe el click del mouse y retorna true si efectivamente
	// el objetos fue seleccionado
	virtual bool onClick(int x, int y) = 0;
	
	// este método es invocado solo hacia el objeto "current"
	virtual void onMove(int x, int y) = 0;

	// podríamos responder a los eventos del mouse
	// . todos responden al click, pero solo uno puede
	// . retornar "yo fui seleccionado"
	virtual shared_ptr<Vertex2D> selectedVertex(int _x, int _y) = 0;

	virtual void MoveVertex(shared_ptr<Vertex2D> aux, int _x, int _y) = 0;

	virtual shared_ptr<Vertex2D> LastVertex() = 0;

	virtual void printShape(ofstream &MyFile) = 0;

	// using putPixel
	void plotPixel(int x1, int y1, int x2, int y2, int dx, int dy, int decide, int Width)
	{
		//pk is initial decision making parameter
		//Note:x1&y1,x2&y2, dx&dy values are interchanged
		//and passed in plotPixel function so
		//it can handle both cases when m>1 & m<1
		int pk = 2 * dy - dx;
		for (int i = 0; i <= dx; i++)
		{
			//cout << x1 << "," << y1 << endl;
			//checking either to decrement or increment the value
			//if we have to plot from (0,100) to (100,0)
			x1 < x2 ? x1++ : x1--;
			if (pk < 0)
			{
				//decision value will decide to plot
				//either  x1 or y1 in x's position
				if (decide == 0)
				{
					putPixel(x1, y1, Width);
					pk = pk + 2 * dy;
				}
				else
				{
					//(y1,x1) is passed in xt
					putPixel(y1, x1, Width);
					pk = pk + 2 * dy;
				}
			}
			else
			{
				y1 < y2 ? y1++ : y1--;
				if (decide == 0)
				{

					putPixel(x1, y1, Width);
				}
				else
				{
					putPixel(y1, x1, Width);
				}
				pk = pk + 2 * dy - 2 * dx;
			}
		}
	}

	void plotPixel(int x1, int y1, int x2, int y2, int dx, int dy, int decide) {
		plotPixel(x1, y1, x2, y2, dx, dy, decide, borderWidthS);
	}

	// Draws Line with bresenham algorithm
	void drawline(int x0, int y0, int x1, int y1, int Width)
	{
		int dx, dy, pk;
		//cin cout
		dx = abs(x1 - x0);
		dy = abs(y1 - y0);
		//If slope is less than one
		if (dx > dy)
		{
			//passing argument as 0 to plot(x,y)
			plotPixel(x0, y0, x1, y1, dx, dy, 0, Width);
		}
		//if slope is greater than or equal to 1
		else
		{
			//passing argument as 1 to plot (y,x)
			plotPixel(y0, x0, y1, x1, dy, dx, 1, Width);
		}
		// getch();
	}
	void drawline(int x0, int y0, int x1, int y1) {
		drawline(x0, y0, x1, y1, borderWidthS);
	}
};
