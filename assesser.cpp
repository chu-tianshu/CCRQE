#include <stdio.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

class Assesser
{
public:
	Assesser(ChineseCharacter sp, ChineseCharacter tplt)
	{
		//Set up FC and near/far scores
		spXC = (sp.getRightMostX() + sp.getLeftMostX()) / 2;
		spYC = (sp.getUpMostY() + sp.getBottomMostY()) / 2;
		tpltXC = (tplt.getRightMostX() + tplt.getLeftMostX()) / 2;
		tpltYC = (tplt.getUpMostY() + tplt.getBottomMostY()) / 2;

		FC = sqrt(pow((spXC - tpltXC), 2) + pow((spYC - tpltYC), 2)) / (0.5 * width);

		muonNearFC = GAMMA(FC, 0.1, 0.9);
		muonFarFC = LAMBDA(FC, 0.1, 0.9);

		//Set up FA and large/proper/small scores
		spBBWidth = sp.getWidthOfBB();
		spBBHeight = sp.getHeightOfBB();
		tpltBBWidth = tplt.getWidthOfBB();
		tpltBBHeight = tplt.getHeightOfBB();

		FA = (spBBWidth * spBBHeight - tpltBBWidth * tpltBBHeight) / (tpltBBWidth * tpltBBHeight * 1.0);

		muonSmallFA = GAMMA(FA, -1, 0);
		muonProperFA = DELTA(FA, -0.4, 0, 0.4);
		muonLargeFA = LAMBDA(FA, 0, 1);

		//Set up FR and tall/proper/short scores
		FR = spBBWidth * 1.0 / spBBHeight - tpltBBWidth * 1.0 / tpltBBHeight;

		muonTallFR = GAMMA(FR, -0.5, 0);
		muonProperFR = DELTA(FR, -0.166667, 0, 0.166667);
		muonShortFR = LAMBDA(FR, 0, 0.5);

		//Set up Dx, Dy and less/much scores
		int *spProX = sp.getProX();
		int *spProY = sp.getProY();
		int *tpltProX = tplt.getProX();
		int *tpltProY = tplt.getProY();

		spXHalfRatio = sp.getXHalfRatio();
		spYHalfRatio = sp.getYHalfRatio();
		tpltXHalfRatio = tplt.getXHalfRatio();
		tpltYHalfRatio = tplt.getYHalfRatio();

		int sumOfDiffInProX = 0;
		int sumOfSumInProX = 0;
		int sumOfDiffInProY = 0;
		int sumOfSumInProY = 0;

		for (int i = 0; i < width; i++)
		{
			sumOfDiffInProX += abs(spProX[i] - tpltProX[i]);
			sumOfSumInProX += (spProX[i] + tpltProX[i]);
		}

		for (int i = 0; i < height; i++)
		{
			sumOfDiffInProY += abs(spProY[i] - tpltProY[i]);
			sumOfSumInProY += (spProY[i] + tpltProY[i]);
		}

		Dx = sumOfDiffInProX * 1.0 / sumOfSumInProX;
		Dy = sumOfDiffInProY * 1.0 / sumOfSumInProY;

		muonLessDx = GAMMA(Dx, 0.1, 0.9);
		muonMuchDx = LAMBDA(Dx, 0.1, 0.9);
		muonLessDy = GAMMA(Dy, 0.1, 0.9);
		muonMuchDy = LAMBDA(Dy, 0.1, 0.9);

		//Set up FS and left/proper/right scores
		int spDiffSym = 0;
		int tpltDiffSym = 0;

		for (int i = 0; i < width / 2 - 1; i++)
		{
		//	printf("sample Prox at %d: %d, sample Prox at %d: %d\n", i, spProX[i], width - 1 - i, spProX[width - 1 - i]);
		//	printf("template Prox at %d: %d, template Prox at %d: %d\n", i, tpltProX[i], width - 1 - i, tpltProX[width - 1- i]);
			spDiffSym += (spProX[i] - spProX[width - 1 - i]);
			tpltDiffSym += (tpltProX[i] - tpltProX[width - 1 - i]);
		}

		FS = (abs(spDiffSym) - abs(tpltDiffSym)) / (width * 1.0);

		muonLeftFS = GAMMA(FS, -1.0, 0.0);
		muonProperFS = DELTA(FS, -0.4, 0.0, 0.4);
		muonRightFS = LAMBDA(FS, 0, 1);
	}

