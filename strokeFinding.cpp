#include <stdio.h>
#include <stdlib.h>
#include "mathMethods.cpp"

unsigned char *getNumberOfConditionsForQ(unsigned char *binary)
{
	int lengthOfGreyScale = width * height;

	unsigned char *numberOfCondistions = new unsigned char[lengthOfGreyScale];

	unsigned char *grey = getChannel(binary, 0);

	for (int k = 0; k < lengthOfGreyScale; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;

		numberOfCondistions[k] = 0;

		if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
		{
			numberOfCondistions[k] = 0;
		}
		else
		{
			//Condition 1
			if (grey[k - 1] == 0 && grey[k + 1] == 0)
			{
				numberOfCondistions[k] += pow(2, 0);
			}

			//condition 2
			if (grey[k + width + 1] == 0 && grey[k - width - 1] == 0)
			{
				numberOfCondistions[k] += pow(2, 1);
			}

			//condition 3
			if (grey[k + width] == 0 && grey[k - width] == 0)
			{
				numberOfCondistions[k] += pow(2, 2);
			}

			//condition 4
			if (grey[k + width - 1] == 0 && grey[k - width + 1] == 0)
			{
				numberOfCondistions[k] += pow(2, 3);
			}

			//condition 5
			if ((grey[k + 1] == 0 && grey[k - width - 1] == 0) || (grey[k - 1] == 0 && grey[k + width + 1] == 0))
			{
				numberOfCondistions[k] += pow(2, 4);
			}

			//condition 6
			if ((grey[k + width] == 0 && grey[k - width - 1] == 0) || (grey[k + width + 1] == 0 && grey[k - width] == 0))
			{
				numberOfCondistions[k] += pow(2, 5);
			}

			//condition 7
			if ((grey[k + width] == 0 && grey[k - width + 1] == 0) || (grey[k + width - 1] == 0 && grey[k - width] == 0))
			{
				numberOfCondistions[k] += pow(2, 6);
			}

			//condition 8
			if ((grey[k + width - 1] == 0 && grey[k + 1] == 0) || (grey[k - width + 1] == 0 && grey[k - 1] == 0))
			{
				numberOfCondistions[k] += pow(2, 7);
			}
		}
	}

	return numberOfCondistions;
}

