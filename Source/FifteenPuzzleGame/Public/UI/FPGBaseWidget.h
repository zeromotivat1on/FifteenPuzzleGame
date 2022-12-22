// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPGBaseWidget.generated.h"

/**
 * Base widget for general purpose with additional functionality.
 */
UCLASS()
class FIFTEENPUZZLEGAME_API UFPGBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Get viewport position of a given widget.
	FVector2D GetWidgetViewportPosition(const UWidget* widget);

	// Get self viewport position.
	FVector2D GetViewportPosition();
};
