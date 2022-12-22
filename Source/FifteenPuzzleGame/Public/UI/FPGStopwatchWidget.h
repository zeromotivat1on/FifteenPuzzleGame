// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FPGBaseWidget.h"
#include "FPGStopwatchWidget.generated.h"

class UTextBlock;
class UFPGStopwatch;

/**
 * Widget for stopwatch.
 */
UCLASS()
class FIFTEENPUZZLEGAME_API UFPGStopwatchWidget : public UFPGBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextTime;

	UPROPERTY(BlueprintReadOnly, Category = "Time")
	UFPGStopwatch* Stopwatch;

protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION(BlueprintCallable)
	FText GetStopwatchTimeText() const;

	UFPGStopwatch* GetStopwatch() const { return Stopwatch; }
};
