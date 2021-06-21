// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

// UMainMenuWidget::UMainMenuWidget()
// {

// }


bool UMainMenuWidget::Initialize()
{
	Super::Initialize();

	TextureDefault = Cast<UTexture2D>(
		StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/Materials/Teaser_Texture.Teaser_Texture")));

	TextureEasy = Cast<UTexture2D>(
		StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/Materials/Easy_Texture.Easy_Texture")));

	TextureMedium = Cast<UTexture2D>(
		StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/Materials/Medium_Texture.Medium_Texture")));

	TextureHard = Cast<UTexture2D>(
	StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/Materials/Hard_Texture.Hard_Texture")));
	
	ButtonEasy->OnHovered.AddDynamic(this, &UMainMenuWidget::OnEasyButtonHover);
	ButtonMedium->OnHovered.AddDynamic(this, &UMainMenuWidget::OnMediumButtonHover);
	ButtonHard->OnHovered.AddDynamic(this, &UMainMenuWidget::OnHardButtonHover);
	ButtonEasy->OnUnhovered.AddDynamic(this, &UMainMenuWidget::OnEasyButtonUnhover);
	ButtonMedium->OnUnhovered.AddDynamic(this, &UMainMenuWidget::OnMediumButtonUnhover);
	ButtonHard->OnUnhovered.AddDynamic(this, &UMainMenuWidget::OnHardButtonUnhover);
	
	ButtonEasy->OnClicked.AddDynamic(this, &UMainMenuWidget::OnEasyButtonPressed);
	ButtonMedium->OnClicked.AddDynamic(this, &UMainMenuWidget::OnMediumButtonPressed);
	ButtonHard->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHardButtonPressed);

	return true;
}

void UMainMenuWidget::OnEasyButtonHover()
{
	ImageBG->SetBrushFromTexture(TextureEasy);
}

void UMainMenuWidget::OnMediumButtonHover()
{
	ImageBG->SetBrushFromTexture(TextureMedium);
}

void UMainMenuWidget::OnHardButtonHover()
{
	ImageBG->SetBrushFromTexture(TextureHard);	
}

void UMainMenuWidget::OnEasyButtonUnhover()
{
	ImageBG->SetBrushFromTexture(TextureDefault);	
}

void UMainMenuWidget::OnMediumButtonUnhover()
{
	ImageBG->SetBrushFromTexture(TextureDefault);	
}

void UMainMenuWidget::OnHardButtonUnhover()
{
	ImageBG->SetBrushFromTexture(TextureDefault);	
}

void UMainMenuWidget::OnEasyButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("LoopTrack")));
}

void UMainMenuWidget::OnMediumButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("BoundedSpaceTrack")));
}

void UMainMenuWidget::OnHardButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("SpaceTrack")));
}