float *getQ(unsigned char *binary)
{
	int lengthOfGreyScale = width * height;

	unsigned char *B = getBValue(binary);
	unsigned char *numberOfCondistions = getNumberOfConditionsForQ(binary);

	unsigned char *grey = getChannel(binary, 0);

	float *Q = new float[lengthOfGreyScale];

	//Step 1
	for (int k = 0; k < lengthOfGreyScale; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;
	
		if (grey[k] == 255 || numberOfCondistions[k] == 0)
		{
			//When the pixel is white or terminal points
			Q[k] = -1;
		}
		else if (grey[k] == 0 && B[k] >= 3)
		{
			Q[k] = -45;
		}
		else
		{
			switch(numberOfCondistions[k])
			{
				case 1:
				{
					//Only condition 1 is satisfied
					Q[k] = 0;
					break;
				}
				case 2:
				{
					//Only condition 2 is satisfied
					Q[k] = 45;
					break;
				}
				case 4:
				{
					//Only condition 3 is satisfied
					Q[k] = 90;
					break;
				}
				case 8:
				{
					//Only condition 4 is satisfied
					Q[k] = 135;
					break;
				}
				case 16:
				{
					//Only condition 5 is satisfied
					Q[k] = 33.5;
					break;
				}
				case 32:
				{
					//Only condition 6 is satisfied
					Q[k] = 56.5;
					break;
				}
				case 64:
				{
					//Only condition 7 is satisfied
					Q[k] = 123.5;
					break;
				}
				case 128:
				{
					//Only condition 8 is satisfied
					Q[k] = 146.5;
					break;
				}
				default:
				{
					//More than 2 conditions are satisfied
					Q[k] = -45;
					break;
				}
			}
		}
	}

	//Step 2
	for (int k = 0; k < lengthOfGreyScale; k++)
	{
		if (grey[k] == 0 && B[k] == 1)
		{
			if (grey[k + width] == 0)
			{
				Q[k] = Q[k + width];
			}

			if (grey[k + width + 1] == 0)
			{
				Q[k] = Q[k + width + 1];
			}

			if (grey[k + 1] == 0)
			{
				Q[k] = Q[k + 1];
			}

			if (grey[k - width + 1] == 0)
			{
				Q[k] = Q[k - width + 1];
			}

			if (grey[k - width] == 0)
			{
				Q[k] = Q[k - width];
			}

			if (grey[k - width - 1] == 0)
			{
				Q[k] = Q[k - width - 1];
			}

			if (grey[k - 1] == 0)
			{
				Q[k] = Q[k - 1];
			}

			if (grey[k + width -1] == 0)
			{
				Q[k] = Q[k + width - 1];
			}
		}
	}

	//Step 3
	float *tempQ = new float[lengthOfGreyScale];

	for (int k = 0; k < lengthOfGreyScale; k++)
	{
		if (Q[k] == -1 || (grey[k] == 0 && B[k] == 1) || Q[k] == -45)
		{
			tempQ[k] = Q[k];
		}
		else
		{
			float sum = Q[k];
			int num = 1;

			if (Q[k + width] >= 0)
			{
				num++;

				if (Q[k + width] - Q[k] > 90)
				{
					sum += (Q[k + width] - 180);
				}

				else
				{
					if (Q[k + width] - Q[k] <= -90)
					{
						sum += (Q[k + width] + 180);
					}

					else
					{
						sum += Q[k + width];
					}
				}
			}

			if (Q[k + width + 1] >= 0)
			{
				num++;

				if (Q[k + width + 1] - Q[k] > 90)
				{
					sum += (Q[k + width + 1] - 180);
				}

				else
				{
					if (Q[k + width + 1] - Q[k] <= -90)
					{
						sum += (Q[k + width + 1] + 180);
					}

					else
					{
						sum += Q[k + width + 1];
					}
				}
			}

			if (Q[k + 1] >= 0)
			{
				num++;

				if (Q[k + 1] - Q[k] > 90)
				{
					sum += (Q[k + 1] - 180);
				}

				else
				{
					if (Q[k + 1] - Q[k] <= -90)
					{
						sum += (Q[k + 1] + 180);
					}

					else
					{
						sum += Q[k + 1];
					}
				}
			}

			if (Q[k - width + 1] >= 0)
			{
				num++;

				if (Q[k - width + 1] - Q[k] > 90)
				{
					sum += (Q[k - width + 1] - 180);
				}

				else if (Q[k - width + 1] - Q[k] <= -90)
				{
					sum += (Q[k - width + 1] + 180);
				}

				else
				{
					sum += Q[k - width + 1];
				}
			}

			if (Q[k - width] >= 0)
			{
				num++;

				if (Q[k - width] - Q[k] > 90)
				{
					sum += (Q[k - width] - 180);
				}

				else if (Q[k - width] - Q[k] <= -90)
				{
					sum += (Q[k - width] + 180);
				}

				else
				{
					sum += Q[k - width];
				}
			}

			if (Q[k - width - 1] >= 0)
			{
				num++;

				if (Q[k - width - 1] - Q[k] > 90)
				{
					sum += (Q[k - width - 1] - 180);
				}

				else if (Q[k - width - 1] - Q[k] <= -90)
				{
					sum += (Q[k - width - 1] + 180);
				}

				else
				{
					sum += Q[k - width - 1];
				}
			}

			if (Q[k - 1] >= 0)
			{
				num++;

				if (Q[k - 1] - Q[k] > 90)
				{
					sum += (Q[k - 1] - 180);
				}

				else if (Q[k - 1] - Q[k] <= -90)
				{
					sum += (Q[k - 1] + 180);
				}

				else
				{
					sum += Q[k - 1];
				}
			}

			if (Q[k + width - 1] >= 0)
			{
				num++;

				if (Q[k + width - 1] - Q[k] > 90)
				{
					sum += (Q[k + width - 1] - 180);
				}

				else 
				{
					if (Q[k + width - 1] - Q[k] <= -90)
					{
						sum += (Q[k + width - 1] + 180);
					}

					else
					{
						sum += Q[k + width - 1];
					}
				}
			}

			tempQ[k] = (sum * 1.0) / (num * 1.0);
		}
	}

	for (int k = 0; k < lengthOfGreyScale; k++)
	{
		Q[k] = tempQ[k];
	}

	//Step 4
	//Step 2
	for (int k = 0; k < lengthOfGreyScale; k++)
	{
		if (grey[k] == 0 && B[k] == 1)
		{
			if (grey[k + width] == 0)
			{
				Q[k] = Q[k + width];
			}

			if (grey[k + width + 1] == 0)
			{
				Q[k] = Q[k + width + 1];
			}

			if (grey[k + 1] == 0)
			{
				Q[k] = Q[k + 1];
			}

			if (grey[k - width + 1] == 0)
			{
				Q[k] = Q[k - width + 1];
			}

			if (grey[k - width] == 0)
			{
				Q[k] = Q[k - width];
			}

			if (grey[k - width - 1] == 0)
			{
				Q[k] = Q[k - width - 1];
			}

			if (grey[k - 1] == 0)
			{
				Q[k] = Q[k - 1];
			}

			if (grey[k + width -1] == 0)
			{
				Q[k] = Q[k + width - 1];
			}
		}
	}

	//For testing:
/*	printf("Q\n");

	for (int k = 0; k < lengthOfGreyScale; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;

		if (Q[k] == -45)
		{
			printf("-");
		}
		else if (Q[k] == -1)
		{
			printf(" ");
		}
		else
		{
			printf("Q");
		}

		if (x == width - 1)
		{
			printf("\n");
		}
	}*/

	return Q;
}

