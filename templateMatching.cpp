#include <stdio.h>
#include <stdlib.h>

double differenceInWeightMap(ChineseCharacter char1, ChineseCharacter char2)
{
	double *map1 = char1.getBlurredWeightMap();
	double *map2 = char2.getBlurredWeightMap();

	double diff = difference(map1, map2, height * width) / (height * width);

	return diff;
}

int leftMostX(unsigned char *image)
{
	unsigned char *red = getChannel(image, 0);

	int XL = width - 1;

	for (int k = 0; k < height * width; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;

		if (red[k] == 0)
		{
			if (x < XL)
			{
				XL = x;
			}
		}
	}

	return XL;
}

int rightMostX(unsigned char *image)
{
	unsigned char *red = getChannel(image, 0);

	int XR = 0;

	for (int k = 0; k < height * width; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;

		if (red[k] == 0)
		{
			if (x > XR)
			{
				XR = x;
			}
		}
	}

	return XR;
}

int upMostY(unsigned char *image)
{
	unsigned char *red = getChannel(image, 0);

	int YU = 0;

	for (int k = 0; k < height * width; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;

		if (red[k] == 0)
		{
			if (y > YU)
			{
				YU = y;
			}
		}
	}

	return YU;
}

int bottomMostY(unsigned char *image)
{
	unsigned char *red = getChannel(image, 0);

	int YB = height - 1;

	for (int k = 0; k < height * width; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;

		if (red[k] == 0)
		{
			if (y < YB)
			{
				YB = y;
			}
		}
	}

	return YB;
}

/*For a better template matching method, we map the original writing sample
 *to a new image which has a bounding box of the same size and position as
 *the template
 */
unsigned char *resizeAndReposition(unsigned char *temp, unsigned char *samp)
{
	int lengthOfGreyScale = height * width;

	unsigned char *resized = new unsigned char[height * width * 3];

	unsigned char *greySamp = getChannel(samp, 0);
	unsigned char *greyNew = new unsigned char[height * width];

	int xltemp = leftMostX(temp);
	int xrtemp = rightMostX(temp);
	int ybtemp = bottomMostY(temp);
	int yutemp = upMostY(temp);

	int heightOfBoundingBoxTemp = yutemp - ybtemp;
	int widthOfBoundingBoxTemp = xrtemp - xltemp;

	int xlsamp = leftMostX(samp);
	int xrsamp = rightMostX(samp);
	int ybsamp = bottomMostY(samp);
	int yusamp = upMostY(samp);

	int heightOfBoundingBoxSamp = yusamp - ybsamp;
	int widthOfBoundingBoxSamp = xrsamp - xlsamp;

	for (int index = 0; index < lengthOfGreyScale; index++)
	{
		int x = index - (index / width) * width;
		int y = index / width;

		if (x < xltemp || x > xrtemp || y < ybtemp || y > yutemp)
		{
			greyNew[index] = 255;
		}
		else
		{
			double xPortion = (1.0 * (x - xltemp)) / widthOfBoundingBoxTemp;
			double yPortion = (1.0 * (y - ybtemp)) / heightOfBoundingBoxTemp;

			int xInSamp = xlsamp + xPortion * widthOfBoundingBoxSamp;
			int yInSamp = ybsamp + yPortion * heightOfBoundingBoxSamp;

			int indexInSamp = yInSamp * width + xInSamp;

			greyNew[index] = greySamp[indexInSamp];
		}
	}

	resized = getCombination(greyNew, greyNew, greyNew);

	return resized;
}