	void assess()
	{
		SC = muonNearFC;

		if (FA < -0.4 || FA > 0.4)
		{
			SA = 0;
		}
		else
		{
			SA = muonProperFA * 1.0 / (muonSmallFA + muonProperFA + muonLargeFA);
		}

		if (FR < -0.166667 || FR > 0.166667)
		{
			SR = 0;
		}
		else
		{
			SR = muonProperFR * 1.0 / (muonProperFR + muonTallFR + muonShortFR);
		}

		SDx = muonLessDx;
		SDy = muonLessDy;

		if (FS < -0.4 || FS > 0.4)
		{
			SS = 0;
		}
		else
		{
			SS = muonProperFS * 1.0 / (muonLeftFS + muonProperFS + muonRightFS);
		}

		S1 = SC;
		S2 = (SA + SR) / 2.0;
		S3 = (SDx + SDy + SS) / 3.0;

		printf("Your score in center is %3f\n", S1 * 100);
		printf("Your score in size is %3f\n", S2 * 100);
		printf("Your score in projection is %3f\n", S3 * 100);

		printf("Suggestions:\n");

		if (SC > 0.95)
		{
			printf("You have placed your character right in the middle. Good job!\n");
		}
		else
		{
			printf("Your character is placed a little bit far from the center of the box.\n");

			double horizontalDisplacementPercentage = abs(spXC - tpltXC) / (width * 1.0);
			double verticalDisplacementPercentage = abs(spYC - tpltYC) / (height * 1.0);

			if (spXC < tpltXC)
			{
				printf("You would need to move your character by %3f percent rightwards, and ", horizontalDisplacementPercentage * 100.0);
			}
			else if (spXC > tpltXC)
			{
				printf("You would need to move your character by %3f percent leftwards, and ", horizontalDisplacementPercentage * 100.0);
			}
			else
			{
				printf("You have placed your character correctly horizontally, but ");
			}

			if (spYC < tpltYC)
			{
				printf("you would need to move your character by %3f percent upwardes.\n", verticalDisplacementPercentage * 100.0);
			}
			else if (spYC < tpltYC)
			{
				printf("you would need to move your character by %3f percent downwards.\n", verticalDisplacementPercentage * 100.0);
			}
			else
			{
				printf("you have placed your character correctly vertically.\n");
			}
		}

		if (FA < -0.07)
		{
			printf("Your character is too small in size.\n");

			double expandPercentage = 1.0 / (1 + FA) - 1;

			printf("Try to expand the size of your character by %3f percent.\n", expandPercentage * 100);
		}
		else if (FA > 0.07)
		{
			printf("Your character is too large in size.\n");

			double shrinkPercentage = 1 - 1.0 / (1 + FA);

			printf("Try to shrink the size of you character by %3f percent.\n", shrinkPercentage * 100);
		}
		else
		{
			printf("The size of your character is good.\n");
		}

		if (FR >= -1.0 / 30 && FR <= 1.0 / 30)
		{
			printf("The shape of your character is good.\n");
		}
		else if (FR < -1.0 / 30)
		{
			printf("Your character is too thin!\n");

			double widerPercentage = (spBBHeight * tpltBBWidth / tpltBBHeight) / (spBBHeight * tpltBBWidth / tpltBBHeight + spBBHeight * FR) - 1;
		
			printf("Try to make your character wider by %3f percent.\n", widerPercentage * 100);
		}
		else
		{
			printf("Your character is too wide!\n");

			double thinnerPercentage = (spBBHeight * tpltBBWidth / tpltBBHeight) / (spBBHeight * tpltBBWidth / tpltBBHeight + spBBHeight * FR) - 1;

			printf("Try to make your character thinner by %3f percent.\n", thinnerPercentage * 100);
		}

		if (Dx <= 0.25)
		{
			printf("You have made a good distribution of ink in the horizontal orientation.\n");
		}
		else
		{
			printf("You have made your character unbalance horizontally.\n");

			if (spXHalfRatio < tpltXHalfRatio)
			{
				printf("Ink is too heavily distributed on the left.\n");
				printf("Try to make it more balance by reducing stroke length, and try to reduce white space on the right side.\n");
			}
			else
			{
				printf("Ink is too heavily distributed on the right.\n");
				printf("Try to make it more balance by reducing stroke length, and try to reduce white space on the left side.\n");				
			}
		}

		if (Dy <= 0.25)
		{
			printf("You have made a good distribution of ink in the vertical orientation.\n");
		}
		else
		{
			printf("You have made your character unbalance vertically.\n");

			if (spYHalfRatio < tpltYHalfRatio)
			{
				printf("Your character is too heavy at the bottom.\n");
				printf("Try to distribute more ink on the upper half of your character.\n");
			}
			else
			{
				printf("Ink is too heavily distributed on the top.\n");
				printf("Try to leave less white space at the bottom to make your character look more stable.\n");
			}
		}
	}

