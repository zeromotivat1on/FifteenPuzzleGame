// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/FPGPuzzleWidget.h"
#include "Engine/Texture.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Materials/MaterialInterface.h"
#include "UI/FPGTileWidget.h"
#include "UI/FPGSlotWidget.h"

void UFPGPuzzleWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InitializeGame();

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UFPGPuzzleWidget::RestartGame);
	}

	if (ResetButton)
	{
		ResetButton->OnClicked.AddDynamic(this, &UFPGPuzzleWidget::ResetGame);
	}
}

void UFPGPuzzleWidget::InitializeGame()
{
	if (TilesTextureAmount <= 0 || TilesTextureSize <= 0) return;

	InitPuzzleDefaults();
	InitPuzzleGrid();
	InitTiles();
	InitSlots();

	UpdateSlotsDraggable();
}

void UFPGPuzzleWidget::RestartGame()
{
	Tiles.Empty(TilesTextureAmount);

	RemoveTilesInSlots();

	InitializeGame();

	OnPuzzleRestart.Broadcast();
}

void UFPGPuzzleWidget::ResetGame()
{
	bool needsReset = false;
	for (int32 i = 0; i < TilesTextureAmount; ++i)
	{
		auto tile = Slots[i]->GetTileWidget();
		if (tile && tile->GetNumber() != PuzzleGrid.Elements[i])
		{
			needsReset = true;
			break;
		}
	}

	if (!needsReset) return;

	InitPuzzleDefaults();
	RemoveTilesInSlots();
	InitSlots();

	UpdateSlotsDraggable();

	OnPuzzleReset.Broadcast();
}

UFPGSlotWidget* UFPGPuzzleWidget::GetSlotNeighbour(const UFPGSlotWidget* slot, const ESlotNeighbour neighbourType) const
{
	if (!slot) return nullptr;

	int32 slotIndex = slot->GetNumber() - 1;
	int32 neighbourIndex = -1;

	switch (neighbourType)
	{
	case ESlotNeighbour::SN_TOP:
	{
		neighbourIndex = slotIndex - TilesTextureSize;
		break;
	}

	case ESlotNeighbour::SN_RIGHT:
	{
		neighbourIndex = slotIndex + 1;
		if (neighbourIndex % TilesTextureSize < slotIndex % TilesTextureSize)
		{
			neighbourIndex = -1;
		}

		break;
	}

	case ESlotNeighbour::SN_DOWN:
	{
		neighbourIndex = slotIndex + TilesTextureSize;
		break;
	}

	case ESlotNeighbour::SN_LEFT:
	{
		neighbourIndex = slotIndex - 1;
		if (neighbourIndex % TilesTextureSize > slotIndex % TilesTextureSize)
		{
			neighbourIndex = -1;
		}

		break;
	}

	default:
		break;
	}

	if (neighbourIndex >= 0 && neighbourIndex < TilesTextureAmount)
	{
		return Slots[neighbourIndex];
	}

	return nullptr;
}

void UFPGPuzzleWidget::InitPuzzleDefaults()
{
	CorrectlyPlacedTilesAmount = 0;
	PlayerFirstMove = true;
}

void UFPGPuzzleWidget::InitPuzzleGrid()
{
	PuzzleGrid = FFPGPuzzleGrid(TilesTextureAmount, TilesTextureSize);
	PuzzleGrid.Indexize();
	PuzzleGrid.Shuffle();
}

void UFPGPuzzleWidget::InitTiles()
{
	Tiles.Reserve(TilesTextureAmount);

	const float reverseTilesTextureSize = UKismetMathLibrary::SafeDivide(1, TilesTextureSize);

	for (int32 number : PuzzleGrid.Elements)
	{
		if (number == 0) number = TilesTextureAmount;

		int32 i = number - 1;

		auto tile = CreateWidget<UFPGTileWidget>(this, TileWidgetClass);
		if (!tile) continue;

		const auto MID = UMaterialInstanceDynamic::Create(TileMaterial, this);
		if (!MID) continue;

		int32 col = i % TilesTextureSize;
		int32 row = i * reverseTilesTextureSize;

		MID->SetScalarParameterValue(MIDTilesAmountName, TilesTextureAmount);
		MID->SetTextureParameterValue(MIDTilesTextureName, TilesTexture);
		MID->SetVectorParameterValue(MIDTileCoordinateName, FVector4(col, row, 0.0f, 1.0f));

		tile->SetNumber(number);
		tile->SetImageBrushFromMaterial(MID);

		Tiles.Add(tile);
	}
}

