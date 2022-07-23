#pragma once
#include <math.h>
#include <stdio.h>
//#include <shape.h> // For putPixel
// Function to calculate distance
float distancef(int x1, int y1, int x2, int y2)
{
    // Calculating distance
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
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