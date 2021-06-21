// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class RACER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonEasy;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonMedium;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonHard;

	UPROPERTY(meta = (BindWidget))
	UImage* ImageBG;

	UPROPERTY(EditAnywhere)
	UTexture2D* TextureDefault;
	
	UPROPERTY(EditAnywhere)
	UTexture2D* TextureEasy;

	UPROPERTY(EditAnywhere)
	UTexture2D* TextureMedium;

	UPROPERTY(EditAnywhere)
	UTexture2D* TextureHard;
	
	UFUNCTION()
	void OnEasyButtonHover();

	UFUNCTION()
	void OnMediumButtonHover();

	UFUNCTION()
	void OnHardButtonHover();	

	UFUNCTION()
	void OnEasyButtonUnhover();

	UFUNCTION()
	void OnMediumButtonUnhover();

	UFUNCTION()
	void OnHardButtonUnhover();	

	UFUNCTION()
	void OnEasyButtonPressed();

	UFUNCTION()
	void OnMediumButtonPressed();

	UFUNCTION()
	void OnHardButtonPressed();

protected:
	// UMainMenuWidget();
	virtual bool Initialize() override;
};
