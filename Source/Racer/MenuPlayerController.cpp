// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPlayerController.h"

#include "Kismet/GameplayStatics.h"

#include "MainMenuWidget.h"

AMenuPlayerController::AMenuPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetBP(TEXT("/Game/UI/MainMenu"));
	check(MenuWidgetBP.Succeeded());
	MainMenuWidgetBPClass = MenuWidgetBP.Class;
}

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true; 
	SetInputMode(FInputModeUIOnly());
	MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetBPClass);
	MainMenuWidget->AddToViewport();
}