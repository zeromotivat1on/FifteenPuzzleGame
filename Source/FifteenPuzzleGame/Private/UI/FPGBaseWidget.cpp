// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/FPGBaseWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"

FVector2D UFPGBaseWidget::GetWidgetViewportPosition(const UWidget* widget)
{
	if (!widget) return FVector2D::ZeroVector;

	FVector2D pixelPosition = FVector2D::ZeroVector;
	FVector2D viewportPosition = FVector2D::ZeroVector;

	USlateBlueprintLibrary::LocalToViewport(
		GetWorld(), widget->GetCachedGeometry(), FVector2D::ZeroVector, pixelPosition, viewportPosition);

	return viewportPosition;
}

FVector2D UFPGBaseWidget::GetViewportPosition()
{
	return GetWidgetViewportPosition(this);
}
