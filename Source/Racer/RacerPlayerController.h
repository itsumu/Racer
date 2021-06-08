// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RacerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RACER_API ARacerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARacerPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float Delta) override;

private:
	int64 GameStartTime;
};
