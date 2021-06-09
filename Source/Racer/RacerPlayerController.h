// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RacerPlayerController.generated.h"

class ACheckpoint;
class UGameCompleteWidget;
class UUserWidget;

UCLASS()
class RACER_API ARacerPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<ACheckpoint*> Checkpoints;

	UPROPERTY()
	UGameCompleteWidget* GameCompleteWidget;

public:
	TSubclassOf<class UUserWidget> GameCompleteWidgetBPClass;

	ARacerPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float Delta) override;
	void UpdateCheckpoint();
	void CompleteGame();
	void RestartGame();
private:
	FDateTime GameStartTime;
	int32 NextCheckpoint;
};
