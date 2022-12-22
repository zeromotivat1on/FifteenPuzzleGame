// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPGBaseHUD.generated.h"

class UFPGPuzzleWidget;
class UFPGStopwatchWidget;

UCLASS()
class FIFTEENPUZZLEGAME_API AFPGBaseHUD : public AHUD
{
	GENERATED_BODY()

protected:
	// Class type for puzzle widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reflection")
	TSubclassOf<UUserWidget> PuzzleWidgetClass;

	// Class type for stopwatch widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reflection")
	TSubclassOf<UUserWidget> StopwatchWidgetClass;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UFPGPuzzleWidget* PuzzleWidget;

	UPROPERTY()
	UFPGStopwatchWidget* StopwatchWidget;

private:
	void InitPuzzleWidgets();
};
