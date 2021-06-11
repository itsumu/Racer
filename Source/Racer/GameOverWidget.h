// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UButton;

UCLASS()
class RACER_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonPlayAgain;

	UFUNCTION()
	void OnPlayAgainButtonPressed();

protected:
	virtual bool Initialize() override;
};
