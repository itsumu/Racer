// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

class UUserWidget;
class UMainMenuWidget;

UCLASS()
class RACER_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;

public:
	TSubclassOf<class UUserWidget> MainMenuWidgetBPClass;

	AMenuPlayerController();

protected:
	virtual void BeginPlay() override;
};
