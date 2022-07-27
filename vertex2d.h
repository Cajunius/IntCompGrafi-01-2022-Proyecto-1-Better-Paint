#pragma once

#include <iostream>
#include <colors.h>

using namespace std;

class Vertex2D
{
protected:
	int x, y;
	bool drawing = false;
	

public:
	Vertex2D(int _x, int  _y)
	{
		x = _x;
		y = _y;
	}

	~Vertex2D()
	{
		//cout << "Se destruyo un Vertex2D" << endl;
	}

	// Get X
	int X() {
		return x;
	}
	// Get Y
	int Y() {
		return y;
	}
	// Set X
	void Xs(int _x) {
		x = _x;
	}
	//Set Y
	void Ys(int _y) {
		y = _y;
	}
	// Set X and Y
	void XY(int _x, int _y) {
		x = _x;
		y = _y;
	}

	shared_ptr<Vertex2D> suma(shared_ptr<Vertex2D> obj) {
		shared_ptr<Vertex2D> res = make_shared<Vertex2D>(0, 0);
		res->x = x + obj->X();
		res->y = y + obj->Y();
		return res;
	}

	shared_ptr<Vertex2D> resta (shared_ptr<Vertex2D> obj) {
		shared_ptr<Vertex2D> res = make_shared<Vertex2D>(0, 0);
		res->x = x - obj->X();
		res->y = y - obj->Y();
		return res;
	}

	shared_ptr<Vertex2D> multiplicar(float a) {
		shared_ptr<Vertex2D> res = make_shared<Vertex2D>(0, 0);
		res->x = x * a;
		res->y = y *a;
		return res;
	}


	Vertex2D operator - (Vertex2D const& obj) {
		Vertex2D res(0,0);
		res.x = x - obj.x;
		res.y = y - obj.y;
		return res;
	}

	Vertex2D operator + (Vertex2D const& obj) {
		Vertex2D res(0, 0);
		res.x = x + obj.x;
		res.y = y + obj.y;
		return res;
	}

	void putPixel(int x, int y)
	{
		glBegin(GL_POINTS);
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

	void render() {}

	// recibe el click del mouse y retorna true si efectivamente
	// el objetos fue seleccionado
	bool onClick(int x, int y) {}

	// este método es invocado solo hacia el objeto "current"
	void onMove(int x, int y) {}

	// podríamos responder a los eventos del mouse
	// . todos responden al click, pero solo uno puede
	// . retornar "yo fui seleccionado"
};
