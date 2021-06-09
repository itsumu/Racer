// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCompleteWidget.h"

#include "Kismet/GameplayStatics.h"

#include "RacerPlayerController.h"

bool UGameCompleteWidget::Initialize()
{
	Super::Initialize();

	ButtonPlayAgain->OnClicked.AddDynamic(this, &UGameCompleteWidget::OnPlayAgainButtonPressed);
	return true;
}

void UGameCompleteWidget::OnPlayAgainButtonPressed()
{
	ARacerPlayerController* PlayerController = Cast<ARacerPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->RestartGame();
}