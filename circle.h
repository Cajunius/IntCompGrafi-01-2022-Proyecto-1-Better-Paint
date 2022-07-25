#pragma once

#include <colors.h>
#include "shape.h"
#include "vertex2d.h"
#include "utils.h"

class CCircle : public CShape
{
private:
	int ID = 2;
	shared_ptr<Vertex2D> center;
	shared_ptr<Vertex2D> radius;
	int r;

public:


	CCircle(float r, float g, float b) : CShape(r, g, b)
	{
		click_dist_tolerance = 5; //px
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 2;
		center = make_shared <Vertex2D>(0, 0);
		radius = make_shared <Vertex2D>(0, 0);
		toogleDrawing();
	}
	CCircle(ImVec4 border) : CShape(border)
	{
		click_dist_tolerance = 5; //px
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 2;
		center = make_shared <Vertex2D>(0, 0);
		radius = make_shared <Vertex2D>(0, 0);
		toogleDrawing();
	}

	CCircle(ImVec4 border, ImVec4 fill) : CShape(border, fill)
	{
		click_dist_tolerance = 5; //px
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 2;
		center = make_shared <Vertex2D>(0, 0);
		radius = make_shared <Vertex2D>(0, 0);
		toogleDrawing();
	}

	~CCircle()
	{
		cout << "Se destruyo un circulo" << endl;
	}

	void set(int x0, int y0, int x1, int y1)
	{
		center->XY(x0, y0);
		radius->XY(x1, y1);
		r = distancei(x0, y0, x1, y1);
		//cout << "radius: " << r << endl;

		vertex = MAX_VERTEXS;
		selected_vertex = vertex - 1;
	}
	void set(int x0, int y0, int _r)
	{
		center->XY(x0, y0);
		radius->XY(x0+_r, y0);
		r = _r;

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
			center = _v;
		if (vid == 1)
			radius = _v;
		selected_vertex = vid;
	}

	void drawCircle(int x, int y, int xc, int yc)
	{
		putPixel(x + xc, y + yc, borderWidthS);
		putPixel(-x + xc, y + yc, borderWidthS);
		putPixel(x + xc, -y + yc, borderWidthS);
		putPixel(-x + xc, -y + yc, borderWidthS);
		putPixel(y + xc, x + yc, borderWidthS);
		putPixel(y + xc, -x + yc, borderWidthS);
		putPixel(-y + xc, x + yc, borderWidthS);
		putPixel(-y + xc, -x + yc, borderWidthS);
	}

	void drawborder(bool drawingMode) {


		//if (drawingMode == 0) // Hardware Mode
		//{
			//Circle Can Only be Draw by SoftwareMode 
		//}

		//else { // Software Mode
		setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

		int xc, yc, pk, x, y;
		xc = center->X();
		yc = center->Y();
		pk = 3 - 2 * r;
		x = 0; y = r;
		drawCircle(x, y, xc, yc);
		while (x < y)
		{
			//cout << "x: " << x << ", y: " << y << endl;
			if (pk <= 0)
			{
				pk = pk + (4 * x) + 6;
				drawCircle(++x, y, xc, yc);
			}
			else
			{
				pk = pk + (4 * (x - y)) + 10;
				drawCircle(++x, --y, xc, yc);
			}
		}
		// user putPixel de aquí en adelante... con Bresenham

	//}
	}

	void drawfill(bool drawingMode) {


		//if (drawingMode == 0) // Hardware Mode
		//{
			//Circle Can Only be Draw by SoftwareMode 
		//}

		//else { // Software Mode
		setColor4(fill_color[0], fill_color[1], fill_color[2], fill_color[3]);

		// user putPixel de aquí en adelante...
		int x = 0;
		int y = r;
		int m = 5 - 4 * r;

		//cout << "circle.drawfill" << endl;

		while (x <= y)
		{

			/* Unoptimized: Double Draws Lines
			drawline(center->X() - x, center->Y() - y, center->X() + x, center->Y() - y);
			drawline(center->X() - y, center->Y() - x, center->X() + y, center->Y() - x);
			drawline(center->X() - y, center->Y() + x, center->X() + y, center->Y() + x);
			drawline(center->X() - x, center->Y() + y, center->X() + x, center->Y() + y);

			if (m > 0)
			{
				y--;
				m -= 8 * y;
			}

			x++;
			m += 8 * x + 4;
			*/

			//**/* Breseham isnt the best for horizontal or vertical lines
			drawline(center->X() - y, center->Y() - x, center->X() + y, center->Y() - x);
			drawline(center->X() - y, center->Y() + x, center->X() + y, center->Y() + x);

			if (m > 0)
			{
				drawline(center->X() - x, center->Y() - y, center->X() + x, center->Y() - y);
				drawline(center->X() - x, center->Y() + y, center->X() + x, center->Y() + y);
				y--;
				m -= 8 * y;
			}

			x++;
			m += 8 * x + 4;
			//**/*/
			
			/* Bugged
			for (int xx = center->X() - y; xx <= center->X() + y; xx++)
			{
				putPixel(xx, center->Y() - x);
				putPixel(xx, center->Y() + x);
			}

			if (m > 0)
			{
				for (int xx = center->X() - x; xx <= center->X() + x; xx++)
				{
					putPixel(xx, center->Y() - y);
					putPixel(xx, center->Y() + y);
				}

				y--;
				m -= 8 * y;
			}
			*/
		}

	//}
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
			glVertex2i(center->X(), center->Y());
			glVertex2i(radius->X(), radius->Y());
			glEnd();
			glFlush();
		}

		else { // Software Mode
			// user putPixel de aquí en adelante... con Bresenham
			putPixel(center->X(), center->Y(), vertexSize);
			putPixel(radius->X(), radius->Y(), vertexSize);

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
		// determinar la distancia del click al circulo
		// si es mejor a un umbral (e.g. 3 píxeles) entonces
		// retornas true

		bool isClicked = false;

		if (!isSelected) {
			// Compare radius of circle with distance
			// of its center from given point
			int d = (x - center->X()) * (x - center->X()) +
				(y - center->Y()) * (y - center->Y());
			//cout << "circle distance: " << d << endl;
			if (d <= ((r+ click_dist_tolerance) * (r+click_dist_tolerance))) {
				cout << "CIRCLE SELECTED" << endl;
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
