// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FPGBaseWidget.h"
#include "Puzzle/FPGPuzzleGrid.h"
#include "FPGPuzzleWidget.generated.h"

class UUniformGridPanel;
class UButton;
class UTexture;
class UFPGTileWidget;
class UFPGSlotWidget;
class UMaterialInterface;

enum class ESlotNeighbour : uint32
{
	SN_NONE = 0,
	SN_TOP = 1,
	SN_RIGHT = 2,
	SN_DOWN = 3,
	SN_LEFT = 4,
	SN_COUNT = 5,
};

DECLARE_MULTICAST_DELEGATE(FOnPlayerFirstMove);
DECLARE_MULTICAST_DELEGATE(FOnPuzzleSolved);
DECLARE_MULTICAST_DELEGATE(FOnPuzzleReset);
DECLARE_MULTICAST_DELEGATE(FOnPuzzleRestart);

/**
 * Base widget class for puzzle game.
 */
UCLASS()
class FIFTEENPUZZLEGAME_API UFPGPuzzleWidget : public UFPGBaseWidget
{
	GENERATED_BODY()

protected:
	// Grid widget to store tile slots that store tiles themself.
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* Grid;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ResetButton;

	// Texture to be split for each one tile texture.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puzzle")
	UTexture* TilesTexture;

	// Puzzle tiles to move in grid.
	UPROPERTY(BlueprintReadOnly, Category = "Puzzle")
	TArray<UFPGTileWidget*> Tiles;

	// Puzzle slots to store tiles in.
	UPROPERTY(BlueprintReadOnly, Category = "Puzzle")
	TArray<UFPGSlotWidget*> Slots;

	// Parent material for tile to create material instance from.
	// The material represents a square grid and allows to dynamically
	// create material instances from it with different textures.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puzzle|MI")
	UMaterialInterface* TileMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puzzle|MI")
	FName MIDTilesAmountName = "TilesAmount";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puzzle|MI")
	FName MIDTilesTextureName = "TilesTexture";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puzzle|MI")
	FName MIDTileCoordinateName = "TileCoordinate";

	// Class type for tile widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puzzle|Reflection")
	TSubclassOf<UUserWidget> TileWidgetClass;

	// Class type for tile slot widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puzzle|Reflection")
	TSubclassOf<UUserWidget> SlotWidgetClass;

	// Amount of tiles to split texture to.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puzzle")
	int32 TilesTextureAmount = 0;

	// Sqrt of TilesTextureAmount, i.e size of texture (its grid).
	// Note that the texture has square size dimensions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puzzle")
	int32 TilesTextureSize = 0;

private:
	// Inital shuffled puzzle grid.
	FFPGPuzzleGrid PuzzleGrid;

	// Amount of tiles correctly placed in their appropriate slots.
	int32 CorrectlyPlacedTilesAmount = 0;

protected:
	virtual void NativeOnInitialized() override;

public:
	// Initialize puzzle game.
	void InitializeGame();

	// Restart the game.
	UFUNCTION()
	void RestartGame();

	// Reset the game to state right after initialization.
	UFUNCTION()
	void ResetGame();

	UFPGSlotWidget* GetSlotNeighbour(const UFPGSlotWidget* slot, const ESlotNeighbour neighbourType) const;

protected:
	// Initialize default class fields.
	void InitPuzzleDefaults();

	// Initialize puzzle grid that represents order of tile numbers.
	void InitPuzzleGrid();

	// Initialize puzzle tiles by grid.
	void InitTiles();

	// Initialize puzzle slots with appropriate tile widgets.
	void InitSlots();

	// Completely remove tiles that are currently in slots.
	void RemoveTilesInSlots();

private:
	// Is this player first move?
	bool PlayerFirstMove = true;

private:
	// Event that is called when tile widget was successfully dropped from one slot to another.
	// @param currSlotWidget - slot widget on which tile widget was dropped.
	// @param prevSlotWidget - slot widget from which tile widget was dragged.
	void OnTileWidgetDroppedOnSlot(UFPGSlotWidget* currSlotWidget, UFPGSlotWidget* prevSlotWidget);

	// Update slots draggable state after player move.
	// Allows to drag only current empty slot neighbors (top, right, left, down).
	void UpdateSlotsDraggable();

public:
	// Called when player moved for the first time (drag and dropped tile from one slot to another).
	FOnPlayerFirstMove OnPlayerFirstMove;

	// Called when player solved the puzzle.
	FOnPuzzleSolved OnPuzzleSolved;

	// Called when player presses reset button.
	FOnPuzzleReset OnPuzzleReset;

	// Called when player presses restart button.
	FOnPuzzleRestart OnPuzzleRestart;
};
