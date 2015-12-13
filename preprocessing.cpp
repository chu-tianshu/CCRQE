#include <stdio.h>
#include <stdlib.h>

using namespace std;

unsigned char *getChannel(unsigned char *pixmap, int num)
{
	int lengthOfChannelMap = height * width;
	unsigned char *channelMap = new unsigned char[lengthOfChannelMap];

	for (int i = 0; i < lengthOfChannelMap; i++)
	{
		channelMap[i] = pixmap[i * 3 + num];
	}

	return channelMap;
}

unsigned char *getCombination(unsigned char *map1, unsigned char *map2, unsigned char *map3)
{
	int lengthOfCombination = height * width * 3;
	unsigned char *combination = new unsigned char[lengthOfCombination];

	for (int i = 0; i < lengthOfCombination; i++)
	{
		int mod = i % 3;

		switch(mod)
		{
			case 0:
			{
				combination[i] = map1[i / 3];
				break;
			}
			case 1:
			{
				combination[i] = map2[i / 3];
				break;
			}
			case 2:
			{
				combination[i] = map3[i / 3];
				break;
			}
			default:
			{
				break;
			}
		}
	}

	return combination;
}

unsigned char *getBValue(unsigned char *original)
{
	int lengthOfChannelMap = height * width;

	unsigned char *BValue = new unsigned char[lengthOfChannelMap];

	unsigned char *red = getChannel(original, 0);

	for (int k = 0; k < lengthOfChannelMap; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;

		if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
		{
			BValue[k] = -1;
		}
		else
		{
			int sumOfColor = red[k + width] + red[k + width + 1] + red[k + 1] + red[k - width + 1] + red[k - width] + red[k - width - 1] + red[k - 1] + red[k + width - 1];
			int B = 8 - sumOfColor / 255;

			BValue[k] = B;
		}
	}

	return BValue;
}

unsigned char *getAValue(unsigned char *original)
{
	int length = height * width * 3;
	int lengthOfChannelMap = height * width;

	unsigned char *BValue = getBValue(original);
	unsigned char *AValue = new unsigned char[lengthOfChannelMap];

	unsigned char *red = getChannel(original, 0);

	for (int k = 0; k < lengthOfChannelMap; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;
		AValue[k] = 0;

		if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
		{
			AValue[k] = -1;
		}
		else
		{
			int neighbor[9];

			neighbor[0] = red[k + width];
			neighbor[1] = red[k + width + 1];
			neighbor[2] = red[k + 1];
			neighbor[3] = red[k - width + 1];
			neighbor[4] = red[k - width];
			neighbor[5] = red[k - width - 1];
			neighbor[6] = red[k - 1];
			neighbor[7] = red[k + width - 1];
			neighbor[8] = red[k + width];

			for (int i = 0; i < 8; i++)
			{
				if (neighbor[i] == 255 && neighbor[i + 1] == 0)
				{
					AValue[k] = AValue[k] + 1;
				}
			}
		}
	}

	return AValue;
}

unsigned char *toGrey(unsigned char *pixmap)
{
	int length = height * width * 3;

	unsigned char *grey = new unsigned char[length];

	for (int i = 0; i < length; i = i + 3)
	{
		int greyScale = (pixmap[i] + pixmap[i + 1] + pixmap[i + 2]) / 3;

		grey[i] = greyScale;
		grey[i + 1] = greyScale;
		grey[i + 2] = greyScale;
	}

	return grey;
}

unsigned char *toBinary(unsigned char *pixmap)
{
	int length = height * width * 3;

	unsigned char *grey = toGrey(pixmap);
	unsigned char *binary = new unsigned char[length];

	for (int i = 0; i < length; i = i + 3)
	{
		int borw = grey[i] >= 127 ? 255 : 0;

		binary[i] = borw;
		binary[i + 1] = borw;
		binary[i + 2] = borw;
	}

	return binary;
}

unsigned char *addWhiteBound(unsigned char *original)
{
	int length = height * width * 3;
	int lengthOfChannelMap = height * width;

	unsigned char *bounded = new unsigned char[length];

	unsigned char *red = getChannel(original, 0);
	unsigned char *green = getChannel(original, 1);
	unsigned char *blue = getChannel(original, 2);
	
	unsigned char *newRed = new unsigned char[lengthOfChannelMap];
	unsigned char *newGreen = new unsigned char[lengthOfChannelMap];
	unsigned char *newBlue = new unsigned char[lengthOfChannelMap];

	for (int k = 0; k < lengthOfChannelMap; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;
		
		if (x < 2 || x > width - 3 || y < 2 || y > height - 3)
		{
			newRed[k] = 255;
			newGreen[k] = 255;
			newBlue[k] = 255;
		}
		else
		{
			newRed[k] = red[k];
			newGreen[k] = green[k];
			newBlue[k] = blue[k];
		}
	}

	bounded = getCombination(newRed, newGreen, newBlue);

	return bounded;
}