unsigned char *getG(float *Q)
{
	int lengthOfGreyScale = width * height;

	unsigned char *G = new unsigned char[lengthOfGreyScale];

	for (int k = 0; k < lengthOfGreyScale; k++)
	{
		if (Q[k] == -45)
		{
			G[k] = '-';
		}
		else if (Q[k] == -1)
		{
			G[k] = ' ';
		}
		else
		{
			if ((Q[k] < 22.5 && Q[k] >= -22.5) || (Q[k] <= 202.5 && Q[k] >= 157.5))
			{
				G[k] = 'H';
			}
			else if (Q[k] >= 22.5 && Q[k] < 67.5)
			{
				G[k] = 'R';
			}
			else if (Q[k] >= 67.5 && Q[k] < 112.5)
			{
				G[k] = 'V';
			}
			else if (Q[k] >= 112.5 && Q[k] < 157.5)
			{
				G[k] = 'L';
			}
		}
	}

	//For testing:
/*	printf("G\n");

	for (int k = 0; k < lengthOfGreyScale; k++)
	{
		int x = k - (k / width) * width;
		int y = k / width;

		printf("%c", G[k]);

		if (x == width - 1)
		{
			printf("\n");
		}
	}*/

	return G;
}

/*Assigns weight value to points in the image
 *Points around terminal points or intersections of stroke segments will get high weight values
 *The more stroke segments intersect at a point, the higher the weight value at that point
 */
