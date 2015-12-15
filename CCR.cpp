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

//sample
unsigned char *sample;

//template
unsigned char *temple;

int main(int argc, char *argv[])
{
	//samples
/*	const char *name = argv[2];

	sample = readPPM(name);

	ChineseCharacter sampleChar(sample);

	//templates
	unsigned char *GB278 = readPPM("gb278.ppm");

	ChineseCharacter templateChar(GB278);

//	Recognizer rec(name);
//	rec.measureByGaborFeature();

	Assesser ass278(sample, GB278);
	ass278.assess();
*/
	if (strcmp(argv[1], "recognize") == 0)
	{
		sample = readPPM(argv[2]);

		ChineseCharacter sampleChar(sample);

		Recognizer rec(argv[2]);

		rec.measureByGaborFeature();
	}

	if (strcmp(argv[1], "evaluate") == 0)
	{
		sample = readPPM(argv[2]);
		temple = readPPM(argv[3]);

		ChineseCharacter sampleChar(sample);
		ChineseCharacter templeChar(temple);

		Assesser assesser(sampleChar, templeChar);
		assesser.assess();
	}

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