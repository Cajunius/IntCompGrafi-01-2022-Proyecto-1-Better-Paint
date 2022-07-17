#pragma once
#include <iostream>


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
		cout << "Se destruyo un Vertex2D" << endl;
	}

	int X() {
		return x;
	}
	int Y() {
		return y;
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
