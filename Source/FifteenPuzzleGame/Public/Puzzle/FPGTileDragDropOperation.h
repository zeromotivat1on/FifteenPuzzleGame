// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "FPGTileDragDropOperation.generated.h"

class UFPGSlotWidget;

/**
 * Custom drag and drop operation that stores additional data for puzzle game.
 */
UCLASS()
class FIFTEENPUZZLEGAME_API UFPGTileDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	// Slot widget from which drag and drop operation was initiated.
	UPROPERTY()
	UFPGSlotWidget* SlotWidget;
};
