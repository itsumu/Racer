// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameModeBase.h"
#include "RacerPlayerController.h"

ARacerPlayerController::ARacerPlayerController()
{
}

void ARacerPlayerController::BeginPlay()
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	Possess(GameMode->DefaultPawnClass.GetDefaultObject());
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, GetPawn()->GetName());
}

void ARacerPlayerController::Tick(float Delta)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, GetPawn()->GetName());
}