// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "HUDWidget.generated.h"

UCLASS()
class RACER_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextTime;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextVelocity;
};
