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

//	printf("total black: %d\n", totalBlack);

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

	int lengthOfGrey = height * width;

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

int totalNumOfBlack(unsigned char *binaryImage)
{
	int totalBlack = 0;

	unsigned char *grey = getChannel(binaryImage, 0);

	for (int index = 0; index < height * width; index++)
	{
		if (grey[index] == 0)
		{
			totalBlack++;
		}
	}

	return totalBlack;
}

int xLeftThirdMesh(unsigned char *binaryImage)
{
	int sum = totalNumOfBlack(binaryImage);

	int xLeftThird = width - 1;
	int numOfPixelsOnLeft = 0;

	unsigned char *grey = getChannel(binaryImage, 0);

	int lengthOfGrey = height * width;

	double thirdBlack = sum / 3.0;
	double thres = sum * 0.1;

	while ((abs(thirdBlack - numOfPixelsOnLeft) > thres) && (xLeftThird != 0))
	{
		numOfPixelsOnLeft = 0;

		xLeftThird--;

		for (int index = 0; index < lengthOfGrey; index++)
		{
			int x = index - (index / width) * width;
			int y = index / width;

			if (grey[index] == 0 && x <= xLeftThird)
			{
				numOfPixelsOnLeft++;
			}
		}
	}

	return xLeftThird;
}

int xRightThirdMesh(unsigned char *binaryImage)
{
	int sum = totalNumOfBlack(binaryImage);

	int xRightThird = 0;
	int numOfPixelsOnRight = 0;

	unsigned char *grey = getChannel(binaryImage, 0);

	int lengthOfGrey = height * width;

	double thirdBlack = sum / 3.0;
	double thres = sum * 0.1;

	while ((abs(thirdBlack - numOfPixelsOnRight) > thres) && (xRightThird != width - 1))
	{
		numOfPixelsOnRight = 0;

		xRightThird++;

		for (int index = 0; index < lengthOfGrey; index++)
		{
			int x = index - (index / width) * width;
			int y = index / width;

			if (grey[index] == 0 && x >= xRightThird)
			{
				numOfPixelsOnRight++;
			}
		}

//		printf("right black %d\n", numOfPixelsOnRight);
	}

	return xRightThird;
}

int yBottomThirdMesh(unsigned char *binaryImage)
{
	int sum = totalNumOfBlack(binaryImage);

	int yBottomThird = height - 1;
	int numOfPixelsOnLeft = 0;

	unsigned char *grey = getChannel(binaryImage, 0);

	int lengthOfGrey = height * width;

	double thirdBlack = sum / 3.0;
	double thres = sum * 0.1;

	while ((abs(thirdBlack - numOfPixelsOnLeft) > thres) && (yBottomThird != 0))
	{
		numOfPixelsOnLeft = 0;

		yBottomThird--;

		for (int index = 0; index < lengthOfGrey; index++)
		{
			int x = index - (index / width) * width;
			int y = index / width;

			if (grey[index] == 0 && x <= yBottomThird)
			{
				numOfPixelsOnLeft++;
			}
		}
	}

	return yBottomThird;	
}

int yUpThirdMesh(unsigned char *binaryImage)
{
	int sum = totalNumOfBlack(binaryImage);

	int yUpThird = 0;
	int numOfPixelsOnRight = 0;

	unsigned char *grey = getChannel(binaryImage, 0);

	int lengthOfGrey = height * width;

	double thirdBlack = sum / 3.0;
	double thres = sum * 0.1;

	while ((abs(thirdBlack - numOfPixelsOnRight) > thres) && (yUpThird != height - 1))
	{
		numOfPixelsOnRight = 0;

		yUpThird++;

		for (int index = 0; index < lengthOfGrey; index++)
		{
			int x = index - (index / width) * width;
			int y = index / width;

			if (grey[index] == 0 && y >= yUpThird)
			{
				numOfPixelsOnRight++;
			}
		}
	}

	return yUpThird;
}

int xHalfMesh(unsigned char *binaryImage)
{
	int sum = totalNumOfBlack(binaryImage);

	int xHalf = width - 1;
	int numOfPixelsOnLeft = 0;

	unsigned char *grey = getChannel(binaryImage, 0);

	int lengthOfGrey = height * width;

	double halfBlack = sum / 2.0;
	double thres = sum * 0.08;

	while ((abs(halfBlack - numOfPixelsOnLeft) > thres) && (xHalf != 0))
	{
		numOfPixelsOnLeft = 0;

		xHalf--;

		for (int index = 0; index < lengthOfGrey; index++)
		{
			int x = index - (index / width) * width;
			int y = index / width;

			if (grey[index] == 0 && x <= xHalf)
			{
				numOfPixelsOnLeft++;
			}
		}
	}

	return xHalf;
}

int yHalfMesh(unsigned char *binaryImage)
{
	int sum = totalNumOfBlack(binaryImage);

	int yHalf = 0;
	int numOfPixelsOnTop = 0;

	unsigned char *grey = getChannel(binaryImage, 0);

	int lengthOfGrey = height * width;

	double halfBlack = sum / 2.0;
	double thres = sum * 0.08;

	while ((abs(halfBlack - numOfPixelsOnTop) > thres) && (yHalf != height - 1))
	{
		numOfPixelsOnTop = 0;

		yHalf++;

		for (int index = 0; index < lengthOfGrey; index++)
		{
			int x = index - (index / width) * width;
			int y = index / width;

			if (grey[index] == 0 && y >= yHalf)
			{
				numOfPixelsOnTop++;
			}
		}
	}

	return yHalf;
}

int *projectionX(unsigned char *boundedBinaryImage)
{
	unsigned char *grey = getChannel(boundedBinaryImage, 0);

	int *proX = new int[width];

	for (int i = 0; i < width; i++)
	{
		proX[i] = 0;
	}

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int index = y * width + x;

			if (grey[index] == 0)
			{
				proX[x]++;
			}
		}
	}

	return proX;
}

int *projectionY(unsigned char *boundedBinaryImage)
{
	unsigned char *grey = getChannel(boundedBinaryImage, 0);

	int *proY = new int[height];

	for (int i = 0; i < height; i++)
	{
		proY[i] = 0;
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = y * width + x;

			if (grey[index] == 0)
			{
				proY[y]++;
			}
		}
	}

	return proY;
}