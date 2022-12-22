// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FPGPuzzleGrid.generated.h"

/**
 * Struct to represent initial game puzzle square grid of tile numbers.
 */
USTRUCT()
struct FFPGPuzzleGrid
{
	GENERATED_BODY()

public:
	FFPGPuzzleGrid() = default;
	FFPGPuzzleGrid(int32 elemsAmount, int32 gridSize);

public:
	// Array of elements in this grid.
	// By default, 0 element represents empty puzzle slot.
	TArray<int32> Elements;

	// Amount of elements in the grid.
	int32 ElementsAmount = 0;

	// Size of grid, i.e. sqrt(ElementsAmount)
	int32 GridSize = 0;

public:
	// Calculate amount of inversions in puzzle grid.
	int32 CalculateInversions() const;

	// Get empty element position (not index!) from grid bottom.
	// element_position = element_index + 1.
	int32 FindEmptyElemPositionFromBottom() const;

	// Is this puzzle can be solved?
	bool IsSolvable() const;

	// Fill grid elements with loop index values - [0, ElementsAmount).
	void Indexize();

	// Shuffle random grid elements within random number of iterations.
	// Default min number of iterations - ElementsAmount.
	// Default max number of iterations - ElementsAmount * 2.
	// The number of iterations may be more than default max parameter due to ensurance of puzzle solvability.
	void Shuffle();

	// Shuffle random grid elements within random number of iterations.
	// @param minIterations - default min number of shuffle iterations.
	// @param maxIterations - default max number of shuffle iterations.
	// The number of iterations may be more than default max parameter due to ensurance of puzzle solvability.
	void Shuffle(int32 minIterations, int32 maxIterations);
};
