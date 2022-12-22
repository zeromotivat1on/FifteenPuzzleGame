// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/FPGSlotWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "Components/WidgetSwitcher.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/FPGTileWidget.h"
#include "UI/FPGTileDragWidget.h"
#include "Puzzle/FPGTileDragDropOperation.h"

void UFPGSlotWidget::NativeOnInitialized()
{
	WidgetSwitcher->SetActiveWidget(SlotImage);
}

void UFPGSlotWidget::InsertTileWidget(UFPGTileWidget* tileWidget)
{
	if (!tileWidget || !tileWidget->GetImage()) return;

	TileWidget = tileWidget;

	TileImage->SetBrushFromMaterial(TileWidget->GetImageDynamicMaterial());

	WidgetSwitcher->SetActiveWidget(TileImage);
}

void UFPGSlotWidget::RemoveTileWidget()
{
	if (!TileWidget) return;

	WidgetSwitcher->SetActiveWidget(SlotImage);

	TileWidget = nullptr;
}

void UFPGSlotWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (TileDragReturnAnimationStarted && TileDragReturnWidget)
	{
		FVector2D viewportPosition = GetViewportPosition();
		TileDragReturnLocation = FMath::Vector2DInterpTo(TileDragReturnLocation, viewportPosition, DeltaTime, 16.0f);

		TileDragReturnWidget->SetRenderTranslation(TileDragReturnLocation);

		if (UKismetMathLibrary::IsNearlyZero2D(FMath::Abs(TileDragReturnLocation - viewportPosition), 1.0f))
		{
			WidgetSwitcher->SetActiveWidget(TileImage);

			TileDragReturnWidget->RemoveFromParent();
			TileDragReturnWidget = nullptr;

			TileDragReturnAnimationStarted = false;
		}
	}
}

FReply UFPGSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UFPGSlotWidget::NativeOnDragDetected(
	const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (!TileWidget || !TileWidget->IsDraggable()) return;

	auto tileDrag = CreateWidget<UFPGTileDragWidget>(this, TileDragWidgetClass);
	if (!tileDrag) return;

	tileDrag->SetImageBrushFromMaterial(TileImage->GetDynamicMaterial());

	auto DragDropOperation = NewObject<UFPGTileDragDropOperation>();
	DragDropOperation->DefaultDragVisual = tileDrag;
	DragDropOperation->SlotWidget = this;

	OutOperation = DragDropOperation;

	WidgetSwitcher->SetActiveWidget(SlotImage);

	UUserWidget::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

void UFPGSlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	SetupTileDragReturnAnimation(InOperation->DefaultDragVisual);

	UUserWidget::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

bool UFPGSlotWidget::NativeOnDrop(
	const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!TileImage || TileWidget) return false;

	auto FPGOutOperation = Cast<UFPGTileDragDropOperation>(InOperation);
	if (!FPGOutOperation) return false;

	auto& opSlotWidget = FPGOutOperation->SlotWidget;
	auto& opTileWidget = FPGOutOperation->SlotWidget->TileWidget;

	if (!opSlotWidget || !opTileWidget) return false;

	TileWidget = opTileWidget;
	opTileWidget = nullptr;

	TileImage->SetBrushFromMaterial(TileWidget->GetImageDynamicMaterial());

	SetupTileDragReturnAnimation(InOperation->DefaultDragVisual);

	OnTileWidgetDropped.Broadcast(this, opSlotWidget);

	UUserWidget::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return true;
}

void UFPGSlotWidget::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& InMouseEvent)
{
	if (!TileWidget) return;

	FColor hoverColor = TileWidget->IsDraggable() ? FColor(80, 200, 70) : FColor(200, 70, 70);
	Border->SetBrushColor(hoverColor);

	Super::NativeOnMouseEnter(MyGeometry, InMouseEvent);
}

void UFPGSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Border->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));

	Super::NativeOnMouseLeave(InMouseEvent);
}

void UFPGSlotWidget::SetupTileDragReturnAnimation(const UWidget* prototype)
{
	auto tileDrag = Cast<UFPGTileDragWidget>(prototype);

	if (!tileDrag || !tileDrag->GetImage()) return;

	TileDragReturnWidget = CreateWidget<UFPGTileDragWidget>(this, TileDragWidgetClass);
	if (!TileDragReturnWidget) return;

	FVector2D dragTileSize = tileDrag->GetDesiredSize();
	FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	// By default, animation tile drag initial location is such that
	// mouse cursor is in center of the new animation drag tile.
	FVector2D tileDragInitLocation = FVector2D(mousePos - dragTileSize * 0.5f);

	TileDragReturnLocation = tileDragInitLocation;

	TileDragReturnWidget->AddToViewport();
	TileDragReturnWidget->SetDesiredSizeInViewport(tileDrag->GetDesiredSize());
	TileDragReturnWidget->SetImageBrushFromMaterial(tileDrag->GetImageDynamicMaterial());
	TileDragReturnWidget->SetRenderTranslation(tileDragInitLocation);

	TileDragReturnAnimationStarted = true;
}
