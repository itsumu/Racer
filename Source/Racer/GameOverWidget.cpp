// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "RacerPlayerController.h"

bool UGameOverWidget::Initialize()
{
	Super::Initialize();

	ButtonPlayAgain->OnClicked.AddDynamic(this, &UGameOverWidget::OnPlayAgainButtonPressed);
	return true;
}

void UGameOverWidget::OnPlayAgainButtonPressed()
{
	ARacerPlayerController* PlayerController = Cast<ARacerPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->RestartGame();
}