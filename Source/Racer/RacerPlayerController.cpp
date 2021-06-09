// Fill out your copyright notice in the Description page of Project Settings.


#include "RacerPlayerController.h"

#include "GameFramework/GameModeBase.h"
#include "Misc/DateTime.h"

#include "Checkpoint.h"
#include "GameCompleteWidget.h"
#include "SportVehicle.h"

ARacerPlayerController::ARacerPlayerController()
{
	bAutoManageActiveCameraTarget = true;

	// Generate UI from assets
	static ConstructorHelpers::FClassFinder<UUserWidget> GameWidgetBP(TEXT("/Game/UI/GameComplete"));
	check(GameWidgetBP.Succeeded());
	if (GameWidgetBP.Class != nullptr)
	{
		GameCompleteWidgetBPClass = GameWidgetBP.Class;
	}
}

void ARacerPlayerController::BeginPlay()
{
	// Timing
	GameStartTime = FDateTime::Now();

	// Checkpoints generation
	//FVector CheckpointStartLocation(-9150.0f, -4490.0f, 260.0f);
	//FVector CheckpointMiddleLocation(-8150.0f, -4490.0f, 260.0f);
	//FVector CheckpointEndLocation(-7150.0f, -4490.0f, 260.0f);

	FVector CheckpointStartLocation(-9660.0f, -4450.0f, 50.0f);
	FVector CheckpointMiddleLocation(-4630.0f, -1740.0f, 50.0f);
	FVector CheckpointEndLocation(-9900, -4450, 50.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);

	ACheckpoint* CheckpointStart = GetWorld()->SpawnActor<ACheckpoint>(ACheckpoint::StaticClass(), CheckpointStartLocation, Rotation);
	ACheckpoint* CheckpointMiddle = GetWorld()->SpawnActor<ACheckpoint>(ACheckpoint::StaticClass(), CheckpointMiddleLocation, Rotation);
	ACheckpoint* CheckpointEnd = GetWorld()->SpawnActor<ACheckpoint>(ACheckpoint::StaticClass(), CheckpointEndLocation, Rotation);

	Checkpoints.Add(CheckpointStart);
	Checkpoints.Add(CheckpointMiddle);
	Checkpoints.Add(CheckpointEnd);

	NextCheckpoint = 0;
	Checkpoints[NextCheckpoint]->Enable();

	// UI widgets
	GameCompleteWidget = CreateWidget<UGameCompleteWidget>(this, GameCompleteWidgetBPClass);
}

void ARacerPlayerController::Tick(float Delta)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp() - GameStartTime));
}

void ARacerPlayerController::UpdateCheckpoint() 
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Checkpoint %d"), NextCheckpoint));
	Checkpoints[NextCheckpoint]->Disable();
	NextCheckpoint = (NextCheckpoint + 1) % Checkpoints.Num();
	Checkpoints[NextCheckpoint]->Enable();
	if (NextCheckpoint == 0)
	{
		CompleteGame();
	}
}

void ARacerPlayerController::CompleteGame()
{
	// Destroy actor
	GetPawn()->Destroy();

	// Construct & Display UI
	bShowMouseCursor = true; 
	SetInputMode(FInputModeUIOnly());
	GameCompleteWidget->TextTimeRecord->SetText(FText::AsTimespan(FDateTime::Now() - GameStartTime));
	GameCompleteWidget->AddToViewport();
}

void ARacerPlayerController::RestartGame()
{
	// Close UI
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
	GameCompleteWidget->RemoveFromViewport();

	// Spawn & Possess new actor
	FTransform PlayerStartTransform = GetWorld()->GetAuthGameMode()->FindPlayerStart(this)->GetActorTransform();
	//FVector PlayerStartLocation = GetWorld()->GetAuthGameMode()->FindPlayerStart(this)->GetActorLocation();
	//FRotator PlayerStartRotation(0.0f, 0.0f, 0.0f);

	//ASportVehicle* NewVehicle = GetWorld()->SpawnActor<ASportVehicle>(ASportVehicle::StaticClass(), PlayerStartLocation, PlayerStartRotation);
	ASportVehicle* NewVehicle = GetWorld()->SpawnActor<ASportVehicle>(ASportVehicle::StaticClass(), PlayerStartTransform);
	Possess(NewVehicle);

	// Reset game start time
	GameStartTime = FDateTime::Now();
}