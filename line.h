#pragma once

#include "shape.h"
#include "vertex2d.h"

class CLine : public CShape
{
private:
	int x0, y0, x1, y1;
	int ID = 1;
	

public:
	CLine(float r, float g, float b) : CShape(r, g, b)
	{
		vertex = 0;
		MAX_VERTEXS = 2;
		toogleDrawing();
	}
	CLine(ImVec4 border) : CShape(border)
	{
		vertex = 0;
		MAX_VERTEXS = 2;
		toogleDrawing();
	}

	~CLine()
	{
		cout << "Se destruyo una linea" << endl;
	}

	void set(int x0, int y0, int x1, int y1)
	{
		this->x0 = x0;
		this->y0 = y0;
		this->x1 = x1;
		this->y1 = y1;

		shared_ptr<Vertex2D> v1 = make_shared <Vertex2D>(x0, y0);
		shared_ptr<Vertex2D> v2 = make_shared <Vertex2D>(x1, y1);

		VERTEXS.push_back(v1);
		VERTEXS.push_back(v2);
	}


	bool addVertex(shared_ptr <Vertex2D> v, bool isLastVertex) {
		VERTEXS.push_back(v);
		vertex = +1;
		if (vertex == MAX_VERTEXS - 1) {
			isLastVertex = true;
		}
		return isLastVertex;
	}

	void render(bool drawingMode)
	{

		// cout << DrawingMode << endl;
		if (drawingMode == 0) // Hardware Mode
		{
		
		// despliegas la línea con el algoritmo de bresenham
		setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

		// user putpixel de aquí en adelante... con Bresenham
		glBegin(GL_LINES);

		//glVertex2i(x0, y0);
		//glVertex2i(x1, y1);

		// Iterate trough vertexs
		int i = 0;
		for (auto const& v : VERTEXS) {
			if (i == MAX_VERTEXS) {
				break;
			}
			glVertex2i(v->X(), v->Y());
			i++;
		}

		glEnd();
		}
		
		else { // Software Mode

			// despliegas la línea con el algoritmo de bresenham
			setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

			// user putpixel de aquí en adelante... con Bresenham
			glBegin(GL_LINES);
			
			glEnd();
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
