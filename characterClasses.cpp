#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "preprocessing.cpp"
#include "strokeFinding.cpp"
#include "qualityEvaluation.cpp"

using namespace std;

class ChineseCharacter
{
public:
	ChineseCharacter(unsigned char *theImage)
	{
		//Set up image
		image = theImage;

		//Set up binary image
		binaryImage = toBinary(image);

		//Set up white bounded image
		whiteBoundedImage = addWhiteBound(image);

		//Preprocess the image
		unsigned char *thinnedImage = thin(binaryImage);

		int n = 1000;

		while (n > 0)
		{
			thinnedImage = thin(thinnedImage);
			n--;
		}

		unsigned char *denoisedImage = denoise(thinnedImage);
		unsigned char *cornerModifiedImage = cornerModify(denoisedImage);

		preprocessedImage = cornerModifiedImage;

		//Set up Q map
		QMap = getQ(preprocessedImage);

		//Set up G map
		GMap = getG(QMap);

		//Set up segmented image
		int length = width * height * 3;
		int lengthOfGreyScale = width * height;

		segmentedImage = new unsigned char[length];

		unsigned char *newRed = new unsigned char[lengthOfGreyScale];
		unsigned char *newGreen = new unsigned char[lengthOfGreyScale];
		unsigned char *newBlue = new unsigned char[lengthOfGreyScale];

		for (int k = 0; k < lengthOfGreyScale; k++)
		{
			if (GMap[k] != ' ' && GMap[k] != '-')
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

		segmentedImage = getCombination(newRed, newGreen, newBlue);

		//Set up connection value map
		int indexOfContour = 0;

		unsigned char *red = getChannel(preprocessedImage, 0);
		unsigned char *theConnectionValueMap = new unsigned char[lengthOfGreyScale];

		for (int k = 0; k < lengthOfGreyScale; k++)
		{
			theConnectionValueMap[k] = 0;
		}

		//Mark each connecting parts with a number
		//There may be different numbers within a connecting part
		for (int k = 0; k < lengthOfGreyScale; k++)
		{
			int x = k - (k / width) * width;
			int y = k / width;
			
			if (red[k] == 0)
			{
				if (theConnectionValueMap[k] == 0)
				{
					if (red[k + width] == 0 && theConnectionValueMap[k + width] != 0)
					{
						theConnectionValueMap[k] = theConnectionValueMap[k + width];
					}

					else if (red[k + width + 1] == 0 && theConnectionValueMap[k + width + 1] != 0)
					{
						theConnectionValueMap[k] = theConnectionValueMap[k + width + 1];
					}

					else if (red[k + 1] == 0 && theConnectionValueMap[k + 1] != 0)
					{
						theConnectionValueMap[k] = theConnectionValueMap[k + 1];
					}

					else if (red[k - width + 1] == 0 && theConnectionValueMap[k - width + 1] != 0)
					{
						theConnectionValueMap[k] = theConnectionValueMap[k - width + 1];
					}

					else if (red[k - width] == 0 && theConnectionValueMap[k - width] != 0)
					{
						theConnectionValueMap[k] = theConnectionValueMap[k - width];
					}

					else if (red[k - width - 1] == 0 && theConnectionValueMap[k - width - 1] != 0)
					{
						theConnectionValueMap[k] = theConnectionValueMap[k - width - 1];
					}

					else if (red[k - 1] == 0 && theConnectionValueMap[k - 1] != 0)
					{
						theConnectionValueMap[k] = theConnectionValueMap[k - 1];
					}

					else if (red[k + width - 1] == 0 && theConnectionValueMap[k + width - 1] != 0)
					{
						theConnectionValueMap[k] = theConnectionValueMap[k + width - 1];
					}

					else
					{
						indexOfContour++;
						theConnectionValueMap[k] = indexOfContour;
					}							
				}		
			}
		}

		int timeOfMerge = 256;

		while (timeOfMerge > 0)
		{
			for (int k = 0; k < lengthOfGreyScale; k++)
			{
				if (theConnectionValueMap[k] != 0)
				{
					if (theConnectionValueMap[k + width] != 0 && theConnectionValueMap[k + width] < theConnectionValueMap[k])
					{
						theConnectionValueMap[k] = theConnectionValueMap[k + width];
					}

					else if (theConnectionValueMap[k + width + 1] != 0 && theConnectionValueMap[k + width + 1] < theConnectionValueMap[k])
					{
						theConnectionValueMap[k] = theConnectionValueMap[k + width + 1];
					}

					else if (theConnectionValueMap[k + 1] != 0 && theConnectionValueMap[k + 1] < theConnectionValueMap[k])
					{
						theConnectionValueMap[k] = theConnectionValueMap[k + 1];
					}

					else if (theConnectionValueMap[k - width + 1] != 0 && theConnectionValueMap[k - width + 1] < theConnectionValueMap[k])
					{
						theConnectionValueMap[k] = theConnectionValueMap[k - width + 1];
					}

					else if (theConnectionValueMap[k - width] != 0 && theConnectionValueMap[k - width] < theConnectionValueMap[k])
					{
						theConnectionValueMap[k] = theConnectionValueMap[k - width];
					}

					else if (theConnectionValueMap[k - width - 1] != 0 && theConnectionValueMap[k - width - 1] < theConnectionValueMap[k])
					{
						theConnectionValueMap[k] = theConnectionValueMap[k - width - 1];
					}

					else if (theConnectionValueMap[k - 1] != 0 && theConnectionValueMap[k - 1] < theConnectionValueMap[k])
					{
						theConnectionValueMap[k] = theConnectionValueMap[k - 1];
					}

					else if (theConnectionValueMap[k + width - 1] != 0 && theConnectionValueMap[k + width - 1] < theConnectionValueMap[k])
					{
						theConnectionValueMap[k] = theConnectionValueMap[k + width - 1];
					}
				}
			}

			timeOfMerge--;
		}

		connectionValueMap = theConnectionValueMap;

		//Set up initial number of connections
		int *markedNumbers = new int[10];

		initialNumberOfConnections = 0;

		for (int i = 0; i < 10; i++)
		{
			markedNumbers[i] = 0;
		}

		for (int k = 0; k < lengthOfGreyScale; k++)
		{
			if (connectionValueMap[k] != 0)
			{
				int it = 0;

				while (1)
				{
					if (markedNumbers[it] == 0)
					{
						markedNumbers[it] = connectionValueMap[k];
						break;
					}
					else if (markedNumbers[it] == connectionValueMap[k])
					{
						break;
					}
					else
					{
						it++;
					}
				}
			}
		}

		for (int i = 0; i < 10; i++)
		{
			if (markedNumbers[i] == 0)
			{
				initialNumberOfConnections = i;
				break;
			}
		}

		//Set up values of connections
		valuesOfConnections = new int[initialNumberOfConnections];

		for (int i = 0; i < width * height; i++)
		{
			if (connectionValueMap[i] != 0)
			{
				for (int j = 0; j < initialNumberOfConnections; j++)
				{
					if (valuesOfConnections[j] == 0)
					{
						valuesOfConnections[j] = connectionValueMap[i];
						break;
					}
					else if (valuesOfConnections[j] == connectionValueMap[i])
					{
						break;
					}
				}
			}
		}

		//Set up weight map
		weightMap = getWeight(preprocessedImage);

		//Set up blurred weight map
		blurredWeightMap = getBlurredWeight(weightMap, 5);

		//Set up maximum blurred weight value
		maxBlurredWeight = 0;

		for (int i = 0; i < height * width; i++)
		{
			if (blurredWeightMap[i] > maxBlurredWeight)
			{
				maxBlurredWeight = blurredWeightMap[i];
			}
		}

		//Set up normalized blurred weight map
		normalizedBlurredWeightMap = new double[height * width];

		for (int index = 0; index < height * width; index++)
		{
			normalizedBlurredWeightMap[index] = blurredWeightMap[index] * 1.0 / maxBlurredWeight;
		}

		//Set up blurred weight image
		unsigned char *blurredWeightRed = new unsigned char[width * height];
		unsigned char *blurredWeightGreen = new unsigned char[width * height];
		unsigned char *blurredWeightBlue = new unsigned char [width * height];

		for (int index = 0; index < height * width; index++)
		{
			blurredWeightRed[index] = 255 * (blurredWeightMap[index] / (maxBlurredWeight * 1.0));
			blurredWeightGreen[index] = 255 * (blurredWeightMap[index] / (maxBlurredWeight * 1.0));
			blurredWeightBlue[index] = 255 * (blurredWeightMap[index] / (maxBlurredWeight * 1.0));
		}

		blurredWeightImage = getCombination(blurredWeightRed, blurredWeightGreen, blurredWeightBlue);

		//Set up size of Gabor kernel
		sizeOfGaborKernel = 5;

		//Set up Gabor kernels
		gaborRealHrzt = new double[sizeOfGaborKernel * sizeOfGaborKernel];
		gaborImaginaryHrzt = new double[sizeOfGaborKernel * sizeOfGaborKernel];
		gaborRealLDgn = new double[sizeOfGaborKernel * sizeOfGaborKernel];
		gaborImaginaryLDgn = new double[sizeOfGaborKernel * sizeOfGaborKernel];
		gaborRealVtc = new double[sizeOfGaborKernel * sizeOfGaborKernel];
		gaborImaginaryVtc = new double[sizeOfGaborKernel * sizeOfGaborKernel];
		gaborRealRDgn = new double[sizeOfGaborKernel * sizeOfGaborKernel];
		gaborImaginaryRDgn = new double[sizeOfGaborKernel * sizeOfGaborKernel];

		for (int index = 0; index < sizeOfGaborKernel * sizeOfGaborKernel; index++)
		{
			int x = index - (index / sizeOfGaborKernel) * sizeOfGaborKernel;
			int y = index / sizeOfGaborKernel;

			gaborRealHrzt[index] = gaborRe(x - sizeOfGaborKernel / 2, y - sizeOfGaborKernel / 2, 4, 0, 0, 8, 20);
			gaborImaginaryHrzt[index] = gaborIm(x - sizeOfGaborKernel / 2, y - sizeOfGaborKernel / 2, 4, 0, 0, 8, 20);
			gaborRealLDgn[index] = gaborRe(x - sizeOfGaborKernel / 2, y - sizeOfGaborKernel / 2, 4, PI / 4, 0, 8, 20);
			gaborImaginaryLDgn[index] = gaborIm(x - sizeOfGaborKernel / 2, y - sizeOfGaborKernel / 2, 4, PI / 4, 0, 8, 20);
			gaborRealVtc[index] = gaborRe(x - sizeOfGaborKernel / 2, y - sizeOfGaborKernel / 2, 4, PI / 2, 0, 8, 20);
			gaborImaginaryVtc[index] = gaborIm(x - sizeOfGaborKernel / 2, y - sizeOfGaborKernel / 2, 4, PI / 2, 0, 8, 20);
			gaborRealRDgn[index] = gaborRe(x - sizeOfGaborKernel / 2, y - sizeOfGaborKernel / 2, 4, 3 * PI / 4, 0, 8, 20);
			gaborImaginaryRDgn[index] = gaborIm(x - sizeOfGaborKernel / 2, y - sizeOfGaborKernel / 2, 4, 3 * PI / 4, 0, 8, 20);		
		}

		//Set up Gabor filtered images
		gaborFilteredImageHrzt = dilation(erosion(toBinary(convolve(whiteBoundedImage, gaborRealHrzt, gaborImaginaryHrzt, sizeOfGaborKernel))));
		gaborFilteredImageLDgn = dilation(erosion(toBinary(convolve(whiteBoundedImage, gaborRealLDgn, gaborImaginaryLDgn, sizeOfGaborKernel))));
		gaborFilteredImageVtc = dilation(erosion(toBinary(convolve(whiteBoundedImage, gaborRealVtc, gaborImaginaryVtc, sizeOfGaborKernel))));
		gaborFilteredImageRDgn = dilation(erosion(toBinary(convolve(whiteBoundedImage, gaborRealRDgn, gaborImaginaryRDgn, sizeOfGaborKernel))));
	
		//Set up numbers of points on different orientations in different parts of the image
		unsigned char *gaborFilteredImageHrztGrey = getChannel(gaborFilteredImageHrzt, 0);
		unsigned char *gaborFilteredImageLDgnGrey = getChannel(gaborFilteredImageLDgn, 0);
		unsigned char *gaborFilteredImageVtcGrey = getChannel(gaborFilteredImageVtc, 0);
		unsigned char *gaborFilteredImageRDgnGrey = getChannel(gaborFilteredImageRDgn, 0);

		numOfHrztPointsOnTop = 0;
		numOfHrztPointsAtBottom = 0;
		numOfLDgnPointsOnLeftTop = 0;
		numOfLDgnPointsOnRightBottom = 0;
		numOfVtcPointsOnLeft = 0;
		numOfVtcPointsOnRight = 0;
		numOfRDgnPointsOnLeftBottom = 0;
		numOfRDgnPointsOnRightTop = 0;

		for (int index = 0; index < height * width; index++)
		{
			int x = index - (index / width) * width;
			int y = index / width;
			
			if (gaborFilteredImageHrztGrey[index] < 128)
			{
				if (y > height / 2)
				{
					numOfHrztPointsOnTop++;
				}
				else
				{
					numOfHrztPointsAtBottom++;
				}
			}

			if (gaborFilteredImageLDgnGrey[index] < 128)
			{
				if (y > x)
				{
					numOfLDgnPointsOnLeftTop++;
				}
				else
				{
					numOfLDgnPointsOnRightBottom++;
				}
			}

			if (gaborFilteredImageVtcGrey[index] < 128)
			{
				if (x <= width / 2)
				{
					numOfVtcPointsOnLeft++;
				}
				else
				{
					numOfVtcPointsOnRight++;
				}
			}

			if (gaborFilteredImageRDgnGrey[index] < 128)
			{
				if (x + y < height - 1)
				{
					numOfRDgnPointsOnLeftBottom++;
				}
				else
				{
					numOfRDgnPointsOnRightTop++;
				}
			}
		}

		sumOfNumbers = numOfHrztPointsOnTop + numOfHrztPointsAtBottom
					 + numOfLDgnPointsOnLeftTop + numOfLDgnPointsOnRightBottom
					 + numOfVtcPointsOnLeft + numOfVtcPointsOnRight
					 + numOfRDgnPointsOnLeftBottom + numOfRDgnPointsOnRightTop;

		meanOfNumbers = sumOfNumbers / 8.0;

		normalizedDistribution = new double[8];
		normalizedDistribution[0] = numOfHrztPointsOnTop / meanOfNumbers;
		normalizedDistribution[1] = numOfHrztPointsAtBottom / meanOfNumbers;
		normalizedDistribution[2] = numOfLDgnPointsOnLeftTop / meanOfNumbers;
		normalizedDistribution[3] = numOfLDgnPointsOnRightBottom / meanOfNumbers;
		normalizedDistribution[4] = numOfVtcPointsOnLeft / meanOfNumbers;
		normalizedDistribution[5] = numOfVtcPointsOnRight / meanOfNumbers;
		normalizedDistribution[6] = numOfRDgnPointsOnLeftBottom / meanOfNumbers;
		normalizedDistribution[7] = numOfRDgnPointsOnRightTop / meanOfNumbers;

		//Set up position of gravity center
		xOfGravityCenter = xOfCenterOfGravity(whiteBoundedImage);
		yOfGravityCenter = yOfCenterOfGravity(whiteBoundedImage);
	}

	virtual ~ChineseCharacter()
	{
	}
	
	unsigned char *getImage()
	{
		return image;
	}

	unsigned char *getBinaryImage()
	{
		return binaryImage;
	}

	unsigned char *getWhiteBoundedImage()
	{
		return whiteBoundedImage;
	}

	unsigned char *getPreprocessedImage()
	{
		return preprocessedImage;
	}

	unsigned char *getSegmentedImage()
	{
		return segmentedImage;
	}

	int getLeftMostX()
	{
		int lengthOfGreyScale = width * height;

		unsigned char *red = getChannel(image, 0);

		int XL = width - 1;

		for (int k = 0; k < lengthOfGreyScale; k++)
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

	int getRightMostX()
	{
		int lengthOfGreyScale = width * height;

		unsigned char *red = getChannel(image, 0);

		int XR = 0;

		for (int k = 0; k < lengthOfGreyScale; k++)
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

	int getUpMostY()
	{
		int lengthOfGreyScale = width * height;

		unsigned char *red = getChannel(image, 0);

		int YU = 0;

		for (int k = 0; k < lengthOfGreyScale; k++)
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

	int getBottomMostY()
	{
		int lengthOfGreyScale = width * height;

		unsigned char *red = getChannel(image, 0);

		int YB = height - 1;

		for (int k = 0; k < lengthOfGreyScale; k++)
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

	unsigned char *getConnectionValueMap()
	{
		//For tesing: print the theConnectionValueMap
		for (int k = 0; k < width * height; k++)
		{
			int x = k - (k / width) * width;
			int y = k / width;

			if (connectionValueMap[k] == 0)
			{
				printf(" ");
			}
			else
			{
				printf("%d", connectionValueMap[k]);
			}
			if (x == width - 1)
			{
				printf("\n");
			}
		}

		return connectionValueMap;
	}

	int getInitialNumberOfConnections()
	{
		return initialNumberOfConnections;
	}

	int *getValuesOfConnections()
	{
		//For testing
		for (int i = 0; i < initialNumberOfConnections; i++)
		{
			printf("%d,", valuesOfConnections[i]);

			if (i == 7)
			{
				printf("\n");
			}
		}

		return valuesOfConnections;
	}

	float *getQMap()
	{
		return QMap;
	}

	unsigned char *getGMap()
	{
		return GMap;
	}

	unsigned char *getWeightMap()
	{
		return weightMap;
	}

	int getMaxBlurredWeight()
	{
		//For testing:
		printf("Max blurred weight: %d", maxBlurredWeight);

		return maxBlurredWeight;
	}

	double *getBlurredWeightMap()
	{
		return blurredWeightMap;
	}

	double *getNormalizedBlurredWeightMap()
	{
		return normalizedBlurredWeightMap;
	}

	unsigned char *getBlurredWeightImage()
	{
		return blurredWeightImage;
	}

	unsigned char *getGaborFilteredImageHrzt()
	{
		return gaborFilteredImageHrzt;
	}

	unsigned char *getGaborFilteredImageLDgn()
	{
		return gaborFilteredImageLDgn;
	}

	unsigned char *getGaborFilteredImageVtc()
	{
		return gaborFilteredImageVtc;
	}

	unsigned char *getGaborFilteredImageRDgn()
	{
		return gaborFilteredImageRDgn;
	}

	int getNumOfHrztPointsOnTop()
	{
		return numOfHrztPointsOnTop;
	}

	int getNumOfHrztPointsAtBottom()
	{
		return numOfHrztPointsAtBottom;
	}

	int getNumOfLDgnPointsOnLeftTop()
	{
		return numOfLDgnPointsOnLeftTop;
	}

	int getNumOfLDgnPointsOnRightBottom()
	{
		return numOfLDgnPointsOnRightBottom;
	}

	int getNumOfVtcPointsOnLeft()
	{
		return numOfVtcPointsOnLeft;
	}

	int getNumOfVtcPointsOnRight()
	{
		return numOfVtcPointsOnRight;
	}

	int getNumOfRDgnPointsOnLeftBottom()
	{
		return numOfRDgnPointsOnLeftBottom;
	}

	int getNumOfRDgnPointsOnRightTop()
	{
		return numOfRDgnPointsOnRightTop;
	}

	double *getNormalizedDistribution()
	{
		return normalizedDistribution;
	}

/*	//For testing:
	void printDistribution()
	{
		printf("Hrzt on top:\n");
		printf("%d\n", numOfHrztPointsOnTop);
		printf("Hrzt at bottom:\n");
		printf("%d\n", numOfHrztPointsAtBottom);
		printf("LDgn on lefttop:\n");
		printf("%d\n", numOfLDgnPointsOnLeftTop);
		printf("LDgn on rightbottom:\n");
		printf("%d\n", numOfLDgnPointsOnRightBottom);
		printf("Vtc on left:\n");
		printf("%d\n", numOfVtcPointsOnLeft);
		printf("Vtc on right:\n");
		printf("%d\n", numOfVtcPointsOnRight);
		printf("RDgn on leftbottom:\n");
		printf("%d\n", numOfRDgnPointsOnLeftBottom);
		printf("RDgn on righttop:\n");
		printf("%d\n", numOfRDgnPointsOnRightTop);
	}

	void printNormalizedDistribution()
	{
		for (int i = 0; i < 8; i++)
		{
			printf("%3f, ", normalizedDistribution[i]);
		}
	}*/

	int getXOfGravityCenter()
	{
		//For testing:
		printf("%d ", xOfGravityCenter);

		return xOfGravityCenter;
	}

	int getYOfGravityCenter()
	{
		//For testing:
		printf("%d ", yOfGravityCenter);
		
		return yOfGravityCenter;
	}

private:
	unsigned char *image;
	unsigned char *binaryImage;
	unsigned char *whiteBoundedImage;
	unsigned char *preprocessedImage;
	
	int initialNumberOfConnections;
	int *valuesOfConnections;
	unsigned char *connectionValueMap;
	
	float *QMap;
	unsigned char *GMap;
	unsigned char *segmentedImage;
	unsigned char *weightMap;
	int maxBlurredWeight;
	double *blurredWeightMap;
	double *normalizedBlurredWeightMap;
	unsigned char *blurredWeightImage;
	
	int sizeOfGaborKernel;
	double *gaborRealHrzt;
	double *gaborImaginaryHrzt;
	double *gaborRealLDgn;
	double *gaborImaginaryLDgn;
	double *gaborRealVtc;
	double *gaborImaginaryVtc;
	double *gaborRealRDgn;
	double *gaborImaginaryRDgn;
	unsigned char *gaborFilteredImageHrzt;
	unsigned char *gaborFilteredImageLDgn;
	unsigned char *gaborFilteredImageVtc;
	unsigned char *gaborFilteredImageRDgn;

	int numOfHrztPointsOnTop;
	int numOfHrztPointsAtBottom;
	int numOfLDgnPointsOnLeftTop;
	int numOfLDgnPointsOnRightBottom;
	int numOfVtcPointsOnLeft;
	int numOfVtcPointsOnRight;
	int numOfRDgnPointsOnLeftBottom;
	int numOfRDgnPointsOnRightTop;
	int sumOfNumbers;
	double meanOfNumbers;
	double *normalizedDistribution;

	int xOfGravityCenter;
	int yOfGravityCenter;
};

class IndependentChineseCharacter:ChineseCharacter
{
public:
	IndependentChineseCharacter();
};

class ComplexChineseCharacter:ChineseCharacter
{
public:
	ComplexChineseCharacter();

	string getRadical()
	{
		return radical;
	}

	void setRadical(string theRadical)
	{
		radical = theRadical;
	}

private:
	string radical;
};

class Radical:IndependentChineseCharacter
{
public:
	Radical();
};

class SimpleChineseCharacter:IndependentChineseCharacter
{
public:
	SimpleChineseCharacter();
};

class HorizontalChineseCharacter:ComplexChineseCharacter
{
public:
	HorizontalChineseCharacter();
};

class LeftRightChineseCharacter:HorizontalChineseCharacter
{
public:
	LeftRightChineseCharacter();
};

class LeftUpBottomChineseCharacter:LeftRightChineseCharacter
{
public:
	LeftUpBottomChineseCharacter();
};

class RightLeftChineseCharacter:HorizontalChineseCharacter
{
public:
	RightLeftChineseCharacter();
};

class RightUpBottomChineseCharacters:RightLeftChineseCharacter
{
public:
	RightUpBottomChineseCharacters();
};

class VerticalChineseCharacter:ComplexChineseCharacter
{
public:
	VerticalChineseCharacter();
};

class UpBottomChineseCharacter:VerticalChineseCharacter
{
public:
	UpBottomChineseCharacter();
};

class UpLeftRightChineseCharacter:UpBottomChineseCharacter
{
public:
	UpLeftRightChineseCharacter();
};

class BottomUpChineseCharacter:VerticalChineseCharacter
{
public:
	BottomUpChineseCharacter();
};

class BottomLeftRightChineseCharacter:BottomUpChineseCharacter
{
public:
	BottomLeftRightChineseCharacter();
};

class SurroundingChineseCharacter:ComplexChineseCharacter
{
public:
	SurroundingChineseCharacter();
};

class HalfsurroundingChineseCharacter:ComplexChineseCharacter
{
public:
	HalfsurroundingChineseCharacter();
};