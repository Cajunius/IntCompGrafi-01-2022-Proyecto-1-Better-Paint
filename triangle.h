#pragma once

#include "shape.h"
#include "vertex2d.h"

class CTriangle : public CShape
{
private:
	int ID = 5;
	shared_ptr<Vertex2D> v0;
	shared_ptr<Vertex2D> v1;
	shared_ptr<Vertex2D> v2;

public:


	CTriangle(float r, float g, float b) : CShape(r, g, b)
	{
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 3;
		v0 = make_shared <Vertex2D>(0, 0);
		v1 = make_shared <Vertex2D>(0, 0);
		v2 = make_shared <Vertex2D>(0, 0);
		toogleDrawing();
	}

	CTriangle(float r, float g, float b, float a) : CShape(r, g, b, a)
	{
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 3;
		v0 = make_shared <Vertex2D>(0, 0);
		v1 = make_shared <Vertex2D>(0, 0);
		v2 = make_shared <Vertex2D>(0, 0);
		toogleDrawing();
	}

	CTriangle(ImVec4 border) : CShape(border)
	{
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 3;
		v0 = make_shared <Vertex2D>(0, 0);
		v1 = make_shared <Vertex2D>(0, 0);
		v2 = make_shared <Vertex2D>(0, 0);
		toogleDrawing();
	}

	CTriangle(ImVec4 border, ImVec4 fill) : CShape(border, fill)
	{
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 3;
		v0 = make_shared <Vertex2D>(0, 0);
		v1 = make_shared <Vertex2D>(0, 0);
		v2 = make_shared <Vertex2D>(0, 0);
		toogleDrawing();
	}

	~CTriangle()
	{
		cout << "Se destruyo un triangulo" << endl;
	}

	void set(int x0, int y0, int x1, int y1, int x2, int y2)
	{
		v0->XY(x0, y0);
		v1->XY(x1, y1);
		v2->XY(x2, y2);

		vertex = MAX_VERTEXS;
		selected_vertex = vertex - 1;
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
		selected_vertex = vid;
	}

	void drawborder(bool drawingMode) {
		

		if (drawingMode == 0) // Hardware Mode
		{
			
			setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

				glLineWidth(borderWidth);

				glBegin(GL_LINE_LOOP);
				//glBegin(GL_LINE_STRIP);

				//glBegin(GL_LINE);

				glVertex2i(v0->X(), v0->Y());
				glVertex2i(v1->X(), v1->Y());

				glVertex2i(v1->X(), v1->Y());
				glVertex2i(v2->X(), v2->Y());

				glVertex2i(v2->X(), v2->Y());
				glVertex2i(v0->X(), v0->Y());

				/*
				glVertex2i(v1->X(), v1->Y());
				glVertex2i(v0->X(), v0->Y());
				
				glVertex2i(v2->X(), v2->Y());
				glVertex2i(v1->X(), v1->Y());

				glVertex2i(v0->X(), v0->Y());
				glVertex2i(v2->X(), v2->Y());
				*/

				glEnd();
				glFlush();
			//}
			
		}

		else { // Software Mode

			setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

			// user putpixel de aquí en adelante... con Bresenham
			drawline(v0->X(), v0->Y(), v1->X(), v1->Y());
			drawline(v1->X(), v1->Y(), v2->X(), v2->Y());
			drawline(v2->X(), v2->Y(), v0->X(), v0->Y());
		}
	}

	void drawfill(bool drawingMode) {


		if (drawingMode == 0) // Hardware Mode
		{
			setColor4(fill_color[0], fill_color[1], fill_color[2], fill_color[3]);

			glBegin(GL_TRIANGLES);
			glVertex2i(v0->X(), v0->Y());
			glVertex2i(v1->X(), v1->Y());
			glVertex2i(v2->X(), v2->Y());
			glEnd();
			glFlush();
		}

		else { // Software Mode

			setColor4(fill_color[0], fill_color[1], fill_color[2], fill_color[3]);

			// user putpixel de aquí en adelante... con Bresenham

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
			glEnd();
			glFlush();
		}

		else { // Software Mode

			setColor4(vertex_color.x, vertex_color.y, vertex_color.z, vertex_color.w);

			// user putpixel de aquí en adelante... con Bresenham
			putPixel(v0->X(), v0->Y(), vertexSize);
			putPixel(v1->X(), v1->Y(), vertexSize);
			putPixel(v2->X(), v2->Y(), vertexSize);
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
				//cout << "drawBorder" << endl;
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
