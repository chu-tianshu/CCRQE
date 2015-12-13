#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

int height;
int width;
int maxcolor;

unsigned char *final;

unsigned char *readPPM(const char* filename)
{
	unsigned char *pixmap;

	char buff[120];
	int c;

	FILE* file;
	file = fopen(filename, "rb");

	char typeFirstLetter;
	char typeSecondLetter;

	fgets(buff, sizeof(buff), file);

	if (buff[0] != 'P' || buff[1] != '6')
	{
		fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }

    c = getc(file);

    while (c == '#')
    {
   		while (getc(file) != '\n');
        c = getc(file);
    }

    ungetc(c, file);

	fscanf(file, "%d %d %d", &width, &height, &maxcolor);

  	pixmap = new unsigned char[width * height * 3];

  	for(int y = 0; y < height; y++)
  	{
    	for(int x = 0; x < width; x++)
    	{
	      	int pixel = ((height - 1 - y) * width + x) * 3;

	      	char red = fgetc(file);
	      	char green = fgetc(file);
	      	char blue = fgetc(file);

		  	pixmap[pixel] = green;
	      	pixel++;
	      	pixmap[pixel] = blue;
	      	pixel++;
	      	pixmap[pixel] = red;
    	}
  	}

  	return pixmap;
}

static void resize(int w, int h)
{   
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (w/2), 0, (h/2), 0, 1); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity() ;
}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0,0);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, final);
	glFlush();
}

static void processMouse(int button, int state, int x, int y)
{
	if(state == GLUT_UP)
	exit(0);               // Exit on mouse click.
}

static void init(void)
{
  	glClearColor(1,1,1,1); // Set background color.
}