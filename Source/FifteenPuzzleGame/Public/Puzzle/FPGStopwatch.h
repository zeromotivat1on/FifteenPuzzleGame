// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FPGStopwatch.generated.h"

UCLASS()
class FIFTEENPUZZLEGAME_API UFPGStopwatch : public UObject
{
	GENERATED_BODY()

protected:
	// Time interval in seconds between stopwatch ticks.
	float TickInterval = 1.0f;

	int32 Hours = 0;
	int32 Minutes = 0;
	int32 Seconds = 0;

	// Values to add to stopwatch time on each stopwatch tick.

	int32 TickIncrementHours = 1;
	int32 TickIncrementMinutes = 1;
	int32 TickIncrementSeconds = 1;

	// Border values for stopwatch time.

	int32 SecondsInMinute = 60;
	int32 MinutesInHour = 60;

public:
	auto GetHours() const { return Hours; }
	auto GetMinutes() const { return Minutes; }
	auto GetSeconds() const { return Seconds; }

	UFUNCTION()
	void Start();

	UFUNCTION()
	void Stop();

	UFUNCTION()
	void Reset();

	UFUNCTION()
	void Restart();

private:
	FTimerHandle StopwatchTimerHandle;

private:
	// Stopwatch time increment.
	void StopwatchTick();
};
