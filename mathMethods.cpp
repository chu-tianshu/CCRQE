#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#define PI 3.1415927
#define e 2.71828

double gaussianFunction2D(double dx, double dy, double sigma)
{
	return ((1.0 / (2 * PI * pow(sigma, 2))) * pow(e, -((pow(dx, 2) + pow(dy, 2)) / (2 * pow(sigma, 2)))));
}

double difference(double *map1, double *map2, int lengthOfMap)
{
	double difference = 0;

	for (int index = 0; index < lengthOfMap; index++)
	{
		difference += abs(map1[index] - map2[index]);
	}

	return difference;
}

double euclideanDistance(double *array1, double *array2, int lengthOfArray)
{
	double squareDistance = 0;
	double distance = 0;

	for (int index = 0; index < lengthOfArray; index++)
	{
		squareDistance += pow((array1[index] - array2[index]), 2);
	}

	distance = sqrt(squareDistance);

	return distance;
}

double gaborRe(int x, int y, double lambda, double theta, double phi, double sigma, double gamma)
{
	double x_theta = x * cos(theta) + y * sin(theta);
	double y_theta = -x * sin(theta) + y * cos(theta);

	double gre = pow(e, -(pow(x_theta, 2) + pow(gamma, 2) * pow(y_theta, 2)) / (2.0 * pow(sigma, 2))) * cos(2 * PI * x_theta / lambda + phi);

	return gre;
}

double gaborIm(int x, int y, double lambda, double theta, double phi, double sigma, double gamma)
{
	double x_theta = x * cos(theta) + y * sin(theta);
	double y_theta = -x * sin(theta) + y * cos(theta);

	double gim = pow(e, -(pow(x_theta, 2) + pow(gamma, 2) * pow(y_theta, 2)) / (2.0 * pow(sigma, 2))) * sin(2 * PI * x_theta / lambda + phi);

	return gim;
}

double modulusOfComplex(double real, double imaginary)
{
	return (sqrt(pow(real, 2) + pow(imaginary, 2)));
}

unsigned char *convolve(unsigned char *image, double *kernelReal, double *kernelImaginary, int sizeOfKernel)
{
	unsigned char *grey = getChannel(image, 0);
	unsigned char *newGrey = new unsigned char[height * width];
	double *newGreyDoubleReal = new double[height * width];
	double *newGreyDoubleImaginary = new double[height * width];
	double *newGreyDouble = new double[height * width];

	for (int index = 0; index < height * width; index++)
	{
		int x = index - (index / width) * width;
		int y = index / width;

		if (x - sizeOfKernel / 2 < 0 || x + sizeOfKernel / 2 > width - 1 || y - sizeOfKernel / 2 < 0 || y + sizeOfKernel / 2 > height - 1)
		{
			newGreyDouble[index] = 255;
		}
		else
		{
			for (int i = 0; i < sizeOfKernel; i++)
			{
				for (int j = 0; j < sizeOfKernel; j++)
				{
					int indexInKernel = j * sizeOfKernel + i;

					int xInImage = x - 2 + i;
					int yInImage = y - 2 + j;

					int indexInImage = yInImage * width + xInImage;

					newGreyDoubleReal[index] += kernelReal[indexInKernel] * grey[indexInImage];
					newGreyDoubleImaginary[index] += kernelImaginary[indexInKernel] * grey[indexInImage];

					newGreyDouble[index] = modulusOfComplex(newGreyDoubleReal[index], newGreyDoubleImaginary[index]);
				}
			}
		}
	}

	for (int index = 0; index < height * width; index++)
	{
		if (newGreyDouble[index] > 255)
		{
			newGrey[index] = 255;
		}
		else if (newGreyDouble[index] < 0)
		{
			newGrey[index] = 0;
		}
		else
		{
			newGrey[index] = (int) newGreyDouble[index];
		}
	}

	unsigned char *convolved = getCombination(newGrey, newGrey, newGrey);

	return convolved;
}

double GAMMA(double x, double a, double b)
{
	if (x < a)
	{
		return 1.0;
	}
	else if (x > b)
	{
		return 0.0;
	}
	else
	{
		return ((b - x * 1.0) / (b - a));
	}
}

double LAMBDA(double x, double b, double c)
{
	if (x < b)
	{
		return 0.0;
	}
	else if (x > c)
	{
		return 1.0;
	}
	else
	{
		return ((x * 1.0 - b) / (c - b));
	}
}

double DELTA(double x, double a, double b, double c)
{
	if (x < a || x > c)
	{
		return 0.0;
	}
	else
	{
		if (x <= b)
		{
			return ((x * 1.0 - a) / (b - a));
		}
		else
		{
			return ((c - x * 1.0) / (c - b));
		}
	}
}