	//Getter of FC and near/far scores
	double getFC()
	{
		printf("FC = %3f\n", FC);
		return FC;
	}

	double getMuonNearFC()
	{
		printf("muonNearFC = %3f\n", muonNearFC);
		return muonNearFC;
	}

	double getMuonFarFC()
	{
		printf("muonFarFC = %3f\n", muonFarFC);
		return muonFarFC;
	}

	//Getter of FA and large/proper/small scores
	double getFA()
	{
		printf("FA = %3f\n", FA);
		return FA;
	}

	double getMuonSmallFA()
	{
		printf("muonSmallFA = %3f\n", muonSmallFA);
		return muonSmallFA;
	}

	double getMuonProperFA()
	{
		printf("muonProperFA = %3f\n", muonProperFA);
		return muonProperFA;
	}

	double getMuonLargeFA()
	{
		printf("muonLargeFA = %3f\n", muonLargeFA);
		return muonLargeFA;
	}

	//Getter of FR and tall/proper/short scores
	double getFR()
	{
		printf("FR = %3f\n", FR);
		return FR;
	}

	double getMuonTallFR()
	{
		printf("muonTallFR = %3f\n", muonTallFR);
		return muonTallFR;
	}

	double getMuonProperFR()
	{
		printf("muonProperFR = %3f\n", muonProperFR);
		return muonProperFR;
	}

	double getMuonShortFR()
	{
		printf("muonShortFR = %3f\n", muonShortFR);
		return muonShortFR;
	}

	//Getter of Dx, Dy and less/much scores
	double getDx()
	{
		printf("Dx = %3f\n", Dx);
		return Dx;
	}

	double getDy()
	{
		printf("Dy = %3f\n", Dy);
		return Dy;
	}

	double getMuonLessDx()
	{
		printf("muonLessDx = %3f\n", muonLessDx);
		return muonLessDx;
	}

	double getMuonMuchDx()
	{
		printf("muonMuchDx = %3f\n", muonMuchDx);
		return muonMuchDx;
	}

	double getMuonLessDy()
	{
		printf("muonLessDy = %3f\n", muonLessDy);
		return muonLessDy;
	}

	double getMuonMuchDy()
	{
		printf("muonMuchDy = %3f\n", muonMuchDy);
		return muonMuchDy;
	}

	//Getter of FS and left/proper/right scores
	double getFS()
	{
		printf("FS = %3f\n", FS);
		return FS;
	}

	double getMuonLeftFS()
	{
		printf("muonLeftFS = %3f\n", muonLeftFS);
		return muonLeftFS;
	}

	double getMuonProperFS()
	{
		printf("muonProperFS = %3f\n", muonProperFS);
		return muonProperFS;
	}

	double getMuonRightFS()
	{
		printf("muonRightFS = %3f\n", muonRightFS);
		return muonRightFS;
	}

private:
	int spXC;
	int spYC;
	int tpltXC;
	int tpltYC;

	int spBBHeight;
	int spBBWidth;
	int tpltBBHeight;
	int tpltBBWidth;

	double spXHalfRatio;
	double spYHalfRatio;
	double tpltXHalfRatio;
	double tpltYHalfRatio;

	double FC;
	double FA;
	double FR;
	double Dx;
	double Dy;
	double FS;

	double muonNearFC;
	double muonFarFC;
	double muonSmallFA;
	double muonProperFA;
	double muonLargeFA;
	double muonTallFR;
	double muonProperFR;
	double muonShortFR;
	double muonLessDx;
	double muonMuchDx;
	double muonLessDy;
	double muonMuchDy;
	double muonLeftFS;
	double muonProperFS;
	double muonRightFS;

	double SC;
	double SA;
	double SR;
	double SDx;
	double SDy;
	double SS;

	double S1;
	double S2;
	double S3;
};