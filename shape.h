#pragma once
#include <iostream>
#include <list>
#include "vertex2d.h"

using namespace std;


class CShape
{
protected:
	float border_color[4];
	float fill_color[4];
	bool drawing = false;
	
public:
	list <shared_ptr<Vertex2D>> VERTEXS;
	int vertex;
	int selected_vertex;
	int MAX_VERTEXS;
	bool drawFill = true;
	bool drawBorder = true;
	bool drawVertex = true;
	bool isSelected = false;
	float borderWidth = 2;
	float vertexSize = 4;

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
	}

	CShape(ImVec4 border, ImVec4 fill)
	{
		fill_color[0] = border.x;
		fill_color[1] = border.y;
		fill_color[2] = border.z;
		fill_color[3] = border.w;

		fill_color[0] = fill.x;
		fill_color[1] = fill.y;
		fill_color[2] = fill.z;
		fill_color[3] = fill.w;
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

	void putPixel(shared_ptr<Vertex2D> v)
	{
		glBegin(GL_POINTS);
		int x = v->X();
		int y = v->Y();
		glVertex2i(x, y);
		glEnd();
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
};