void UFPGPuzzleWidget::InitSlots()
{
	const float reverseTilesTextureSize = UKismetMathLibrary::SafeDivide(1, TilesTextureSize);

	// Have we already created slots or should we create new ones?
	bool shouldCreateNew = Slots.Num() != TilesTextureAmount;

	if (shouldCreateNew)
	{
		Slots.Reserve(TilesTextureAmount);
	}

	for (int32 i = 0; i < TilesTextureAmount; ++i)
	{
		auto slot = shouldCreateNew ? CreateWidget<UFPGSlotWidget>(this, SlotWidgetClass) : Slots[i];
		if (!slot) continue;

		if (shouldCreateNew)
		{
			slot->SetNumber(i + 1);
			slot->OnTileWidgetDropped.AddUObject(this, &UFPGPuzzleWidget::OnTileWidgetDroppedOnSlot);

			int32 row = i * reverseTilesTextureSize;
			int32 col = i % TilesTextureSize;
			Grid->AddChildToUniformGrid(slot, row, col);

			Slots.Add(slot);
		}

		// We need 1 more additional free slot in order to move tiles.
		if (Tiles[i]->GetNumber() != TilesTextureAmount)
		{
			slot->InsertTileWidget(Tiles[i]);

			bool correctlyPlaced = slot->GetNumber() == slot->GetTileWidget()->GetNumber();
			if (correctlyPlaced) CorrectlyPlacedTilesAmount++;
		}
	}
}

void UFPGPuzzleWidget::RemoveTilesInSlots()
{
	for (auto& slot : Slots)
	{
		if (auto tile = slot->GetTileWidget()) tile->RemoveFromParent();

		slot->RemoveTileWidget();
	}
}

void UFPGPuzzleWidget::OnTileWidgetDroppedOnSlot(UFPGSlotWidget* currSlotWidget, UFPGSlotWidget* prevSlotWidget)
{
	if (!currSlotWidget || !prevSlotWidget) return;

	auto tileWidget = currSlotWidget->GetTileWidget();
	if (!tileWidget || prevSlotWidget->GetTileWidget()) return;

	// Tile widget is considered to be correctly placed
	// if its representative number equals to slot representative number.

	bool correctlyPlaced = currSlotWidget->GetNumber() == tileWidget->GetNumber();
	bool wasCorrectlyPlaced = prevSlotWidget->GetNumber() == tileWidget->GetNumber();

	if (correctlyPlaced)
	{
		CorrectlyPlacedTilesAmount++;
	}
	else
	{
		if (wasCorrectlyPlaced && CorrectlyPlacedTilesAmount > 0)
		{
			CorrectlyPlacedTilesAmount--;
		}
	}

	// Check if puzzle solved.

	if (CorrectlyPlacedTilesAmount >= TilesTextureAmount - 1)
	{
		for (auto& tile : Tiles)
		{
			if (tile->GetNumber() == TilesTextureAmount)
			{
				Slots.Last()->InsertTileWidget(tile);
				break;
			}
		}

		OnPuzzleSolved.Broadcast();
	}

	UpdateSlotsDraggable();

	if (PlayerFirstMove)
	{
		OnPlayerFirstMove.Broadcast();
		PlayerFirstMove = false;
	}
}

void UFPGPuzzleWidget::UpdateSlotsDraggable()
{
	TArray<int32> ignoredSlotNumbers;
	ignoredSlotNumbers.Reserve(2);

	for (const auto& slot : Slots)
	{
		if (!slot->GetTileWidget())
		{
			auto topNeighbour = GetSlotNeighbour(slot, ESlotNeighbour::SN_TOP);
			if (topNeighbour && topNeighbour->GetTileWidget())
			{
				topNeighbour->GetTileWidget()->SetDraggable(true);
			}

			auto rightNeighbour = GetSlotNeighbour(slot, ESlotNeighbour::SN_RIGHT);
			if (rightNeighbour && rightNeighbour->GetTileWidget())
			{
				rightNeighbour->GetTileWidget()->SetDraggable(true);
				ignoredSlotNumbers.Add(rightNeighbour->GetNumber());
			}

			auto downNeighbour = GetSlotNeighbour(slot, ESlotNeighbour::SN_DOWN);
			if (downNeighbour && downNeighbour->GetTileWidget())
			{
				downNeighbour->GetTileWidget()->SetDraggable(true);
				ignoredSlotNumbers.Add(downNeighbour->GetNumber());
			}

			auto leftNeighbour = GetSlotNeighbour(slot, ESlotNeighbour::SN_LEFT);
			if (leftNeighbour && leftNeighbour->GetTileWidget())
			{
				leftNeighbour->GetTileWidget()->SetDraggable(true);
			}

			continue;
		}

		if (!ignoredSlotNumbers.Contains(slot->GetNumber()))
		{
			slot->GetTileWidget()->SetDraggable(false);
		}
	}
}
