// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameCompleteWidget.generated.h"

UCLASS()
class RACER_API UGameCompleteWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextTimeRecord;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonPlayAgain;

	UFUNCTION()
	void OnPlayAgainButtonPressed();
	
protected:
	virtual bool Initialize() override;
};
