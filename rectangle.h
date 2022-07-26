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

	shared_ptr<Vertex2D> LastVertex() {
		selected_vertex = 3;
		return v3;
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
			glVertex2i(v3->X(), v3->Y());
			glEnd();
			glFlush();
		}

		else { // Software Mode
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

			int minX = min(v0->X(), v1->X());
			minX = min(minX, v2->X());
			minX = min(minX, v3->X());

			int maxX = max(v0->X(), v1->X());
			maxX = max(maxX, v2->X());
			maxX = max(maxX, v3->X());

			int minY = min(v0->Y(), v1->Y());
			minY = min(minY, v2->Y());
			minY = min(minY, v3->Y());

			int maxY = max(v0->Y(), v1->Y());
			maxY = max(maxY, v2->Y());
			maxY = max(maxY, v3->Y());

			v0->XY(minX, minY);
			v1->XY(minX, maxY);
			v2->XY(maxX, maxY);
			v3->XY(maxX, minY);

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
		// determinar la distancia del click al rectangulo
		// si es mejor a un umbral (e.g. 3 píxeles) entonces
		// retornas true
		bool isClicked = false;
		if (!isSelected) {
			shared_ptr<Vertex2D> polygon[] = { v0, v1, v2, v3};
			if (isInside(polygon, 4, x, y))
			{
				cout << "RECTANGLE SELECTED" << endl;
				isClicked = true;
			}
		}
		isSelected = isClicked;
		return isClicked;
	}

	void onMove(int _x, int _y)
	{
		cout << "RECTANGLE MOVED" << endl;
		v0->Xs(v0->X() + _x);
		v0->Ys(v0->Y() + _y);
		v1->Xs(v1->X() + _x);
		v1->Ys(v1->Y() + _y);
		v2->Xs(v2->X() + _x);
		v2->Ys(v2->Y() + _y);
		v3->Xs(v3->X() + _x);
		v3->Ys(v3->Y() + _y);
	}


	shared_ptr<Vertex2D> selectedVertex(int _x, int _y) {
		shared_ptr<Vertex2D> aux = NULL;

		int d = distancei(v0->X(), v0->Y(), _x, _y);
		if (d <= click_dist_tolerance)
		{
			cout << "VERTEX 1 SELECTED" << endl;
			aux = v0;
			selected_vertex = 0;
		}
		else {
			d = distancei(v1->X(), v1->Y(), _x, _y);
			if (d <= click_dist_tolerance)
			{
				cout << "VERTEX 2 SELECTED" << endl;
				aux = v1;
				selected_vertex = 1;
			}
			else {
				d = distancei(v2->X(), v2->Y(), _x, _y);
				if (d <= click_dist_tolerance)
				{
					cout << "VERTEX 3 SELECTED" << endl;
					aux = v2;
					selected_vertex = 2;
				}
				else {
					d = distancei(v3->X(), v3->Y(), _x, _y);
					if (d <= click_dist_tolerance)
					{
						cout << "VERTEX 4 SELECTED" << endl;
						aux = v3;
						selected_vertex = 3;
					}
				}
			}
		}

		return aux;
	}

	void MoveVertex(shared_ptr<Vertex2D> aux, int _x, int _y)
	{
		cout << "RECTANGLE VERTEX MOVED" << endl;
		aux->Xs(aux->X() + _x);
		aux->Ys(aux->Y() + _y);
		// Ordering Vertex taking into account the figure:
		/*
			v1 -> v2
			|	  |
			v0 <- v3
		*/
		if (selected_vertex == 0) {
			v1->XY(v0->X(), v2->Y());
			v3->XY(v2->X(), v0->Y());
		}
		else {
			if (selected_vertex == 1) {
				v0->XY(v1->X(), v3->Y());
				v2->XY(v3->X(), v1->Y());
			}
			else {
				if (selected_vertex == 2) {
					v1->XY(v0->X(), v2->Y());
					v3->XY(v2->X(), v0->Y());
				}
				else {
					if (selected_vertex == 3) {
						v0->XY(v1->X(), v3->Y());
						v2->XY(v3->X(), v1->Y());
					}
				}
			}
		}
	}

	void printShape(ofstream& MyFile) {
		if (drawFill) {
			MyFile << "FILLED_RECTANGLE " << v0->X() << " " << v0->Y() << " " << v3->X() << " " << v3->Y() << " " << border_color[0] << " " << border_color[1] << " " << border_color[2] << " " << fill_color[0] << " " << fill_color[1] << " " << fill_color[2] << endl;
			cout << "FILLED_RECTANGLE " << v0->X() << " " << v0->Y() << " " << v3->X() << " " << v3->Y() << " " << border_color[0] << " " << border_color[1] << " " << border_color[2] << " " << fill_color[0] << " " << fill_color[1] << " " << fill_color[2] << endl;
		}
		else {
			MyFile << "RECTANGLE " << v0->X() << " " << v0->Y() << " " << v3->X() << " " << v3->Y() << " " << border_color[0] << " " << border_color[1] << " " << border_color[2] << endl;
			cout << "RECTANGLE " << v0->X() << " " << v0->Y() << " " << v3->X() << " " << v3->Y() << " " << border_color[0] << " " << border_color[1] << " " << border_color[2] << endl;
		}
	}
};
