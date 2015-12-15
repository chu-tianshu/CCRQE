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

//templates
unsigned char *GB43;
unsigned char *GB45;
unsigned char *GB180;
unsigned char *GB258;
unsigned char *GB278;
unsigned char *GB286;
unsigned char *GB289;
unsigned char *GB366;
unsigned char *GB390;
unsigned char *GB407;
unsigned char *GB418;
unsigned char *GB461;
unsigned char *GB530;
unsigned char *GB660;
unsigned char *GB699;
unsigned char *GB859;
unsigned char *GB977;
unsigned char *GB1039;
unsigned char *GB1114;
unsigned char *GB1144;
unsigned char *GB1497;
unsigned char *GB303;

//sample
unsigned char *sample;

int main(int argc, char *argv[])
{
	//samples
	const char *name = argv[1];

	sample = readPPM(name);

	//templates
	GB43 = readPPM("gb43.ppm");
	GB45 = readPPM("gb45.ppm");
	GB180 = readPPM("gb180.ppm");
	GB258 = readPPM("gb258.ppm");
	GB278 = readPPM("gb278.ppm");
	GB286 = readPPM("gb286.ppm");
	GB289 = readPPM("gb289.ppm");
	GB366 = readPPM("gb366.ppm");
	GB390 = readPPM("gb390.ppm");
	GB407 = readPPM("gb407.ppm");
	GB418 = readPPM("gb418.ppm");
	GB461 = readPPM("gb461.ppm");
	GB530 = readPPM("gb530.ppm");
	GB660 = readPPM("gb660.ppm");
	GB699 = readPPM("gb699.ppm");
	GB859 = readPPM("gb859.ppm");
	GB977 = readPPM("gb977.ppm");
	GB1039 = readPPM("gb1039.ppm");
	GB1114 = readPPM("gb1114.ppm");
	GB1144 = readPPM("gb1144.ppm");
	GB1497 = readPPM("gb1497.ppm");
	GB303 = readPPM("gb303.ppm");

//	Recognizer rec(name);
//	rec.measureByGaborFeature();

	Assesser ass278(sample, GB278);
	ass278.assess();

	final = sample;

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