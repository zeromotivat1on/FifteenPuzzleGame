// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzle/FPGPuzzleGrid.h"

FFPGPuzzleGrid::FFPGPuzzleGrid(int32 elemsAmount, int32 gridSize) //
	: ElementsAmount(elemsAmount), GridSize(gridSize)
{
	Elements.Reserve(ElementsAmount);
	Elements.AddDefaulted(ElementsAmount);
}

int32 FFPGPuzzleGrid::CalculateInversions() const
{
	int32 inversionsCount = 0;

	for (int32 i = 0; i < ElementsAmount - 1; ++i)
	{
		for (int32 j = i + 1; j < ElementsAmount; ++j)
		{
			if (Elements[i] && Elements[j] && Elements[i] > Elements[j])
			{
				inversionsCount++;
			}
		}
	}

	return inversionsCount;
}

int32 FFPGPuzzleGrid::FindEmptyElemPositionFromBottom() const
{
	int32 row = 0;
	for (int32 i = 0; i < ElementsAmount; ++i)
	{
		if (i % GridSize == 0) row++;
		if (Elements[i] == 0) return GridSize - row + 1;
	}

	return -1;
}

bool FFPGPuzzleGrid::IsSolvable() const
{
	int32 inversionsCount = CalculateInversions();

	bool invCntEven = !(inversionsCount & 1);
	bool gridSizeEven = !(GridSize & 1);

	if (gridSizeEven)
	{
		int32 position = FindEmptyElemPositionFromBottom();
		bool posEven = !(position & 1);

		if (posEven)
		{
			return !invCntEven;
		}
		else
		{
			return invCntEven;
		}
	}
	else
	{
		return invCntEven;
	}

	return false;
}

void FFPGPuzzleGrid::Indexize()
{
	for (int i = 0; i < ElementsAmount; ++i)
	{
		Elements[i] = i;
	}
}

void FFPGPuzzleGrid::Shuffle()
{
	Shuffle(ElementsAmount, ElementsAmount * 2);
}

void FFPGPuzzleGrid::Shuffle(int32 minIterations, int32 maxIterations)
{
	for (int i = 0; i < FMath::RandRange(minIterations, maxIterations); ++i)
	{
		Elements.Swap(FMath::RandRange(0, ElementsAmount - 1), FMath::RandRange(0, ElementsAmount - 1));
	}

	while (!IsSolvable())
	{
		Elements.Swap(FMath::RandRange(0, ElementsAmount - 1), FMath::RandRange(0, ElementsAmount - 1));
	}
}
