#pragma once

#include <colors.h>
#include "shape.h"
#include "vertex2d.h"
#include "utils.h"

class CElipse : public CShape
{
private:
	//int ID = 3;
	shared_ptr<Vertex2D> center;
	shared_ptr<Vertex2D> radiusX;
	shared_ptr<Vertex2D> radiusY;
	int rx, ry;

public:


	CElipse(float r, float g, float b) : CShape(r, g, b)
	{
		ID = 3;
		click_dist_tolerance = 0; //px
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 3;
		center = make_shared <Vertex2D>(0, 0);
		radiusX = make_shared <Vertex2D>(0, 0);
		radiusY = make_shared <Vertex2D>(0, 0);
		rx = ry = 0;
		toogleDrawing();
	}
	CElipse(ImVec4 border) : CShape(border)
	{
		ID = 3;
		click_dist_tolerance = 0; //px
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 3;
		center = make_shared <Vertex2D>(0, 0);
		radiusX = make_shared <Vertex2D>(0, 0);
		radiusY = make_shared <Vertex2D>(0, 0);
		rx = ry = 0;
		toogleDrawing();
	}

	CElipse(ImVec4 border, ImVec4 fill) : CShape(border, fill)
	{
		ID = 3;
		click_dist_tolerance = 0; //px
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 3;
		center = make_shared <Vertex2D>(0, 0);
		radiusX = make_shared <Vertex2D>(0, 0);
		radiusY = make_shared <Vertex2D>(0, 0);
		rx = ry = 0;
		toogleDrawing();
	}

	~CElipse()
	{
		cout << "Se destruyo una elipse" << endl;
	}

	shared_ptr<Vertex2D> LastVertex() {
		selected_vertex = 2;
		return radiusY;
	}

	void set(int x0, int y0, int x1, int y1, int x2, int y2)
	{
		center->XY(x0, y0);
		radiusX->XY(x1, y1);
		radiusY->XY(x2, y2);
		rx = abs(center->X() - radiusX->X());
		ry = abs(center->Y() - radiusY->Y());
		//r = distancei(x0, y0, x1, y1);
		//cout << "radius: " << r << endl;

		vertex = MAX_VERTEXS;
		selected_vertex = vertex - 1;
	}
	void set(int x0, int y0, int _rx, int _ry)
	{
		center->XY(x0, y0);
		radiusX->XY(x0 + _rx, y0);
		radiusY->XY(x0, y0 + _ry);
		rx = _rx;
		ry = _ry;

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
			radiusX = _v;
		if (vid == 2)
			radiusY = _v;
		selected_vertex = vid;
	}

	void drawElipse(int x, int y, int xc, int yc)
	{

		putPixel(x + xc, y + yc, borderWidthS);
		putPixel(-x + xc, y + yc, borderWidthS);
		putPixel(x + xc, -y + yc, borderWidthS);
		putPixel(-x + xc, -y + yc, borderWidthS);

	}

