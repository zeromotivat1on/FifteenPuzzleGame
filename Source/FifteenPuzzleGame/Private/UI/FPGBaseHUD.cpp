// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/FPGBaseHUD.h"
#include "UI/FPGPuzzleWidget.h"
#include "UI/FPGStopwatchWidget.h"
#include "Puzzle/FPGStopwatch.h"

void AFPGBaseHUD::BeginPlay()
{
	Super::BeginPlay();

	InitPuzzleWidgets();
}

void AFPGBaseHUD::InitPuzzleWidgets()
{
	PuzzleWidget = CreateWidget<UFPGPuzzleWidget>(GetWorld(), PuzzleWidgetClass);

	if (PuzzleWidget)
	{
		PuzzleWidget->AddToViewport();
	}

	StopwatchWidget = CreateWidget<UFPGStopwatchWidget>(GetWorld(), StopwatchWidgetClass);

	if (StopwatchWidget)
	{
		StopwatchWidget->AddToViewport();

		if (const auto stopwatch = StopwatchWidget->GetStopwatch())
		{
			PuzzleWidget->OnPlayerFirstMove.AddUObject(stopwatch, &UFPGStopwatch::Start);
			PuzzleWidget->OnPuzzleSolved.AddUObject(stopwatch, &UFPGStopwatch::Stop);
			PuzzleWidget->OnPuzzleReset.AddUObject(stopwatch, &UFPGStopwatch::Reset);
			PuzzleWidget->OnPuzzleRestart.AddUObject(stopwatch, &UFPGStopwatch::Reset);
		}
	}
}
