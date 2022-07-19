#pragma once

#include "shape.h"
#include "vertex2d.h"

class CLine : public CShape
{
private:
	//int x0, y0, x1, y1;
	int ID = 1;
	shared_ptr<Vertex2D> v0;
	shared_ptr<Vertex2D> v1;

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

	void plotPixel(int x1, int y1, int x2, int y2, int dx, int dy, int decide)
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
					// putPixel(x1, y1, RED);
					pk = pk + 2 * dy;
				}
				else
				{
					//(y1,x1) is passed in xt
				    putPixel(y1, x1, borderWidthS);
					pk = pk + 2 * dy;
				}
			}
			else
			{
				y1 < y2 ? y1++ : y1--;
				if (decide == 0)
				{

					putPixel(x1, y1, borderWidthS);
				}
				else
				{
					putPixel(y1, x1, borderWidthS);
				}
				pk = pk + 2 * dy - 2 * dx;
			}
		}
	}

	void drawline(int x0, int y0, int x1, int y1)
	{
		int dx, dy, pk;
		//cin cout
		dx = abs(x1 - x0);
		dy = abs(y1 - y0);
		//If slope is less than one
		if (dx > dy)
		{
			//passing argument as 0 to plot(x,y)
			plotPixel(x0, y0, x1, y1, dx, dy, 0);
		}
		//if slope is greater than or equal to 1
		else
		{
			//passing argument as 1 to plot (y,x)
			plotPixel(y0, x0, y1, x1, dy, dx, 1);
		}
		// getch();
	}
	void drawborder(bool drawingMode) {
		if (drawingMode == 0) // Hardware Mode
		{

			// despliegas la línea con el algoritmo de bresenham
			setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

			// user putPixel de aquí en adelante... con Bresenham
			glBegin(GL_LINES);

			//glVertex2i(x0, y0);
			//glVertex2i(x1, y1);

			/*
			// Iterate trough vertexs
			int i = 0;
			for (auto const& v : VERTEXS) {
				if (i == MAX_VERTEXS) {
					break;
				}
				glVertex2i(v->X(), v->Y());
				i++;
			}
			*/
			glVertex2i(v0->X(), v0->Y());
			glVertex2i(v1->X(), v1->Y());
			glEnd();
			glFlush();
		}

		else { // Software Mode

			// despliegas la línea con el algoritmo de bresenham
			setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

			// user putPixel de aquí en adelante... con Bresenham
			drawline(v0->X(), v0->Y(), v1->X(), v1->Y());
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

			
		}
	}

	bool onClick(int x, int y) 
	{
		// determinar la distancia del click a la línea
		// si es mejor a un umbral (e.g. 3 píxeles) entonces
		// retornas true
		return false;
	}

	void onMove(int x, int y)
	{
	}

};
