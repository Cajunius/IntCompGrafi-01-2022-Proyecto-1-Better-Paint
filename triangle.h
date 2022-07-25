#pragma once

#include <colors.h>
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

		//Order respect to origin distance
		int d0 = distancei(0, 0, x0, y0);
		int d1 = distancei(0, 0, x1, y1);
		int d2 = distancei(0, 0, x2, y2);

		if (d0 <= d1 and d0 <= d2) {
			v0->XY(x0, y0);
			if (d1 <= d2) {
				v1->XY(x1, y1);
				v2->XY(x2, y2);
			}
			else {
				v2->XY(x1, y1);
				v1->XY(x2, y2);
			}
		}
		else
		{
			if (d1 <= d0 and d1 <= d2) {
				v0->XY(x1, y1);
				if (d0 <= d2) {
					v1->XY(x0, y0);
					v2->XY(x2, y2);
				}
				else {
					v2->XY(x0, y0);
					v1->XY(x2, y2);
				}
			}
			else {
				v0->XY(x2, y2);
				if (d0 <= d1) {
					v1->XY(x0, y0);
					v2->XY(x1, y1);
				}
				else {
					v2->XY(x0, y0);
					v1->XY(x1, y1);
				}
			}
			
		}

		/*
		v0->XY(x0, y0);
		v1->XY(x1, y1);
		v2->XY(x2, y2);
		*/

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
			int d1 = distancei(v0->X(), v0->Y(), v1->X(), v1->Y());
			int d2 = distancei(v1->X(), v1->Y(), v2->X(), v2->Y());
			int d3 = distancei( v2->X(), v2->Y(), v0->X(), v0->Y());

			int tx, ty, vx, vy;
			if (((d1 < d2) or (d1 == d2)) and ((d1 < d2) or (d1 == d2))) {
				tx = v0->X();
				ty = v0->Y();
				vx = (v1->X() - v0->X()) / d1;
				vy = (v1->Y() - v0->Y()) / d1;
				int counter = 0;
				while (counter < d1) {
					drawline(v2->X(), v2->Y(), tx, ty, 1);
					//drawing a line from point(v2->X(),v2->Y()) to point(tx,ty).
					tx = tx + vx;
					ty = ty + vy;
					counter = counter + 1;
				}
			}
			else {
				if ((d2 < d3) or (d2 == d3)) {
					tx = v1->X();
					ty = v1->Y();
					vx = (v2->X() - v1->X()) / d2;
					vy = (v2->Y() - v1->Y()) / d2;
					int counter = 0;
					while (counter < d2) {
						drawline(v0->X(), v0->Y(), tx, ty, 1);
						tx = tx + vx;
						ty = ty + vy;
						counter = counter + 1;
					}
				}
				else {
					tx = v2->X();
					ty = v2->Y();
					vx = (v0->X() - v2->X()) / d3;
					vy = (v0->Y() - v2->Y()) / d3;
					int counter = 0;
					while (counter < d3) {
						drawline(v1->X(), v1->Y(), tx, ty, 1);
						tx = tx + vx;
						ty = ty + vy;
						counter = counter + 1;
					}
				}
			}
		}
	}

	void drawvertex(bool drawingMode) {

		if (!isSelected) {
			vertex_color[0] = vertex_color_original[0];
			vertex_color[1] = vertex_color_original[1];
			vertex_color[2] = vertex_color_original[2];
			vertex_color[3] = vertex_color_original[3];
		}
		else {
			vertex_color[0] = vertex_color_selected.x;
			vertex_color[1] = vertex_color_selected.y;
			vertex_color[2] = vertex_color_selected.z;
			vertex_color[3] = vertex_color_selected.w;
		}

		setColor4(vertex_color[0], vertex_color[1], vertex_color[2], vertex_color[3]);

		if (drawingMode == 0) // Hardware Mode
		{
			glPointSize(vertexSize);

			glBegin(GL_POINTS);		
			glVertex2i(v0->X(), v0->Y());
			glVertex2i(v1->X(), v1->Y());
			glVertex2i(v2->X(), v2->Y());
			glEnd();
			glFlush();
		}

		else { // Software Mode
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
		// determinar la distancia del click aL triangulo
		// si es mejor a un umbral (e.g. 3 píxeles) entonces
		// retornas true
		bool isClicked = false;
		if (!isSelected) {
			shared_ptr<Vertex2D> polygon[] = { v0, v1, v2 };
			if (isInside(polygon, 3, x, y))
			{
				cout << "TRIANGLE SELECTED" << endl;
				isClicked = true;
			}
		}
		isSelected = isClicked;
		return isClicked;
	}

	void onMove(int x, int y)
	{
		
	}

};
