
// 1. Bajar freeglut http://www.lighthouse3d.com/tutorials/glut-tutorial/
// 2. Bajar AntTweakBar http://anttweakbar.sourceforge.net/doc/
// 3. Bajar "Open File Dialog", "open"

#define FREEGLUT_STATIC
#include "GL/freeglut.h"
#include <list>
#include <iostream>
#include <memory>
#include <line.h>

using namespace std;


int width = 320, height = 240;



list <shared_ptr<CShape>> shapes;

shared_ptr<CShape> current_shape;


void renderScene(void) 
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.5f, 0.25f);

	for (auto const& x : shapes) 
		x->render();
	
	glutSwapBuffers();
}

void changeSize(int w, int h) 
{
	if (h == 0)
		return;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	glOrtho(0, w, 0, h, -1, 1);

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	width = w;
	height = h;
}

int main(int argc, char** argv)
{
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Proyecto 1 - Leonardo Mendoza");

	cout << "Hi Hi :,D" << endl;

	glViewport(0, 0, width, height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);

	shared_ptr<CLine> l1 = make_shared <CLine>(1, 1, 1);
	shared_ptr<CLine> l2 = make_shared <CLine>(1, 0, 0);
	
	l1->set(0, 0, 100, 100);
	l2->set(100, 100, 200, 100);

	shapes.push_back(l1);
	shapes.push_back(l2);

	// register callbacks
	glutDisplayFunc(renderScene);
	
	// Here is our new entry in the main function
	glutReshapeFunc(changeSize);
	glutMainLoop();
	cout << "Bye Bye..." << endl;
	return 0;
}