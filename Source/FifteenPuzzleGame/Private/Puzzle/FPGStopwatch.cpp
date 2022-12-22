// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzle/FPGStopwatch.h"

void UFPGStopwatch::Start()
{
	if (!GetWorld() || StopwatchTimerHandle.IsValid()) return;
	GetWorld()->GetTimerManager().SetTimer(
		StopwatchTimerHandle, this, &UFPGStopwatch::StopwatchTick, TickInterval, true);
}

void UFPGStopwatch::Stop()
{
	if (!GetWorld() || !StopwatchTimerHandle.IsValid()) return;
	GetWorld()->GetTimerManager().ClearTimer(StopwatchTimerHandle);
}

void UFPGStopwatch::Reset()
{
	Hours = Minutes = Seconds = 0;
	Stop();
}

void UFPGStopwatch::Restart()
{
	Reset();
	Start();
}

void UFPGStopwatch::StopwatchTick()
{
	Seconds += TickIncrementSeconds;
	if (Seconds >= SecondsInMinute)
	{
		Seconds = 0;
		Minutes += TickIncrementMinutes;

		if (Minutes >= MinutesInHour)
		{
			Minutes = 0;
			Hours += TickIncrementHours;
		}
	}
}
