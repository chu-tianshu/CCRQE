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
		int spXC = sp.getXOfGravityCenter();
		int spYC = sp.getYOfGravityCenter();
		int tpltXC = tplt.getXOfGravityCenter();
		int tpltYC = tplt.getYOfGravityCenter();

		FC = sqrt(pow((spXC - tpltXC), 2) + pow((spYC - tpltYC), 2)) / (0.5 * width);

		muonNearFC = GAMMA(FC, 0.1, 0.9);
		muonFarFC = LAMBDA(FC, 0.1, 0.9);

		//Set up FA and large/proper/small scores
		int spBBWidth = sp.getWidthOfBB();
		int spBBHeight = sp.getHeightOfBB();
		int tpltBBWidth = tplt.getWidthOfBB();
		int tpltBBHeight = tplt.getHeightOfBB();

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
};