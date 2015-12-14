#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <cstring>

class Recognizer
{
public:
	Recognizer(const char *filename)
	{
		sampImg = readPPM(filename);

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
	}

	double *measureByWeightedMap()
	{
		double *diff = new double[22];

		unsigned char *sampResized43 = resizeAndReposition(GB43, sampImg);
		unsigned char *sampResized45 = resizeAndReposition(GB45, sampImg);
		unsigned char *sampResized180 = resizeAndReposition(GB180, sampImg);
		unsigned char *sampResized258 = resizeAndReposition(GB258, sampImg);
		unsigned char *sampResized278 = resizeAndReposition(GB278, sampImg);
		unsigned char *sampResized286 = resizeAndReposition(GB286, sampImg);
		unsigned char *sampResized289 = resizeAndReposition(GB289, sampImg);
		unsigned char *sampResized366 = resizeAndReposition(GB366, sampImg);
		unsigned char *sampResized390 = resizeAndReposition(GB390, sampImg);
		unsigned char *sampResized407 = resizeAndReposition(GB407, sampImg);
		unsigned char *sampResized418 = resizeAndReposition(GB418, sampImg);
		unsigned char *sampResized461 = resizeAndReposition(GB461, sampImg);
		unsigned char *sampResized530 = resizeAndReposition(GB530, sampImg);
		unsigned char *sampResized660 = resizeAndReposition(GB660, sampImg);
		unsigned char *sampResized699 = resizeAndReposition(GB699, sampImg);
		unsigned char *sampResized859 = resizeAndReposition(GB859, sampImg);
		unsigned char *sampResized977 = resizeAndReposition(GB977, sampImg);
		unsigned char *sampResized1039 = resizeAndReposition(GB1039, sampImg);
		unsigned char *sampResized1114 = resizeAndReposition(GB1114, sampImg);
		unsigned char *sampResized1144 = resizeAndReposition(GB1144, sampImg);
		unsigned char *sampResized1497 = resizeAndReposition(GB1497, sampImg);
		unsigned char *sampResized303 = resizeAndReposition(GB303, sampImg);
		
		ChineseCharacter sampCharR43(sampResized43);
		ChineseCharacter sampCharR45(sampResized45);
		ChineseCharacter sampCharR180(sampResized180);
		ChineseCharacter sampCharR258(sampResized258);
		ChineseCharacter sampCharR278(sampResized278);
		ChineseCharacter sampCharR286(sampResized286);
		ChineseCharacter sampCharR289(sampResized289);
		ChineseCharacter sampCharR366(sampResized366);
		ChineseCharacter sampCharR390(sampResized390);
		ChineseCharacter sampCharR407(sampResized407);
		ChineseCharacter sampCharR418(sampResized418);
		ChineseCharacter sampCharR461(sampResized461);
		ChineseCharacter sampCharR530(sampResized530);
		ChineseCharacter sampCharR660(sampResized660);
		ChineseCharacter sampCharR699(sampResized699);
		ChineseCharacter sampCharR859(sampResized859);
		ChineseCharacter sampCharR977(sampResized977);
		ChineseCharacter sampCharR1039(sampResized1039);
		ChineseCharacter sampCharR1114(sampResized1114);
		ChineseCharacter sampCharR1144(sampResized1144);
		ChineseCharacter sampCharR1497(sampResized1497);
		ChineseCharacter sampCharR303(sampResized303);

		ChineseCharacter tempChar43(GB43);
		ChineseCharacter tempChar45(GB45);
		ChineseCharacter tempChar180(GB180);
		ChineseCharacter tempChar258(GB258);
		ChineseCharacter tempChar278(GB278);
		ChineseCharacter tempChar286(GB286);
		ChineseCharacter tempChar289(GB289);
		ChineseCharacter tempChar366(GB366);
		ChineseCharacter tempChar390(GB390);
		ChineseCharacter tempChar407(GB407);
		ChineseCharacter tempChar418(GB418);
		ChineseCharacter tempChar461(GB461);
		ChineseCharacter tempChar530(GB530);
		ChineseCharacter tempChar660(GB660);
		ChineseCharacter tempChar699(GB699);
		ChineseCharacter tempChar859(GB859);
		ChineseCharacter tempChar977(GB977);
		ChineseCharacter tempChar1039(GB1039);
		ChineseCharacter tempChar1114(GB1114);
		ChineseCharacter tempChar1144(GB1144);
		ChineseCharacter tempChar1497(GB1497);
		ChineseCharacter tempChar303(GB303);

		diff[0] = differenceInWeightMap(sampCharR43, tempChar43);
		diff[1] = differenceInWeightMap(sampCharR45, tempChar45);
		diff[2] = differenceInWeightMap(sampCharR180, tempChar180);
		diff[3] = differenceInWeightMap(sampCharR258, tempChar258);
		diff[4] = differenceInWeightMap(sampCharR278, tempChar278);
		diff[5] = differenceInWeightMap(sampCharR286, tempChar286);
		diff[6] = differenceInWeightMap(sampCharR289, tempChar289);
		diff[7] = differenceInWeightMap(sampCharR366, tempChar366);
		diff[8] = differenceInWeightMap(sampCharR390, tempChar390);
		diff[9] = differenceInWeightMap(sampCharR407, tempChar407);
		diff[10] = differenceInWeightMap(sampCharR418, tempChar418);
		diff[11] = differenceInWeightMap(sampCharR461, tempChar461);
		diff[12] = differenceInWeightMap(sampCharR530, tempChar530);
		diff[13] = differenceInWeightMap(sampCharR660, tempChar660);
		diff[14] = differenceInWeightMap(sampCharR699, tempChar699);
		diff[15] = differenceInWeightMap(sampCharR859, tempChar859);
		diff[16] = differenceInWeightMap(sampCharR977, tempChar977);
		diff[17] = differenceInWeightMap(sampCharR1039, tempChar1039);
		diff[18] = differenceInWeightMap(sampCharR1114, tempChar1114);
		diff[19] = differenceInWeightMap(sampCharR1144, tempChar1144);
		diff[20] = differenceInWeightMap(sampCharR1497, tempChar1497);
		diff[21] = differenceInWeightMap(sampCharR303, tempChar303);

		//For testing:
		for (int i = 0; i < 22; i++)
		{
			printf("diff %d: %3f\n", i, diff[i]);
		}

		return diff;
	}

