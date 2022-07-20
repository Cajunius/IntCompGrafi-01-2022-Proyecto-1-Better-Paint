#pragma once

#include "shape.h"
#include "vertex2d.h"
#include "utils.h"

class CLine : public CShape
{
private:
	//int x0, y0, x1, y1;
	int ID = 1;
	shared_ptr<Vertex2D> v0;
	shared_ptr<Vertex2D> v1;
	ImVec4 vertex_color = ImVec4(1.f, 0.01f, 0.1f, 1.00f);

public:


	CLine(float r, float g, float b) : CShape(r, g, b)
	{
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 2;
		v0 = make_shared <Vertex2D>(0, 0);
		v1 = make_shared <Vertex2D>(0, 0);
		toogleDrawing();
	}
	CLine(ImVec4 border) : CShape(border)
	{
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 2;
		v0 = make_shared <Vertex2D>(0, 0);
		v1 = make_shared <Vertex2D>(0, 0);
		toogleDrawing();
	}

	~CLine()
	{
		cout << "Se destruyo una linea" << endl;
	}

	void set(int x0, int y0, int x1, int y1)
	{
		/*
		this->x0 = x0;
		this->y0 = y0;
		this->x1 = x1;
		this->y1 = y1;
		*/

		//shared_ptr<Vertex2D> v0 = make_shared <Vertex2D>(x0, y0);
		//shared_ptr<Vertex2D> v1 = make_shared <Vertex2D>(x1, y1);
		v0->XY(x0, y0);
		v1->XY(x1, y1);

		/*
		// Iterate trough vertexs
		int i = 0;
		for (auto const& v : VERTEXS) {
			if (i == MAX_VERTEXS) {
				break;
			}
			if (i == 0)
				v->XY(x0, y0);
			if (i == 1)
				v->XY(x1, y1);
			i++;
		}
		*/
		// VERTEXS.push_back(v0);
		// VERTEXS.push_back(v1);

		vertex = MAX_VERTEXS;
		selected_vertex = vertex-1;
	}


	bool addVertex(shared_ptr <Vertex2D> v, bool isLastVertex) {
		VERTEXS.push_back(v);
		vertex = +1;
		selected_vertex = vertex - 1;
		if (vertex == MAX_VERTEXS - 1) {
			isLastVertex = true;
		}
		return isLastVertex;
	}
	void modifyVertex(int vid, shared_ptr <Vertex2D> _v) {
		if (vid < 0 or vid > MAX_VERTEXS) {
			//ERROR
			return;
		}

		if (vid == 0)
			v0 = _v;
		if (vid == 1)
			v1 = _v;
		selected_vertex = vid;
	}

	void drawline2(int x0, int y0, int x1, int y1)
	{
		int dx, dy, p, x, y;

		dx = x1 - x0;
		dy = y1 - y0;

		x = x0;
		y = y0;

		p = 2 * dy - dx;

		while (x < x1)
		{
			if (p >= 0)
			{
				putPixel(x, y, borderWidthS);
				y = y + 1;
				p = p + 2 * dy - 2 * dx;
			}
			else
			{
				putPixel(x, y, borderWidthS);
				p = p + 2 * dy;
			}
			x = x + 1;
		}
	}

	void drawborder(bool drawingMode) {
		if (drawingMode == 0) // Hardware Mode
		{
			setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

			glBegin(GL_LINES);
			glVertex2i(v0->X(), v0->Y());
			glVertex2i(v1->X(), v1->Y());
			glEnd();
			glFlush();
		}

		else { // Software Mode

			// despliegas la l�nea con el algoritmo de bresenham
			setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

			// user putPixel de aqu� en adelante... con Bresenham
			drawline(v0->X(), v0->Y(), v1->X(), v1->Y());
		}
	}

	void drawvertex(bool drawingMode) {


		if (drawingMode == 0) // Hardware Mode
		{

			setColor4(vertex_color.x, vertex_color.y, vertex_color.z, vertex_color.w);

			glPointSize(vertexSize);
			glBegin(GL_POINTS);
			glVertex2i(v0->X(), v0->Y());
			glVertex2i(v1->X(), v1->Y());
			glEnd();
			glFlush();
		}

		else { // Software Mode
			setColor4(vertex_color.x, vertex_color.y, vertex_color.z, vertex_color.w);

			// user putpixel de aqu� en adelante... con Bresenham
			putPixel(v0->X(), v0->Y(), vertexSize);
			putPixel(v1->X(), v1->Y(), vertexSize);
		}
	}
	

	void render(bool drawingMode)
	{
		//cout << "vertex: " << vertex << ", MAX_VERTEXS: " << MAX_VERTEXS << endl;
		if (vertex == MAX_VERTEXS) { //Es dibujable
			// cout << DrawingMode << endl;

			if (drawBorder) {
				drawborder(drawingMode);
			}
			if (drawVertex) {
				drawvertex(drawingMode);
			}
			
		}
	}

	bool onClick(int x, int y) 
	{
		// determinar la distancia del click a la l�nea
		// si es mejor a un umbral (e.g. 3 p�xeles) entonces
		// retornas true
		return false;
	}

	void onMove(int x, int y)
	{
	}

};
