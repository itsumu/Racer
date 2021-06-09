// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameCompleteWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class RACER_API UGameCompleteWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//UPROPERTY()
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextTimeRecord;

	//UPROPERTY()
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonPlayAgain;

	UFUNCTION()
	void OnPlayAgainButtonPressed();
	
protected:
	virtual bool Initialize() override;
};
