// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPGGameModeBase.h"
#include "Player/FPGPlayerController.h"
#include "UI/FPGBaseHUD.h"

AFPGGameModeBase::AFPGGameModeBase()
{
	HUDClass = AFPGBaseHUD::StaticClass();
	PlayerControllerClass = AFPGPlayerController::StaticClass();
}
