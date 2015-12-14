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

using namespace std;

unsigned char *heTplt;
unsigned char *chuanTplt;

unsigned char *heSp;
unsigned char *chuanSp;

unsigned char *heSpReshapedToHe;
unsigned char *heSpReshapedToChuan;
unsigned char *chuanSpReshapedToHe;
unsigned char *chuanSpReshapedToChuan;

int main(int argc, char *argv[])
{
	heTplt = readPPM("heTplt.ppm");
	chuanTplt = readPPM("chuanTplt.ppm");
	
	heSp = readPPM("heSp.ppm");
	chuanSp = readPPM("chuanSp.ppm");

//	heSpReshapedToHe = resizeAndReposition(heTplt, heSp);
//	heSpReshapedToChuan = resizeAndReposition(chuanTplt, heSp);
//	chuanSpReshapedToHe = resizeAndReposition(heTplt, chuanSp);
//	chuanSpReshapedToChuan = resizeAndReposition(chuanTplt, chuanSp);

	ChineseCharacter heTpltChar(heTplt);
	ChineseCharacter chuanTpltChar(chuanTplt);

	ChineseCharacter heSpChar(heSp);
	ChineseCharacter chuanSpChar(chuanSp);

//	ChineseCharacter heSpReshapedToHeChar(heSpReshapedToHe);
//	ChineseCharacter heSpReshapedToChuanChar(heSpReshapedToChuan);
//	ChineseCharacter chuanSpReshapedToHeChar(chuanSpReshapedToHe);
//	ChineseCharacter chuanSpReshapedToChuanChar(chuanSpReshapedToChuan);

//	double diffHeTpltHeSp = differenceInWeightMap(heTpltChar, heSpReshapedToHeChar, height * width);
//	double diffChuanTpltHeSp = differenceInWeightMap(chuanTpltChar, heSpReshapedToChuanChar, height * width);
//	double diffHeTpltChuanSp = differenceInWeightMap(heTpltChar, chuanSpReshapedToHeChar, height * width);
//	double diffChuanTpltChuanSp = differenceInWeightMap(chuanTpltChar, chuanSpReshapedToChuanChar, height * width);

//	printf("Difference between he template and he sample: %3f\n", diffHeTpltHeSp);
//	printf("Difference between he template and chuan sample: %3f\n", diffHeTpltChuanSp);
//	printf("Difference between chuan template and he sample: %3f\n", diffChuanTpltHeSp);
//	printf("Difference between chuan template and chuan sample: %3f\n", diffChuanTpltChuanSp);

//	printf("he template\n");
//	heTpltChar.printNormalizedDistribution();
//	printf("he sample\n");
//	heSpChar.printNormalizedDistribution();
//	printf("chuan template\n");
//	chuanTpltChar.printNormalizedDistribution();
//	printf("chuan sample\n");
//	chuanSpChar.printNormalizedDistribution();

//	double *normDistrHeTplt = heTpltChar.getNormalizedDistribution();
//	double *normDistrHeSp = heSpChar.getNormalizedDistribution();
//	double *normDistrChuanTplt = chuanTpltChar.getNormalizedDistribution();
//	double *normDistrChuanSp = chuanSpChar.getNormalizedDistribution();

//	double distHeHe = euclideanDistance(normDistrHeSp, normDistrHeTplt, 8);
//	double distChuanHe = euclideanDistance(normDistrChuanSp, normDistrHeTplt, 8);
//	double distChuanChuan = euclideanDistance(normDistrChuanSp, normDistrChuanTplt, 8);
//	double distHeChuan = euclideanDistance(normDistrHeSp, normDistrChuanTplt, 8);

//	printf("dist heSp heTplt: %3f\n", distHeHe);
//	printf("dist chuanSp heTplt: %3f\n", distChuanHe);
//	printf("dist chuanSp chuanTplt: %3f\n", distChuanChuan);
//	printf("dist heSp chuanTplt: %3f\n", distHeChuan);

//	heSpChar.getYUpThird();
	
	Assesser heAssesser(heSpChar, heTpltChar);

//	heAssesser.getMuonNearFC();
//	heAssesser.getMuonFarFC();
//	heAssesser.getMuonSmallFA();
//	heAssesser.getMuonProperFA();
//	heAssesser.getMuonLargeFA();
//	heAssesser.getDx();
//	heAssesser.getDy();
//	heAssesser.getMuonLessDx();
//	heAssesser.getMuonMuchDx();
//	heAssesser.getMuonLessDy();
//	heAssesser.getMuonMuchDy();
	heAssesser.getFS();
	heAssesser.getMuonLeftFS();
	heAssesser.getMuonProperFS();
	heAssesser.getMuonRightFS();

	Assesser chuanAssesser(chuanSpChar, chuanTpltChar);

//	chuanAssesser.getFS();
//	chuanAssesser.getMuonLeftFS();
//	chuanAssesser.getMuonProperFS();
//	chuanAssesser.getMuonRightFS();

	final = heSpChar.getGaborFilteredImageLDgn();

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