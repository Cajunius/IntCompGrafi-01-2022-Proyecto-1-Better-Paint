#pragma once

#include <colors.h>
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
	// Return minimum distance between line segment vw and shared_ptr<Vertex2D> p
	//const float l2 = length_squared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
	const float l2 = pow(distancef(v, w),2);
	if (l2 == 0.0) return distancef(p, v);   // v == w case
	// Consider the line extending the segment, parameterized as v + t (w - v).
	// We find projection of shared_ptr<Vertex2D> p onto the line. 
	// It falls where t = [(p-v) . (w-v)] / |w-v|^2
	// We clamp t from [0,1] to handle shared_ptr<Vertex2D>s outside the segment vw.
	const float t = max((float)0.0, min((float)1.0, dot(p->resta(v), w->resta(v)) / l2));
	const shared_ptr <Vertex2D> projection = ((w->resta(v))->multiplicar(t))->suma(v);  // Projection falls on the segment
	return distancef(p, projection);
}


// Given three collinear shared_ptr<Vertex2D>s p, q, r, the function checks if
// shared_ptr<Vertex2D> q lies on line segment 'pr'
bool onSegment(shared_ptr<Vertex2D> p, shared_ptr<Vertex2D> q, shared_ptr<Vertex2D> r)
{
    if (q->X() <= max(p->X(), r->X()) && q->X() >= min(p->X(), r->X()) &&
        q->Y() <= max(p->Y(), r->Y()) && q->Y() >= min(p->Y(), r->Y()))
        return true;
    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(shared_ptr<Vertex2D> p, shared_ptr<Vertex2D> q, shared_ptr<Vertex2D> r)
{
    int val = (q->Y() - p->Y()) * (r->X() - q->X()) -
        (q->X() - p->X()) * (r->Y() - q->Y());

    if (val == 0) return 0; // collinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// The function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(shared_ptr<Vertex2D> p1, shared_ptr<Vertex2D> q1, shared_ptr<Vertex2D> p2, shared_ptr<Vertex2D> q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and p2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}
// Define Infinite (Using INT_MAX caused overflow problems)
#define INF 10000

// Returns true if the shared_ptr<Vertex2D> p lies inside the polygon[] with n vertices
bool isInside(shared_ptr<Vertex2D> polygon[], int n, shared_ptr<Vertex2D> p)
{
    // There must be at least 3 vertices in polygon[]
    if (n < 3) return false;

    // Create a shared_ptr<Vertex2D> for line segment from p to infinite
    shared_ptr<Vertex2D> extreme = make_shared<Vertex2D>(INF, p->Y());

    // Count intersections of the above line with sides of polygon
    int count = 0, i = 0;
    do
    {
        int next = (i + 1) % n;

        // Check if the line segment from 'p' to 'extreme' intersects
        // with the line segment from 'polygon[i]' to 'polygon[next]'
        if (doIntersect(polygon[i], polygon[next], p, extreme))
        {
            // If the shared_ptr<Vertex2D> 'p' is collinear with line segment 'i-next',
            // then check if it lies on segment. If it lies, return true,
            // otherwise false
            if (orientation(polygon[i], p, polygon[next]) == 0)
                return onSegment(polygon[i], p, polygon[next]);

            count++;
        }
        i = next;
    } while (i != 0);

    // Return true if count is odd, false otherwise
    return count & 1; // Same as (count%2 == 1)
}

bool isInside(shared_ptr<Vertex2D> polygon[], int n, int x, int y) {
    shared_ptr<Vertex2D> p = make_shared<Vertex2D>(x, y);
    return isInside(polygon, n, p);;
}