	double *measureByGaborFeature()
	{
		double *diff = new double[22];

		ChineseCharacter sampChar(sampImg);

		ChineseCharacter tempChar43(GB43);
		ChineseCharacter tempChar45(GB45);
		ChineseCharacter tempChar180(GB180);
		ChineseCharacter tempChar258(GB258);
		ChineseCharacter tempChar278(GB278);
		ChineseCharacter tempChar286(GB286);
		ChineseCharacter tempChar289(GB289);
		ChineseCharacter tempChar366(GB366);
		ChineseCharacter tempChar390(GB390);
		ChineseCharacter tempChar407(GB407);
		ChineseCharacter tempChar418(GB418);
		ChineseCharacter tempChar461(GB461);
		ChineseCharacter tempChar530(GB530);
		ChineseCharacter tempChar660(GB660);
		ChineseCharacter tempChar699(GB699);
		ChineseCharacter tempChar859(GB859);
		ChineseCharacter tempChar977(GB977);
		ChineseCharacter tempChar1039(GB1039);
		ChineseCharacter tempChar1114(GB1114);
		ChineseCharacter tempChar1144(GB1144);
		ChineseCharacter tempChar1497(GB1497);
		ChineseCharacter tempChar303(GB303);

		diff[0] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar43.getNormalizedDistribution(), 8);
		diff[1] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar45.getNormalizedDistribution(), 8);
		diff[2] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar180.getNormalizedDistribution(), 8);
		diff[3] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar258.getNormalizedDistribution(), 8);
		diff[4] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar278.getNormalizedDistribution(), 8);
		diff[5] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar286.getNormalizedDistribution(), 8);
		diff[6] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar289.getNormalizedDistribution(), 8);
		diff[7] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar366.getNormalizedDistribution(), 8);
		diff[8] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar390.getNormalizedDistribution(), 8);
		diff[9] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar407.getNormalizedDistribution(), 8);
		diff[10] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar418.getNormalizedDistribution(), 8);
		diff[11] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar461.getNormalizedDistribution(), 8);
		diff[12] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar530.getNormalizedDistribution(), 8);
		diff[13] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar660.getNormalizedDistribution(), 8);
		diff[14] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar699.getNormalizedDistribution(), 8);
		diff[15] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar859.getNormalizedDistribution(), 8);
		diff[16] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar977.getNormalizedDistribution(), 8);
		diff[17] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar1039.getNormalizedDistribution(), 8);
		diff[18] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar1114.getNormalizedDistribution(), 8);
		diff[19] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar1144.getNormalizedDistribution(), 8);
		diff[20] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar1497.getNormalizedDistribution(), 8);
		diff[21] = euclideanDistance(sampChar.getNormalizedDistribution(), tempChar303.getNormalizedDistribution(), 8);	

		//For testing:
		for (int i = 0; i < 22; i++)
		{
			printf("diff %d: %3f\n", i, diff[i]);
		}

		return diff;
	}

private:
	unsigned char *sampImg;

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
};