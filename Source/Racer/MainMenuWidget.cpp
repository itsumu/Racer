// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"

#include "RacerPlayerController.h"

bool UMainMenuWidget::Initialize()
{
	Super::Initialize();

	ButtonEasy->OnClicked.AddDynamic(this, &UMainMenuWidget::OnEasyButtonPressed);
	ButtonMedium->OnClicked.AddDynamic(this, &UMainMenuWidget::OnMediumButtonPressed);
	ButtonHard->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHardButtonPressed);
	return true;
}

void UMainMenuWidget::OnEasyButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("LoopTrack")));
}

void UMainMenuWidget::OnMediumButtonPressed()
{
	//UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("SpaceTrack")));
}

void UMainMenuWidget::OnHardButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("SpaceTrack")));
}