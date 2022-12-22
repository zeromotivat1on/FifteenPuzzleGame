// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/FPGBaseTileWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

UMaterialInstanceDynamic* UFPGBaseTileWidget::GetImageDynamicMaterial() const
{
	if (!Image) return nullptr;
	return Image->GetDynamicMaterial();
}

void UFPGBaseTileWidget::SetImageBrushFromMaterial(UMaterialInstanceDynamic* MID)
{
	if (!Image) return;
	Image->SetBrushFromMaterial(MID);
}