#pragma once
#include <math.h>
#include <stdio.h>
#include <vertex2d.h>

//#include <shape.h> // For putPixel
// Function to calculate distance
float distancef(int x1, int y1, int x2, int y2)
{
    // Calculating distance
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

float distancef(shared_ptr <Vertex2D> v, shared_ptr <Vertex2D> w)
{
	// Calculating distance
	return distancef(v->X(), v->Y(), w->X(), w->Y());
}

float distancei(int x1, int y1, int x2, int y2)
{
    // Calculating distance
    return round(distancef(x1, y1, x2, y2));
}

//-----------------------------------------------------------

double computeBinominal(int n, int k)
{

	double value = 1.0;

	for (int i = 1; i <= k; i++)
	{

		value = value * ((n + 1 - i) / i);
	}

	if (n == k) {
		value = 1;
	}

	return value;
}

//-----------------------------------------------------------

/*
float length_squared(shared_ptr <Vertex2D> v, shared_ptr <Vertex2D> w) {
	return (distancef)
}
*/

float dot(shared_ptr <Vertex2D> v, shared_ptr <Vertex2D> w) {
	return (v->X() * w->X()) + (v->Y() * w->Y());
}

float minimum_distance(shared_ptr <Vertex2D> v, shared_ptr <Vertex2D> w, shared_ptr <Vertex2D> p) {
	// Return minimum distance between line segment vw and point p
	//const float l2 = length_squared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
	const float l2 = pow(distancef(v, w),2);
	if (l2 == 0.0) return distancef(p, v);   // v == w case
	// Consider the line extending the segment, parameterized as v + t (w - v).
	// We find projection of point p onto the line. 
	// It falls where t = [(p-v) . (w-v)] / |w-v|^2
	// We clamp t from [0,1] to handle points outside the segment vw.
	const float t = max((float)0.0, min((float)1.0, dot(p->resta(v), w->resta(v)) / l2));
	const shared_ptr <Vertex2D> projection = ((w->resta(v))->multiplicar(t))->suma(v);  // Projection falls on the segment
	return distancef(p, projection);
}