unsigned char *getWeight(unsigned char *binary)
{
	unsigned char *weight = new unsigned char[height * width];
	unsigned char *grey = getChannel(binary, 0);
	float *Q = getQ(binary);
	unsigned char *G = getG(Q);

	for (int index = 0; index < height * width; index++)
	{
		weight[index] = 0;
	}

	for (int index = 0; index < height * width; index++)
	{
		int i = index - (index / width) * width;
		int j = index / width;
		int l;
		int k;
		int lapo;
		int kapo;

		if (grey[index] == 0 && 
			((G[index] == G[index + 1] && G[index] != G[index - width + 1] && G[index] != G[index - width] && G[index] != G[index - width - 1] && G[index] != G[index - 1] && G[index] != G[index + width - 1] && G[index] != G[index + width] && G[index] != G[index + width + 1])
		  || (G[index] == G[index - width + 1] && G[index] != G[index - width] && G[index] != G[index - width - 1] && G[index] != G[index - 1] && G[index] != G[index + width - 1] && G[index] != G[index + width] && G[index] != G[index + width + 1] && G[index] != G[index + 1])
		  || (G[index] == G[index - width] && G[index] != G[index - width - 1] && G[index] != G[index - 1] && G[index] != G[index + width - 1] && G[index] != G[index + width] && G[index] != G[index + width + 1] && G[index] != G[index + 1] && G[index] != G[index - width + 1])
		  || (G[index] == G[index - width - 1] && G[index] != G[index - 1] && G[index] != G[index + width - 1] && G[index] != G[index + width] && G[index] != G[index + width + 1] && G[index] != G[index + 1] && G[index] != G[index - width + 1] && G[index] != G[index - width])
		  || (G[index] == G[index - 1] && G[index] != G[index + width - 1] && G[index] != G[index + width] && G[index] != G[index + width + 1] && G[index] != G[index + 1] && G[index] != G[index - width + 1] && G[index] != G[index - width] && G[index] != G[index - width - 1])
		  || (G[index] == G[index + width - 1] && G[index] != G[index + width] && G[index] != G[index + width + 1] && G[index] != G[index + 1] && G[index] != G[index - width + 1] && G[index] != G[index - width] && G[index] != G[index - width - 1] && G[index] != G[index - 1])
		  || (G[index] == G[index + width] && G[index] != G[index + width + 1] && G[index] != G[index + 1] && G[index] != G[index - width + 1] && G[index] != G[index - width] && G[index] != G[index - width - 1] && G[index] != G[index - 1] && G[index] != G[index + width - 1])
		  || (G[index] == G[index + width + 1] && G[index] != G[index + 1] && G[index] != G[index - width + 1] && G[index] != G[index - width] && G[index] != G[index - width - 1] && G[index] != G[index - 1] && G[index] != G[index + width - 1] && G[index] != G[index + width])))
		{
			weight[index] = 2;

			if (G[index] == 'H')
			{
				if (G[index + width - 1] == 'H' || G[index - 1] == 'H' || G[index - width - 1] == 'H')
				{
					l = i + 1;
					k = j;
					lapo = i + 2;
					kapo = j;

					if (G[k * width + l] == '-')
					{
						weight[k * width + l] = weight[k * width + l] + 3;
						weight[kapo * width + lapo] = weight[kapo * width + lapo] + 4;
					}
					else
					{
						weight[k * width + l] = weight[k * width + l] + 4;
					}
				}

				if (G[index + width + 1] == 'H' || G[index + 1] == 'H' || G[index + width + 1] == 'H')
				{
					l = i - 1;
					k = j;
					lapo = i - 2;
					kapo = j;

					if (G[k * width + l] == '-')
					{
						weight[k * width + l] = weight[k * width + l] + 3;
						weight[kapo * width + lapo] = weight[kapo * width + lapo] + 4;
					}
					else
					{
						weight[k * width + l] = weight[k * width + l] + 4;
					}
				}
			}

			if (G[index] == 'V')
			{
				if (G[index + width - 1] == 'V' || G[index + width] == 'V' || G[index + width + 1] == 'V')
				{
					l = i;
					k = j - 1;
					lapo = i;
					kapo = j - 2;

					if (G[k * width + l] == '-')
					{
						weight[k * width + l] = weight[k * width + l] + 3;
						weight[kapo * width + lapo] = weight[kapo * width + lapo] + 4;
					}
					else
					{
						weight[k * width + l] = weight[k * width + l] + 4;
					}					
				}

				if (G[index - width - 1] == 'V' || G[index - width] == 'V' || G[index - width + 1] == 'V')
				{
					l = i;
					k = j + 1;
					lapo = i;
					kapo = j + 2;

					if (G[k * width + l] == '-')
					{
						weight[k * width + l] = weight[k * width + l] + 3;
						weight[kapo * width + lapo] = weight[kapo * width + lapo] + 4;
					}
					else
					{
						weight[k * width + l] = weight[k * width + l] + 4;
					}					
				}
			}

			if (G[index] == 'L')
			{
				if (G[index + width] == 'L' || G[index + width - 1] == 'L' || G[index - 1] == 'L')
				{
					l = i + 1;
					k = j - 1;
					lapo = i + 2;
					kapo = j - 2;

					if (G[k * width + l] == '-')
					{
						weight[k * width + l] = weight[k * width + l] + 3;
						weight[kapo * width + lapo] = weight[kapo * width + lapo] + 4;
					}
					else
					{
						weight[k * width + l] = weight[k * width + l] + 4;
					}				
				}

				if (G[index - width] == 'L' || G[index - width + 1] == 'L' || G[index + 1] == 'L')
				{
					l = i - 1;
					k = j + 1;
					lapo = i + 2;
					kapo = j - 2;

					if (G[k * width + l] == '-')
					{
						weight[k * width + l] = weight[k * width + l] + 3;
						weight[kapo * width + lapo] = weight[kapo * width + lapo] + 4;
					}
					else
					{
						weight[k * width + l] = weight[k * width + l] + 4;
					}					
				}
			}

			if (G[index] == 'R')
			{
				if (G[index + width] == 'R' || G[index + width + 1] == 'R' || G[index + 1] == 'R')
				{
					l = i - 1;
					k = j - 1;
					lapo = i - 2;
					kapo = j - 2;

					if (G[k * width + l] == '-')
					{
						weight[k * width + l] = weight[k * width + l] + 3;
						weight[kapo * width + lapo] = weight[kapo * width + lapo] + 4;
					}
					else
					{
						weight[k * width + l] = weight[k * width + l] + 4;
					}					
				}

				if (G[index - 1] == 'R' || G[index - width - 1] == 'R' || G[index - width] == 'R')
				{
					l = i + 1;
					k = j + 1;
					lapo = i + 2;
					kapo = j + 2;

					if (G[k * width + l] == '-')
					{
						weight[k * width + l] = weight[k * width + l] + 3;
						weight[kapo * width + lapo] = weight[kapo * width + lapo] + 4;
					}
					else
					{
						weight[k * width + l] = weight[k * width + l] + 4;
					}					
				}
			}
		}
	}

	for (int index = 0; index < width * height; index++)
	{
		if (grey[index] != 0 && weight[index] == 0)
		{
			weight[index] = ' ';
		}
	}

	//For testing:
/*	for (int index = 0; index < width * height; index++)
	{
		int i = index - (index / width) * width;
		int j = index / width;

		if (weight[index] == ' ')
		{
			printf(" ");
		}
		else
		{
			if (weight[index] == 0)
			{
				printf("0");
			}
			else
			{
				if (weight[index] >= 20)
				{
					printf("L");
				}
				else
				{
					if (weight[index] >= 10)
					{
						printf("M");
					}
					else
					{
						printf("%d", weight[index]);
					}
				}
			}
		}

		if (i == width - 1)
		{
			printf("\n");
		}
	}*/

	return weight;
}