unsigned char *hilditch(unsigned char *original)
{
	int length = height * width * 3;
	int lengthOfChannelMap = height * width;

	unsigned char *BValue = getBValue(original);
	unsigned char *AValue = getAValue(original);

	unsigned char *thinned = new unsigned char[length];

	unsigned char *red = getChannel(original, 0);
	unsigned char *green = getChannel(original, 1);
	unsigned char *blue = getChannel(original, 2);
	
	unsigned char *newRed = new unsigned char[lengthOfChannelMap];
	unsigned char *newGreen = new unsigned char[lengthOfChannelMap];
	unsigned char *newBlue = new unsigned char[lengthOfChannelMap];

	for (int k = 0; k < lengthOfChannelMap; k++)
	{
		if (red[k] == 255)
		{
			newRed[k] = 255;
			newGreen[k] = 255;
			newBlue[k] = 255;
		}
		else
		{
			if (((BValue[k] >= 2) && (BValue[k] <= 6))
			 && (AValue[k] == 1) 
			 && ((red[k + width] == 255 || red[k + 1] == 255 || red[k - 1] == 255) || AValue[k + width] != 1)
			 && ((red[k + width] == 255 || red[k + 1] == 255 || red[k - width] == 255) || AValue[k + 1] != 1))
			{
				newRed[k] = 255;
				newGreen[k] = 255;
				newBlue[k] = 255;
			}
			else
			{
				newRed[k] = 0;
				newGreen[k] = 0;
				newBlue[k] = 0;
			}
		}
	}

	thinned = getCombination(newRed, newGreen, newBlue);

	return thinned;
}

unsigned char *deParallelLeft(unsigned char *original)
{
	int length = height * width * 3;
	int lengthOfChannelMap = height * width;

	unsigned char *deParallelLefted = new unsigned char[length];

	unsigned char *red = getChannel(original, 0);
	unsigned char *green = getChannel(original, 1);
	unsigned char *blue = getChannel(original, 2);
	
	unsigned char *newRed = new unsigned char[lengthOfChannelMap];
	unsigned char *newGreen = new unsigned char[lengthOfChannelMap];
	unsigned char *newBlue = new unsigned char[lengthOfChannelMap];

	for (int k = 0; k < lengthOfChannelMap; k++)
	{
		if ((red[k] == 0) && (red[k + width] == 0 && red[k - 1] == 0) && red[k - width + 1] == 255)
		{
			newRed[k] = 255;
			newGreen[k] = 255;
			newBlue[k] = 255;
		}
		else
		{
			newRed[k] = red[k];
			newGreen[k] = green[k];
			newBlue[k] = blue[k];
		}
	}

	deParallelLefted = getCombination(newRed, newGreen, newBlue);

	return deParallelLefted;
}

unsigned char *deParallelRight(unsigned char *original)
{
	int length = height * width * 3;
	int lengthOfChannelMap = height * width;

	unsigned char *deParallelRighted = new unsigned char[length];

	unsigned char *red = getChannel(original, 0);
	unsigned char *green = getChannel(original, 1);
	unsigned char *blue = getChannel(original, 2);
	
	unsigned char *newRed = new unsigned char[lengthOfChannelMap];
	unsigned char *newGreen = new unsigned char[lengthOfChannelMap];
	unsigned char *newBlue = new unsigned char[lengthOfChannelMap];

	for (int k = 0; k < lengthOfChannelMap; k++)
	{
		if ((red[k] == 0) && (red[k + width] == 0 && red[k + 1] == 0) && red[k - width - 1] == 255)
		{
			newRed[k] = 255;
			newGreen[k] = 255;
			newBlue[k] = 255;
		}
		else
		{
			newRed[k] = red[k];
			newGreen[k] = green[k];
			newBlue[k] = blue[k];
		}
	}

	deParallelRighted = getCombination(newRed, newGreen, newBlue);

	return deParallelRighted;
}

unsigned char *deParallel(unsigned char *original)
{
	unsigned char *deParallelLefted = deParallelLeft(original);
	unsigned char *deParalleled = deParallelRight(deParallelLefted);

	return deParalleled;
}

