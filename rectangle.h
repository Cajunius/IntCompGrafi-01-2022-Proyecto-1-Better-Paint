#pragma once

#include <colors.h>
#include "shape.h"
#include "vertex2d.h"

class CRectangle : public CShape
{
private:
	int ID = 4;
	shared_ptr<Vertex2D> v0;
	shared_ptr<Vertex2D> v1;
	shared_ptr<Vertex2D> v2;
	shared_ptr<Vertex2D> v3;

public:


	CRectangle(float r, float g, float b) : CShape(r, g, b)
	{
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 4;
		v0 = make_shared <Vertex2D>(0, 0);
		v1 = make_shared <Vertex2D>(0, 0);
		v2 = make_shared <Vertex2D>(0, 0);
		v3 = make_shared <Vertex2D>(0, 0);
		toogleDrawing();
	}
	CRectangle(ImVec4 border) : CShape(border)
	{
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 4;
		v0 = make_shared <Vertex2D>(0, 0);
		v1 = make_shared <Vertex2D>(0, 0);
		v2 = make_shared <Vertex2D>(0, 0);
		v3 = make_shared <Vertex2D>(0, 0);
		toogleDrawing();
	}

	CRectangle(ImVec4 border, ImVec4 fill) : CShape(border, fill)
	{
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 4;
		v0 = make_shared <Vertex2D>(0, 0);
		v1 = make_shared <Vertex2D>(0, 0);
		v2 = make_shared <Vertex2D>(0, 0);
		v3 = make_shared <Vertex2D>(0, 0);
		toogleDrawing();
	}

	~CRectangle()
	{
		cout << "Se destruyo un rectangulo" << endl;
	}

	void set(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
	{

		// Ordering Vertex taking into account the figure:
		/*
			v1 -> v2
			|	  |
			v0 <- v3
		*/

		int minX = min(x0, x1);
			minX = min(minX, x2);
			minX = min(minX, x3);

		int maxX = max(x0, x1);
			maxX = max(maxX, x2);
			maxX = max(maxX, x3);

		int minY = min(y0, y1);
			minY = min(minY, y2);
			minY = min(minY, y3);

		int maxY = max(y0, y1);
			maxY = max(maxY, y2);
			maxY = max(maxY, y3);

		/*
		v0->XY(x0, y0);
		v1->XY(x1, y1);
		v2->XY(x2, y2);
		v3->XY(x3, y3);
		*/

		v0->XY(minX, minY);
		v1->XY(minX, maxY);
		v2->XY(maxX, maxY);
		v3->XY(maxX, minY);

		vertex = MAX_VERTEXS;
		selected_vertex = vertex - 1;
	}

	void set(int x0, int y0, int x1, int y1)
	{
		set(x0, y0, x0, y1, x1, y1, x1, y0);
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
		if (vid == 2)
			v2 = _v;
		if (vid == 3)
			v3 = _v;
		selected_vertex = vid;
	}

	void drawborder(bool drawingMode) {


		if (drawingMode == 0) // Hardware Mode
		{
			setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

			glLineWidth(borderWidth);
			glBegin(GL_LINE_LOOP);

			glVertex2i(v0->X(), v0->Y());
			glVertex2i(v1->X(), v1->Y());

			glVertex2i(v1->X(), v1->Y());
			glVertex2i(v2->X(), v2->Y());

			glVertex2i(v2->X(), v2->Y());
			glVertex2i(v3->X(), v3->Y());

			//glVertex2i(v3->X(), v3->Y());
			//glVertex2i(v0->X(), v0->Y());

			glEnd();
			glFlush();
		}

		else { // Software Mode

			setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

			// user putpixel de aquí en adelante... con Bresenham
			drawline(v0->X(), v0->Y(), v1->X(), v1->Y());
			drawline(v1->X(), v1->Y(), v2->X(), v2->Y());
			drawline(v2->X(), v2->Y(), v3->X(), v3->Y());
			drawline(v3->X(), v3->Y(), v0->X(), v0->Y());

		}
	}

	void drawfill(bool drawingMode) {


		if (drawingMode == 0) // Hardware Mode
		{
			setColor4(fill_color[0], fill_color[1], fill_color[2], fill_color[3]);

			//glBegin(GL_QUADS);
			glBegin(GL_QUADS);
			// glBegin(GL_POLYGON);
			glVertex2i(v0->X(), v0->Y());
			glVertex2i(v1->X(), v1->Y());
			glVertex2i(v2->X(), v2->Y());
			glVertex2i(v3->X(), v3->Y());
			glEnd();
			glFlush();
		}

		else { // Software Mode

			setColor4(fill_color[0], fill_color[1], fill_color[2], fill_color[3]);

			// user putpixel de aquí en adelante... 
			int x0 = v0->X(); // Bottom Left Corner
			int y0 = v0->Y();

			int x3 = v3->X(); // Upper Right Corner
			int y2 = v2->Y();

			while (y0 <= y2) {
				int xx = x0;
				while (xx <= x3){
					putPixel(xx, y0, 1);
					xx++;
				}
				//drawline(x0, y0, x3, y0); //...con Bresenham
				y0++;
			}
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
			glVertex2i(v2->X(), v2->Y());
			glVertex2i(v3->X(), v3->Y());
			glEnd();
			glFlush();
		}

		else { // Software Mode
			setColor4(vertex_color.x, vertex_color.y, vertex_color.z, vertex_color.w);

			// user putpixel de aquí en adelante... con Bresenham
			putPixel(v0->X(), v0->Y(), vertexSize);
			putPixel(v1->X(), v1->Y(), vertexSize);
			putPixel(v2->X(), v2->Y(), vertexSize);
			putPixel(v3->X(), v3->Y(), vertexSize);
		}
	}

	void render(bool drawingMode)
	{
		//cout << "vertex: " << vertex << ", MAX_VERTEXS: " << MAX_VERTEXS << endl;
		if (vertex == MAX_VERTEXS) { //Es dibujable
			// cout << DrawingMode << endl;

			if (drawFill) {
				drawfill(drawingMode);
			}

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
		// determinar la distancia del click a la línea
		// si es mejor a un umbral (e.g. 3 píxeles) entonces
		// retornas true
		return false;
	}

	void onMove(int x, int y)
	{
	}

};