double *getBlurredWeight(unsigned char *weight, double sigma)
{
	double *blurredWeight = new double[height * width];

	for (int index = 0; index < height * width; index++)
	{
		int x = index - (index / width) * width;
		int y = index / width;

		if ((x - 2) >= 0 && (x + 2) <= width - 1 && (y - 2) >= 0 && (y + 2) <= height - 1)
		{
			if (weight[index] != ' ' && weight[index] > 0)
			{
				blurredWeight[index] += weight[index] * gaussianFunction2D(0, 0, sigma);

				blurredWeight[index + width] += weight[index] * gaussianFunction2D(0, 1, sigma);
				blurredWeight[index + width + 1] += weight[index] * gaussianFunction2D(1, 1, sigma);
				blurredWeight[index + 1] += weight[index] * gaussianFunction2D(1, 0, sigma);
				blurredWeight[index - width + 1] += weight[index] * gaussianFunction2D(1, -1, sigma);
				blurredWeight[index - width] += weight[index] * gaussianFunction2D(0, -1, sigma);
				blurredWeight[index - width - 1] += weight[index] * gaussianFunction2D(-1, -1, sigma);
				blurredWeight[index - 1] += weight[index] * gaussianFunction2D(-1, 0, sigma);
				blurredWeight[index + width - 1] += weight[index] * gaussianFunction2D(-1, 1, sigma);

				blurredWeight[index + 2 * width] += weight[index] * gaussianFunction2D(0, 2, sigma);
				blurredWeight[index + 2 * width + 1] += weight[index] * gaussianFunction2D(1, 2, sigma);
				blurredWeight[index + 2 * width + 2] += weight[index] * gaussianFunction2D(2, 2, sigma);
				blurredWeight[index + width + 2] += weight[index] * gaussianFunction2D(2, 1, sigma);
				blurredWeight[index + 2] += weight[index] * gaussianFunction2D(2, 0, sigma);
				blurredWeight[index - width + 2] += weight[index] * gaussianFunction2D(2, -1, sigma);
				blurredWeight[index - 2 * width + 2] += weight[index] * gaussianFunction2D(2, -2, sigma);
				blurredWeight[index - 2 * width + 1] += weight[index] * gaussianFunction2D(1, -2, sigma);
				blurredWeight[index - 2 * width] += weight[index] * gaussianFunction2D(0, -2, sigma);
				blurredWeight[index - 2 * width - 1] += weight[index] * gaussianFunction2D(-1, -2, sigma);
				blurredWeight[index - 2 * width - 2] += weight[index] * gaussianFunction2D(-2, -2, sigma);
				blurredWeight[index - width - 2] += weight[index] * gaussianFunction2D(-2, -1, sigma);
				blurredWeight[index - 2] += weight[index] * gaussianFunction2D(-2, 0, sigma);
				blurredWeight[index + width - 2] += weight[index] * gaussianFunction2D(-2, 1, sigma);
				blurredWeight[index + 2 * width - 2] += weight[index] * gaussianFunction2D(-2, 2, sigma);
				blurredWeight[index + 2 * width - 1] += weight[index] * gaussianFunction2D(-1, 2, sigma);
			}
		}
	}

	for (int index = 0; index < height * width; index++)
	{
		blurredWeight[index] *= (2.0 * PI * pow(sigma, 2));
	}

	return blurredWeight;
}