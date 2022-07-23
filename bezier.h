#pragma once
#pragma once

#include "shape.h"
#include "vertex2d.h"
#include "utils.h"

#include <iostream> // for std::out
#include <vector> // for the points vector
#include <valarray> // for Point2d

class CBezier : public CShape
{
private:
	//int x0, y0, x1, y1;
	int ID = 6;
	int grade = -1;
	shared_ptr<Vertex2D> v[20];
	list<shared_ptr<Vertex2D>> vL;
	ImVec4 vertex_color = ImVec4(1.f, 0.01f, 0.1f, 1.00f);

public:

	double PRECISION = 0.05;

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
		vL.push_back(_v);
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
		vL.push_back(v[grade]);
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

		for (double t = 0.0; t <= 1.0; t += PRECISION)
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

	double calcPRECISION(int _grade, list<shared_ptr<Vertex2D>> _vL) {
		
		int i = 0;
		list <shared_ptr<Vertex2D>>::iterator it;
		it = _vL.begin();
		double polygonlong = 0;
		while (i + 1 <= _vL.size() - 1) {
			list <shared_ptr<Vertex2D>>::iterator itprev = it;
			//if (i + 1 <= bCurve.size() - 1) {
			it++;
			polygonlong += distancef((*itprev)->X(), (*itprev)->Y(), (*it)->X(), (*it)->Y());
			//}
			i++;
		}
		// Last
		//it = bCurve.end();
		//distancef((*it)->X(), (*it)->Y(), v[grade]->X(), v[grade]->Y(), borderWidthS);
		if (polygonlong <=0)
			polygonlong = 100;
		cout << "[" << _grade << "] polygonlong: " << polygonlong << endl;
		//double _PRECISION = (((1 / polygonlong) * (1 / _grade)));
		double _PRECISION = ((1.0 / polygonlong));
		int _2grade = 2 * _grade;
		if (_grade <= 0)
			_grade = 1;
		cout << "[" << _grade << "] 1_PRECISION: " << _PRECISION << endl;
		double modifier = (1.0 / _2grade);
		cout << "[" << _grade << "] modifier: " << modifier << endl;

		_PRECISION = ((1.0 / (polygonlong*modifier)));
		cout << "[" << _grade << "] 2_PRECISION: " << _PRECISION << endl;
		if (_PRECISION == 0 or _PRECISION >= 1)
			_PRECISION = 0.05;
		cout << "[" << _grade << "] 3_PRECISION: " << _PRECISION << endl;
		return _PRECISION;
	}

	using Real = double; // better define Real and not just use double

	// A way to have Point2d from an existing std include
	// good for job interviews or samples like this one that are shared
	using Point2d = std::valarray<Real>;

	// a vector of points
	using Point2dVec = std::vector<Point2d>;

	const shared_ptr<Vertex2D> Approximate(const Real t, const Real oneMinusT, const shared_ptr<Vertex2D>& pt1, const shared_ptr<Vertex2D>& pt2)
	{
		shared_ptr<Vertex2D> _v = make_shared <Vertex2D>(0, 0);
		_v->XY(pt1->X() * oneMinusT + pt2->X() * t, pt1->Y() * oneMinusT + pt2->Y() * t);
		return _v;
	}

	void GetBezierPoints(list<shared_ptr<Vertex2D>> inputPoints, const Real precision, list<shared_ptr<Vertex2D>>& resultBezierPoints)
	{
		// result
		//resultBezierPoints.reserve(static_cast<size_t>(1 / precision + 1));
		//resultBezierPoints.clear();

		// temp vector, declare and reserve it here (and not inside the loop) to save allocations
		list<shared_ptr<Vertex2D>>  approximatedPoints;
		//approximatedPoints.reserve(inputPoints.size());

		// loop t by precision
		for (Real t = 0; t < 1; t += precision)
		{
			// save (1 - t) now as it is used inside Approximate
			Real oneMinusT = (1 - t);

			approximatedPoints = inputPoints;

			while (approximatedPoints.size() > 1)
			{
				auto pointsIter = approximatedPoints.begin();

				// as we advance the iter inside the loop and use the next value
				// we loop until the prev to the last item on the list
				auto pointsIterEnd = std::prev(approximatedPoints.end());

				// loop using an iterator
				while (pointsIter != pointsIterEnd)
				{
					// save the current iter - Approximate result will be stored here
					// this way we can use a single buffer
					auto updateThisIter = pointsIter;

					// Approximate the current and next points
					const auto& pt1 = *(pointsIter++);
					const auto& pt2 = *(pointsIter);
					const auto approximatedPoint = Approximate(t, oneMinusT, pt1, pt2);

					// store Approximate result in the same buffer
					*updateThisIter = approximatedPoint;
				}

				approximatedPoints.pop_back();
			}

			//finally only one point will be left
			resultBezierPoints.push_back((*approximatedPoints.begin()));
		}
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

		//list <shared_ptr<Vertex2D>> bCurve = computeNVertexBasierCurve2D(v, grade);
		//De Castejau
		//list <shared_ptr<Vertex2D>> bCurve = GetBezierPoints(v, grade);

		list<shared_ptr<Vertex2D>> bCurve;
		//PRECISION = calcPRECISION(grade, vL);

		GetBezierPoints(vL, PRECISION, bCurve);

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
