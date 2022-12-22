// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FPGBaseWidget.h"
#include "FPGBaseTileWidget.generated.h"

class UImage;
class UMaterialInstanceDynamic;

/**
 * Base tile for puzzle game.
 */
UCLASS()
class FIFTEENPUZZLEGAME_API UFPGBaseTileWidget : public UFPGBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Image;

public:
	UImage* GetImage() const { return Image; }
	UMaterialInstanceDynamic* GetImageDynamicMaterial() const;

	void SetImageBrushFromMaterial(UMaterialInstanceDynamic* MID);
};