unsigned char *thin(unsigned char *original)
{
	unsigned char *hilditched = hilditch(original);
	unsigned char *thinned = deParallel(hilditched);

	return thinned;
}

/* Rid the binary image of unexpected curve on a stroke of one direction like these:
 * 100
 * 010
 * 100
 * and turn it to like:
 * 100
 * 100
 * 100
 * Performed after the thinning process
 */ 
unsigned char *denoise(unsigned char *original)
{
	int lengthOfChannelMap = width * height;

	unsigned char *denoised = original;

	unsigned char *red = getChannel(original, 0);
	unsigned char *green = getChannel(original, 1);
	unsigned char *blue = getChannel(original, 2);
	
	unsigned char *newRed = new unsigned char[lengthOfChannelMap];
	unsigned char *newGreen = new unsigned char[lengthOfChannelMap];
	unsigned char *newBlue = new unsigned char[lengthOfChannelMap];

	for (int k = 0; k < lengthOfChannelMap; k++)
	{
		if (red[k] == 0)
		{
			/* from
			 * 100
			 * 010
			 * 100
			 * to
			 * 100
			 * 100
			 * 100
			 */
			if (red[k + width] == 255
			 && red[k + width + 1] == 255
			 && red[k + 1] == 255
			 && red[k - width + 1] == 255
			 && red[k - width] == 255
			 && red[k - width - 1] == 0
			 && red[k - 1] == 255
			 && red[k + width - 1] == 0)
			{
				newRed[k - 1] = 0;
				newGreen[k - 1] = 0;
				newBlue[k - 1] = 0;

				newRed[k] = 255;
				newGreen[k] = 255;
				newBlue[k] = 255;
			}

			/* from
			 * 001
			 * 010
			 * 001
			 * to
			 * 001
			 * 001
			 * 001
			 */
			else if (red[k + width] == 255
			 && red[k + width + 1] == 0
			 && red[k + 1] == 255
			 && red[k - width + 1] == 0
			 && red[k - width] == 255
			 && red[k - width - 1] == 255
			 && red[k - 1] == 255
			 && red[k + width - 1] == 255)
			{
				newRed[k + 1] = 0;
				newGreen[k + 1] = 0;
				newBlue[k + 1] = 0;

				newRed[k] = 255;
				newGreen[k] = 255;
				newBlue[k] = 255;
			}

			/* from
			 * 101
			 * 010
			 * 000
			 * to
			 * 111
			 * 000
			 * 000
			 */
			else if (red[k + width] == 255
			 && red[k + width + 1] == 0
			 && red[k + 1] == 255
			 && red[k - width + 1] == 255
			 && red[k - width] == 255
			 && red[k - width - 1] == 255
			 && red[k - 1] == 255
			 && red[k + width - 1] == 0)
			{
				newRed[k + width] = 0;
				newGreen[k + width] = 0;
				newBlue[k + width] = 0;

				newRed[k] = 255;
				newGreen[k] = 255;
				newBlue[k] = 255;
			}

			/* from
			 * 000
			 * 010
			 * 101
			 * to
			 * 000
			 * 000
			 * 111
			 */
			else if (red[k + width] == 255
			 && red[k + width + 1] == 255
			 && red[k + 1] == 255
			 && red[k - width + 1] == 0
			 && red[k - width] == 255
			 && red[k - width - 1] == 0
			 && red[k - 1] == 255
			 && red[k + width - 1] == 255)
			{
				newRed[k - width] = 0;
				newGreen[k - width] = 0;
				newBlue[k - width] = 0;

				newRed[k] = 255;
				newGreen[k] = 255;
				newBlue[k] = 255;
			}

			else
			{
				newRed[k] = red[k];
				newGreen[k] = green[k];
				newBlue[k] = blue[k];
			}
		}

		else
		{
			newRed[k] = red[k];
			newGreen[k] = green[k];
			newBlue[k] = blue[k];
		}
	}

	denoised = getCombination(newRed, newGreen, newBlue);

	return denoised;
}

