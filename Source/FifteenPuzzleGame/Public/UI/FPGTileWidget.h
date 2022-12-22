// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FPGBaseTileWidget.h"
#include "FPGTileWidget.generated.h"

class UImage;

/**
 * Tile widget that can be dragged and dropped.
 * Represents puzzle tile with number (by default puzzle rules).
 */
UCLASS()
class FIFTEENPUZZLEGAME_API UFPGTileWidget : public UFPGBaseTileWidget
{
	GENERATED_BODY()

protected:
	// Number that this tile represents.
	UPROPERTY(BlueprintReadOnly, Category = "Puzzle")
	int32 Number = 0;

	// Is this tile can be dragged and dropped?
	UPROPERTY(BlueprintReadOnly, Category = "Puzzle")
	bool Draggable = false;

public:
	auto GetNumber() const { return Number; }
	auto IsDraggable() const { return Draggable; }

	void SetNumber(int32 number) { Number = number; }
	void SetDraggable(bool draggable) { Draggable = draggable; }
};
