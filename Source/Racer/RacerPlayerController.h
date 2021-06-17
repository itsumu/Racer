// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RacerPlayerController.generated.h"

class ACheckpoint;
class UGameCompleteWidget;
class UGameOverWidget;
class UUserWidget;

UCLASS()
class RACER_API ARacerPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<ACheckpoint*> Checkpoints;

	UPROPERTY()
	UGameCompleteWidget* GameCompleteWidget;

	UPROPERTY()
	UGameOverWidget* GameOverWidget;

	UPROPERTY()
	UUserWidget* MinimapWidget;

public:
	TSubclassOf<class UUserWidget> GameCompleteWidgetBPClass;
	TSubclassOf<class UUserWidget> GameOverWidgetBPClass;
	TSubclassOf<class UUserWidget> MinimapWidgetBPClass;
	
	ARacerPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float Delta) override;
	virtual void SetupInputComponent() override;
	void UpdateCheckpoint();
	void CompleteGame();
	void TerminateGame();
	void RestartGame();
	void Revive();

private:
	FDateTime GameStartTime;
	int32 NextCheckpoint;
	bool bVehicleIsRunning;
};
