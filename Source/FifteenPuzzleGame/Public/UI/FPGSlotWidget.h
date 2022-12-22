// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FPGBaseWidget.h"
#include "FPGSlotWidget.generated.h"

class USizeBox;
class UBorder;
class UWidgetSwitcher;
class UImage;
class UFPGTileWidget;
class UFPGTileDragWidget;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTileWidgetDropped, UFPGSlotWidget*, UFPGSlotWidget*);

/**
 * Slot widget to store puzzle tile.
 */
UCLASS()
class FIFTEENPUZZLEGAME_API UFPGSlotWidget : public UFPGBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox;

	UPROPERTY(meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	// Image for slot with placed tile on it.
	UPROPERTY(meta = (BindWidget))
	UImage* TileImage;

	// Default image for slot without placed tile.
	UPROPERTY(meta = (BindWidget))
	UImage* SlotImage;

	// Pointer to tile widget that is held by this slot.
	UPROPERTY(BlueprintReadOnly, Category = "Puzzle")
	UFPGTileWidget* TileWidget;

	// Class type for tile drag widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puzzle|Reflection")
	TSubclassOf<UUserWidget> TileDragWidgetClass;

	// Tile drag widget that is used to represent return to slot animation.
	// Created at mouse cursor position after drag cancelled event.
	UPROPERTY(BlueprintReadOnly, Category = "Puzzle")
	UFPGTileDragWidget* TileDragReturnWidget;

	// Number that this slot represents.
	UPROPERTY(BlueprintReadOnly, Category = "Puzzle")
	int32 Number = 0;

public:
	auto GetNumber() const { return Number; }
	auto GetTileWidget() const { return TileWidget; }

	void SetNumber(int32 number) { Number = number; }

	// Insert tile widget into this slot.
	void InsertTileWidget(UFPGTileWidget* tileWidget);

	// Remove tile widget from this slot.
	void RemoveTileWidget();

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnDragDetected(
		const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(
		const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	// Called when tile widget was inserted in this slot from drag and drop operation.
	FOnTileWidgetDropped OnTileWidgetDropped;

private:
	// Flag to check if return to slot animation should be applied.
	bool TileDragReturnAnimationStarted = false;

	// Current location of tile drag for return to slot animation.
	FVector2D TileDragReturnLocation = FVector2D::ZeroVector;

private:
	// Prepare to init tile drag return to slot animation.
	// @param prototype - widget on which new tile drag widget will be based.
	void SetupTileDragReturnAnimation(const UWidget* prototype);
};