	void drawborder(bool drawingMode) {


		//if (drawingMode == 0) // Hardware Mode
		//{
			//Elipse Can Only be Draw by SoftwareMode 
		//}

		//else { // Software Mode
		setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

		// user putPixel de aquí en adelante... con Bresenham

		int xc = center->X(), yc = center->Y();
		int dx, dy, d1, d2, x, y;
		int rx2 = rx * rx, ry2 = ry * ry;
		x = 0;
		y = ry;

		// Initial decision parameter of region 1
		
		/*d1 = (ry * ry)
			- (rx * rx * ry)
			+ (0.25 * rx * rx);*/
		d1 = ry* ((4 * ry)-4 * (rx2)) + (rx2); //It gives me erroneous displays with this //SOLVER
		//dx = 2 * ry * ry * x;
		dx = 2 * ry2 * x;
		//dy = 2 * rx * rx * y;
		dy = 2 * rx2 * y;

		// For region 1
		while (dx < dy) {

			// Print points based on 4-way symmetry
			drawElipse(x, y, xc, yc);

			// Checking and updating value of
			// decision parameter based on algorithm
			if (d1 < 0) {
				x++;
				//dx = dx + (2 * ry * ry);
				dx = dx + (2 * ry2);
				//d1 = d1 + dx + (ry * ry);
				d1 = d1 + 4*(dx + (ry2));
			}
			else {
				x++;
				y--;
				//dx = dx + (2 * ry * ry);
				dx = dx + (2 * ry2);
				//dy = dy - (2 * rx * rx);
				dy = dy - (2 * rx2);
				//d1 = d1 + dx - dy + (ry * ry);
				d1 = d1 + 4 * (dx - dy + (ry2));
			}
		}

		// Decision parameter of region 2
		/*d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5)))
			+ ((rx * rx) * ((y - 1) * (y - 1)))
			- (rx * rx * ry * ry);*/

		d2 = (ry2 * ((4 * x * x) + (4 * x) + 1)) + (rx2 * ((4 * y * y) - (8 * y) + 4)) - (4 * rx2 * ry2);

		// Plotting points of region 2
		while (y >= 0) {

			// printing points based on 4-way symmetry
			drawElipse(x, y, xc, yc);

			// Checking and updating parameter
			// value based on algorithm
			if (d2 > 0) {
				y--;
				//dy = dy - (2 * rx * rx);
				dy = dy - (2 * rx2);
				//d2 = d2 + (rx * rx) - dy;
				d2 = d2 + 4*((rx2) - dy);
			}
			else {
				y--;
				x++;
				//dx = dx + (2 * ry * ry);
				dx = dx + (2 * ry2);
				//dy = dy - (2 * rx * rx);
				dy = dy - (2 * rx2);
				//d2 = d2 + dx - dy + (rx * rx);
				d2 = d2 + 4 * (dx - dy + (rx2));
			}
		}
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

		int ry2 = ry * ry;
		int rx2 = rx * rx;
		int ry2rx2 = ry2 * rx2;
		int x0 = rx;
		int dx = 0;

		// do the horizontal diameter
		for (int x = -rx; x <= rx; x++)
			putPixel(center->X() + x, center->Y(), 1);

		// now do both halves at the same time, away from the diameter
		for (int y = 1; y <= ry; y++)
		{
			int x1 = x0 - (dx - 1);  // try slopes of dx - 1 or more
			for (; x1 > 0; x1--)
				if (x1 * x1 * ry2 + y * y * rx2 <= ry2rx2)
					break;
			dx = x0 - x1;  // current approximation of the slope
			x0 = x1;

			for (int x = -x0; x <= x0; x++)
			{
				putPixel(center->X() + x, center->Y() - y, 1);
				putPixel(center->X() + x, center->Y() + y, 1);
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
			glVertex2i(center->X(), center->Y());
			glVertex2i(radiusX->X(), center->Y());
			glVertex2i(center->X(), radiusY->Y());
			glEnd();
			glFlush();
		}

		else { // Software Mode
			// user putPixel de aquí en adelante... con Bresenham
			putPixel(center->X(), center->Y(), vertexSize);
			putPixel(radiusX->X(), center->Y(), vertexSize);
			putPixel(center->X(), radiusY->Y(), vertexSize);

		}
	}

	void render(bool drawingMode)
	{
		//cout << "vertex: " << vertex << ", MAX_VERTEXS: " << MAX_VERTEXS << endl;
		if (vertex == MAX_VERTEXS) { //Es dibujable
			// cout << DrawingMode << endl;
			rx = abs(center->X() - radiusX->X());
			ry = abs(center->Y() - radiusY->Y());

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
		// determinar la distancia del click a la elipse
		// si es mejor a un umbral (e.g. 3 píxeles) entonces
		// retornas true

		bool isClicked = false;
		if (!isSelected) {
			int d;
			d = (pow((x - center->X()), 2) / pow(rx, 2)) + (pow((y - center->Y()), 2) / pow(ry, 2));
			//(rx > ry){}
			//else{}
			//int a = max(rx, ry), b = min(rx, ry);
			// checking the equation of
			// ellipse with the given point
			//int d = (pow((x - center->X()), 2) / pow(a, 2)) + (pow((y - center->Y()), 2) / pow(b, 2));
			//cout << "elipse distance: " << d << endl;
			if (d <= 0 + click_dist_tolerance)
			{
				cout << "ELIPSE SELECTED" << endl;
				isClicked = true;
			}
		}
		isSelected = isClicked;
		return isClicked;
	}

	void onMove(int _x, int _y)
	{
		cout << "ELIPSE MOVED" << endl;
		center->Xs(center->X() + _x);
		center->Ys(center->Y() + _y);
		radiusX->Xs(radiusX->X() + _x);
		radiusX->Ys(radiusX->Y() + _y);
		radiusY->Xs(radiusY->X() + _x);
		radiusY->Ys(radiusY->Y() + _y);
	}

	shared_ptr<Vertex2D> selectedVertex(int _x, int _y) {
		shared_ptr<Vertex2D> aux = NULL;

		int d = distancei(center->X(), center->Y(), _x, _y);
		if (d <= click_dist_tolerance)
		{
			cout << "VERTEX CENTER SELECTED" << endl;
			aux = center;
			selected_vertex = 0;
		}
		else {
			d = distancei(radiusX->X(), radiusX->Y(), _x, _y);
			if (d <= click_dist_tolerance)
			{
				cout << "VERTEX RADIUSX SELECTED" << endl;
				aux = radiusX;
				selected_vertex = 1;
			}
			else {
				d = distancei(radiusY->X(), radiusY->Y(), _x, _y);
				if (d <= click_dist_tolerance)
				{
					cout << "VERTEX RADIUSY SELECTED" << endl;
					aux = radiusY;
					selected_vertex = 2;
				}
			}
		}

		return aux;
	}

	void MoveVertex(shared_ptr<Vertex2D> aux, int _x, int _y)
	{
		cout << "ELIPSE VERTEX MOVED" << endl;
		aux->Xs(aux->X() + _x);
		aux->Ys(aux->Y() + _y);
	}

	void printShape(ofstream& MyFile) {
		if (drawFill) {
			MyFile << "FILLED_ELLIPSE " << center->X() << " " << center->Y() << " " << rx << " " << ry << " " << border_color[0] << " " << border_color[1] << " " << border_color[2] << " " << fill_color[0] << " " << fill_color[1] << " " << fill_color[2] << endl;
			cout << "FILLED_ELLIPSE " << center->X() << " " << center->Y() << " " << rx << " " << ry << " " << border_color[0] << " " << border_color[1] << " " << border_color[2] << " " << fill_color[0] << " " << fill_color[1] << " " << fill_color[2] << endl; 
		}
		else {
			MyFile << "ELLIPSE " << center->X() << " " << center->Y() << " " << rx << " " << ry << " " << border_color[0] << " " << border_color[1] << " " << border_color[2] << endl;
			cout << "ELLIPSE " << center->X() << " " << center->Y() << " " << rx << " " << ry << " " << border_color[0] << " " << border_color[1] << " " << border_color[2] << endl;
		}
	}
};
