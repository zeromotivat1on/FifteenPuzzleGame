// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/FPGPlayerController.h"

void AFPGPlayerController::BeginPlay()
{
	bShowMouseCursor = true;

	SetInputMode(FInputModeUIOnly());
}
