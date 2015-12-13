#include <stdio.h>
#include <stdlib.h>
#include <cmath>

int xOfCenterOfGravity(unsigned char *binaryImage)
{
	int xOfCenterOfGravity = 0;
	int totalBlack = 0;
	int sumOfBlackByX = 0;

	unsigned char *grey = getChannel(binaryImage, 0);

	int lengthOfGrey = height * width;

	for (int index = 0; index < lengthOfGrey; index++)
	{
		int x = index - (index / width) * width;
		int y = index / width;

		if (grey[index] == 0)
		{
			totalBlack++;
			sumOfBlackByX += x;
		}
	}

	xOfCenterOfGravity = sumOfBlackByX / totalBlack;

	return xOfCenterOfGravity;
}

int yOfCenterOfGravity(unsigned char *binaryImage)
{
	int yOfCenterOfGravity = 0;
	int totalBlack = 0;
	int sumOfBlackByY = 0;

	unsigned char *grey = getChannel(binaryImage, 0);

	int lengthOfGrey = height * width;

	for (int index = 0; index < lengthOfGrey; index++)
	{
		int x = index - (index / width) * width;
		int y = index / width;

		if (grey[index] == 0)
		{
			totalBlack++;
			sumOfBlackByY += y;
		}
	}

	yOfCenterOfGravity = sumOfBlackByY / totalBlack;

	return yOfCenterOfGravity;
}

int *numOfPixelsInEachQuadrant(unsigned char *binaryImage, int xo, int yo)
{
	int *numOfPixels = new int[4];

	unsigned char *grey = getChannel(binaryImage, 0);

	for (int index = 0; index < lengthOfGrey; index++)
	{
		int x = index - (index / width) * width;
		int y = index / width;		

		if (grey[index] == 0)
		{
			if (y >= yo)
			{
				if (x >= xo)
				{
					numOfPixels[0]++;
				}
				else
				{
					numOfPixels[1]++;
				}
			}
			else
			{
				if (x < xo)
				{
					numOfPixels[2]++;
				}
				else
				{
					numOfPixels[3]++;
				}
			}
		}
	}

	return numOfPixels;
}