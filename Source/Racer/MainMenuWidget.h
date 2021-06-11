// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class RACER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonEasy;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonMedium;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonHard;

	UFUNCTION()
	void OnEasyButtonPressed();

	UFUNCTION()
	void OnMediumButtonPressed();

	UFUNCTION()
	void OnHardButtonPressed();

protected:
	virtual bool Initialize() override;
};