//Make 4-connecting corners to 8-connecting corners
unsigned char *cornerModify(unsigned char *original)
{
	int lengthOfChannelMap = width * height;

	unsigned char *cornerModified = original;
	unsigned char *originalB = getBValue(original);

	unsigned char *red = getChannel(original, 0);
	unsigned char *green = getChannel(original, 1);
	unsigned char *blue = getChannel(original, 2);
	
	unsigned char *newRed = new unsigned char[lengthOfChannelMap];
	unsigned char *newGreen = new unsigned char[lengthOfChannelMap];
	unsigned char *newBlue = new unsigned char[lengthOfChannelMap];

	for (int k = 0; k < lengthOfChannelMap; k++)
	{
		if (red[k] == 0 && originalB[k] == 2)
		{
			if (red[k + width] == 0)
			{
				if (red[k + 1] == 0 || red[k - 1] == 0)
				{
					newRed[k] = 255;
					newGreen[k] = 255;
					newBlue[k] = 255;
				}
				else
				{
					newRed[k] = red[k];
					newGreen[k] = green[k];
					newBlue[k] = blue[k];
				}
			}
			else if (red[k - width] == 0)
			{
				if (red[k + 1] == 0 || red[k - 1] == 0)
				{
					newRed[k] = 255;
					newGreen[k] = 255;
					newBlue[k] = 255;
				}
				else
				{
					newRed[k] = red[k];
					newGreen[k] = green[k];
					newBlue[k] = blue[k];
				}				
			}
			else
			{
				newRed[k] = red[k];
				newGreen[k] = green[k];
				newBlue[k] = blue[k];				
			}
		}
		else
		{
			newRed[k] = red[k];
			newGreen[k] = green[k];
			newBlue[k] = blue[k];
		}
	}

	cornerModified = getCombination(newRed, newGreen, newBlue);

	return cornerModified;
}

void showBlackAndWhite(unsigned char *binaryMap)
{
	int lengthOfGreyScale = width * height;

	unsigned char *grey = getChannel(binaryMap, 0);

	for (int i = 0; i < lengthOfGreyScale; i++)
	{
		int x = i - (i / width) * width;
		int y = i / width;

		if (grey[i] == 255)
		{
			printf(" ");
		}
		else
		{
			printf("b");
		}

		if (x == width - 1)
		{
			printf("\n");
		}
	}
}

unsigned char *erosion(unsigned char *pixmap)
{
	int length = height * width * 3;
	int lengthOfChannelMap = height * width;

	unsigned char *erosion = new unsigned char[length];

	unsigned char *binary = toBinary(pixmap);
	unsigned char *red = getChannel(binary, 1);

	unsigned char *newRed = new unsigned char[lengthOfChannelMap];
	unsigned char *newGreen = new unsigned char[lengthOfChannelMap];
	unsigned char *newBlue = new unsigned char[lengthOfChannelMap];	

	for (int k = 0; k < lengthOfChannelMap; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;

		if (x > 0 && x < width - 1 && y > 0 && y < height - 1)
		{
			if (red[k - width] == 255 || red[k - 1] == 255 || red[k] == 255 || red[k + 1] == 255 || red[k + width] == 255)
			{
				newRed[k] = 255;
				newGreen[k] = 255;
				newBlue[k] = 255;
			}
			else
			{
				newRed[k] = 0;
				newGreen[k] = 0;
				newBlue[k] = 0;
			}
		}
		else
		{
			newRed[k] = red[k];
			newGreen[k] = red[k];
			newBlue[k] = red[k];		
		}		
	}

	erosion = getCombination(newRed, newGreen, newBlue);

	return erosion;
}

unsigned char *dilation(unsigned char *pixmap)
{
	int length = height * width * 3;
	int lengthOfChannelMap = height * width;

	unsigned char *dilation = new unsigned char[length];

	unsigned char *binary = toBinary(pixmap);
	unsigned char *red = getChannel(binary, 1);

	unsigned char *newRed = new unsigned char[lengthOfChannelMap];
	unsigned char *newGreen = new unsigned char[lengthOfChannelMap];
	unsigned char *newBlue = new unsigned char[lengthOfChannelMap];	

	for (int k = 0; k < lengthOfChannelMap; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;

		if (x > 0 && x < width - 1 && y > 0 && y < height - 1)
		{
			if (red[k - width] == 0 || red[k - 1] == 0 || red[k] == 0 || red[k + 1] == 0 || red[k + width] == 0)
			{
				newRed[k] = 0;
				newGreen[k] = 0;
				newBlue[k] = 0;
			}
			else
			{
				newRed[k] = 255;
				newGreen[k] = 255;
				newBlue[k] = 255;
			}
		}
		else
		{
			newRed[k] = red[k];
			newGreen[k] = red[k];
			newBlue[k] = red[k];		
		}		
	}

	dilation = getCombination(newRed, newGreen, newBlue);

	return dilation;
}