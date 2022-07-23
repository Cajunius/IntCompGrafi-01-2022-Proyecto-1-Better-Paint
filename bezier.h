#pragma once
#pragma once

#include "shape.h"
#include "vertex2d.h"
#include "utils.h"

class CBezier : public CShape
{
private:
	//int x0, y0, x1, y1;
	int ID = 6;
	int grade = -1;
	shared_ptr<Vertex2D> v[20];
	ImVec4 vertex_color = ImVec4(1.f, 0.01f, 0.1f, 1.00f);

public:


	CBezier(float r, float g, float b) : CShape(r, g, b)
	{
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 20;
		for (int i = 0; i < MAX_VERTEXS; i++) {
			v[i] = make_shared <Vertex2D>(0, 0);
		}
		toogleDrawing();
	}
	CBezier(ImVec4 border) : CShape(border)
	{
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 20;
		for (int i = 0; i < MAX_VERTEXS; i++) {
			v[i] = make_shared <Vertex2D>(0, 0);
		}
		toogleDrawing();
	}
	CBezier(ImVec4 border, ImVec4 fill) : CShape(border, fill)
	{
		vertex = 0;
		selected_vertex = vertex;
		MAX_VERTEXS = 20;
		for (int i = 0; i < MAX_VERTEXS; i++) {
			v[i] = make_shared <Vertex2D>(0, 0);
		}
		toogleDrawing();
	}

	~CBezier()
	{
		cout << "Se destruyo una curva de bezier" << endl;
	}

	void setgrade(int n) {
		grade = n;
	}

	void set(int x0, int y0, int x1, int y1)
	{
		vertex = MAX_VERTEXS;
		selected_vertex = vertex - 1;
	}


	bool addVertex(shared_ptr <Vertex2D> _v, bool isLastVertex) {
		VERTEXS.push_back(_v);
		vertex = vertex +1;
		selected_vertex = vertex - 1;
		setgrade(vertex - 1);
		v[vertex] = _v;
		if (vertex == MAX_VERTEXS - 1) {
			isLastVertex = true;
		}
		return isLastVertex;
	}

	void addVertex(int _x, int _y) {
		vertex = vertex +1;
		selected_vertex = vertex - 1;
		setgrade(vertex - 1);
		v[grade]->XY(_x, _y);
		VERTEXS.push_back(v[grade]);
		
	}

	void modifyVertex(int vid, shared_ptr <Vertex2D> _v) {
		if (vid < 0 or vid > MAX_VERTEXS) {
			//ERROR
			return;
		}
		v[vid] = _v;
		selected_vertex = vid;
	}

	list <shared_ptr<Vertex2D>> computeNVertexBasierCurve2D(shared_ptr<Vertex2D>* _V, int n)
	{

		list <shared_ptr<Vertex2D>> bCurve;

		//cout << "n :" << n << "\n";

		for (double t = 0.0; t <= 1.0; t += 0.02)
		{

			double bCurveXt{ 0 };
			double bCurveYt{ 0 };
			shared_ptr<Vertex2D> _v = make_shared <Vertex2D>(0, 0);

			for (int i = 0; i <= n; ++i)
			{
				
				bCurveXt += computeBinominal(n, i) * std::pow((1 - t), (n - i)) * std::pow(t, i) * _V[i]->X();
				bCurveYt += computeBinominal(n, i) * std::pow((1 - t), (n - i)) * std::pow(t, i) * _V[i]->Y();
				//cout << " t= "<< t<< " i=" << i << " bCurveXt=" << bCurveXt << " = " << computeBinominal(n, i)  << " * " << std::pow((1 - t), (n - i))  << " * " << std::pow(t, i) << " * " << _V[i]->X() << std::endl;
			}

			_v->XY(round(bCurveXt), round(bCurveYt));
			bCurve.push_back(_v);
		}

		return bCurve;
	}

	//Draws line between control points
	void drawfill(bool drawingMode) {
		if (drawingMode == 0) // Hardware Mode
		{
			setColor4(fill_color[0], fill_color[1], fill_color[2], fill_color[3]);

			// Control Line
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i <= grade; i++) {
				glVertex2i(v[i]->X(), v[i]->Y());
			}
			glEnd();
			glFlush();
		}

		else { // Software Mode

			// despliegas la línea con el algoritmo de bresenham
			setColor4(fill_color[0], fill_color[1], fill_color[2], fill_color[3]);

			// user putPixel de aquí en adelante... con Bresenham

			for (int i = 0; i < grade; i++) {
				if(i+1 <= grade)
					drawline(v[i]->X(), v[i]->Y(), v[i+1]->X(), v[i+1]->Y(), borderWidthS);
			}
			

		}
	}

	void drawborder(bool drawingMode) {

		list <shared_ptr<Vertex2D>> bCurve = computeNVertexBasierCurve2D(v, grade);
		
		if (drawingMode == 0) // Hardware Mode
		{
			setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);
			
			// Control Line
			glBegin(GL_LINE_STRIP);
			for (auto const& _v : bCurve) {
				glVertex2i(_v->X(), _v->Y());
			}
			glVertex2i(v[grade]->X(), v[grade]->Y()); //Last
			glEnd();
			glFlush();
		}

		else { // Software Mode

			// despliegas la línea con el algoritmo de bresenham
			setColor4(border_color[0], border_color[1], border_color[2], border_color[3]);

			// user putPixel de aquí en adelante... con Bresenham
			int i = 0;
			list <shared_ptr<Vertex2D>>::iterator it;
			it = bCurve.begin();
			while (i + 1 <= bCurve.size() - 1) {
				list <shared_ptr<Vertex2D>>::iterator itprev = it;
				//if (i + 1 <= bCurve.size() - 1) {
					it++;
					drawline((*itprev)->X(), (*itprev)->Y(), (*it)->X(), (*it)->Y(), borderWidthS);
				//}
				i++;
			}
			// Last
			//it = bCurve.end();
			drawline((*it)->X(), (*it)->Y(), v[grade]->X(), v[grade]->Y(), borderWidthS);

			/*
			for (auto const& _v : bCurve) {
				if (i + 1 <= bCurve.size()-1)
					drawline(_v->X(), _v->Y(), _v[i + 1]->X(), _v[i + 1]->Y(), borderWidthS);
				i++;
			}
			*/
			
		}
	}

	void drawvertex(bool drawingMode) {


		if (drawingMode == 0) // Hardware Mode
		{

			setColor4(vertex_color.x, vertex_color.y, vertex_color.z, vertex_color.w);

			glPointSize(vertexSize);
			glBegin(GL_POINTS);
			for (int i = 0; i <= grade; i++) {
				glVertex2i(v[i]->X(), v[i]->Y());
			}
			glEnd();
			glFlush();
		}

		else { // Software Mode
			setColor4(vertex_color.x, vertex_color.y, vertex_color.z, vertex_color.w);

			// user putpixel de aquí en adelante... con Bresenham
			for (int i = 0; i <= grade; i++) {
				putPixel(v[i]->X(), v[i]->Y(), vertexSize);
			}
		}
	}


	void render(bool drawingMode)
	{
		//cout << "vertex: " << vertex << ", MAX_VERTEXS: " << MAX_VERTEXS << endl;
		if (grade >= 0) { //Es dibujable
			// cout << DrawingMode << endl;

			if (drawBorder) {
				drawborder(drawingMode);
			}

			// Line between control points
			if (drawFill) {
				drawfill(drawingMode);
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
