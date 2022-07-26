#pragma once

#include <colors.h>
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

	shared_ptr<Vertex2D> LastVertex() {
		return v1;
	}

	void set(int x0, int y0, int x1, int y1)
	{

		//Order respect to origin distance
		int d0 = distancei(0, 0, x0, y0);
		int d1 = distancei(0, 0, x1, y1);

		if (d0 <= d1) {
			v0->XY(x0, y0);
			v1->XY(x1, y1);
		}
		else
		{
			v0->XY(x1, y1);
			v1->XY(x0, y0);
		}

		/*
		this->x0 = x0;
		this->y0 = y0;
		this->x1 = x1;
		this->y1 = y1;
		*/

		//shared_ptr<Vertex2D> v0 = make_shared <Vertex2D>(x0, y0);
		//shared_ptr<Vertex2D> v1 = make_shared <Vertex2D>(x1, y1);
		/*
		v0->XY(x0, y0);
		v1->XY(x1, y1);
		*/
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

			// despliegas la línea con el algoritmo de bresenham
			setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

			// user putPixel de aquí en adelante... con Bresenham
			drawline(v0->X(), v0->Y(), v1->X(), v1->Y());
		}
	}
	ImVec4 vertex_color_selected = ImVec4(0.5f, 1.0f, 1.0f, 1.00f);
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
			glEnd();
			glFlush();
		}

		else { // Software Mode
			// user putpixel de aquí en adelante... con Bresenham
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
		// determinar la distancia del click a la línea
		// si es mejor a un umbral (e.g. 3 píxeles) entonces
		// retornas true
		bool isClicked = false;
		if(!isSelected){
			shared_ptr<Vertex2D> p = make_shared<Vertex2D>(x, y);
			float d = minimum_distance(v0, v1, p);
			//cout << "line distance: " << d << endl;
			if (d <= click_dist_tolerance)
			{
				cout << "LINE SELECTED"<< endl;
				isClicked = true;
			}
		}
		isSelected = isClicked;
		return isClicked;
	}

	void onMove(int _x, int _y)
	{
		cout << "LINE MOVED" << endl;
		v0->Xs(v0->X() + _x);
		v0->Ys(v0->Y() + _y);
		v1->Xs(v1->X() + _x);
		v1->Ys(v1->Y() + _y);
	}

	shared_ptr<Vertex2D> selectedVertex(int _x, int _y) {
		shared_ptr<Vertex2D> aux = NULL;
		
		int d = distancei(v0->X(), v0->Y(), _x, _y);
		if (d <= click_dist_tolerance)
		{
			cout << "VERTEX 1 SELECTED" << endl;
			aux = v0;
		}
		else {
			d = distancei(v1->X(), v1->Y(), _x, _y);
			if (d <= click_dist_tolerance)
			{
				cout << "VERTEX 2 SELECTED" << endl;
				aux = v1;
			}
		}
		
		return aux;
	}

	void MoveVertex(shared_ptr<Vertex2D> aux, int _x, int _y)
	{
		cout << "LINE VERTEX MOVED" << endl;
		aux->Xs(aux->X() + _x);
		aux->Ys(aux->Y() + _y);
	}
};
