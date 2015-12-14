#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <cmath>
#include "glIO.h"
#include "characterClasses.cpp"
#include "templateMatching.cpp"
#include "assesser.cpp"
#include "Recognizer.cpp"

using namespace std;

//samples
unsigned char *GB43_95;

int main(int argc, char *argv[])
{
	//samples
	const char *name = "gb258_95.ppm";

	Recognizer rec(name);
	rec.measureByGaborFeature();

	final = GB43_95;

	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100); // Where the window will display on-screen.
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutCreateWindow("ppmview");
	init();
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMainLoop();

	return 0;
}