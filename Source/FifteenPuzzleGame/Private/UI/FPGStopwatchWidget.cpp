// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/FPGStopwatchWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Puzzle/FPGStopwatch.h"

void UFPGStopwatchWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Stopwatch = NewObject<UFPGStopwatch>(GetWorld());
}

FText UFPGStopwatchWidget::GetStopwatchTimeText() const
{
	int32 hours = Stopwatch->GetHours();
	int32 minutes = Stopwatch->GetMinutes();
	int32 seconds = Stopwatch->GetSeconds();

	auto hoursText = UKismetTextLibrary::Conv_IntToText(hours, false, true, 2);
	auto minutesText = UKismetTextLibrary::Conv_IntToText(minutes, false, true, 2);
	auto secondsText = UKismetTextLibrary::Conv_IntToText(seconds, false, true, 2);

	return FText::Format(FTextFormat::FromString("{0} : {1} : {2}"), hoursText, minutesText, secondsText);
}
