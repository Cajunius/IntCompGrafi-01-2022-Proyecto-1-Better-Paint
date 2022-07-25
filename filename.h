#pragma once


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

extern string current_file_name;
extern int current_file_mode; //0 Nothing, 1 Load, 2 Save
/*
class text
{
public:
	string current_file_name = "ShapesFile.txt";
	text(string s) {
		current_file_name = s;
	}
};
*/


/*
#pragma once

#include <list>
#include <memory>
#include <shape.h>


list <shared_ptr<CShape>> shapes;
shared_ptr<CShape> current_shape;

*/