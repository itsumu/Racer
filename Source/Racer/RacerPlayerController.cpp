// Fill out your copyright notice in the Description page of Project Settings.


#include "RacerPlayerController.h"

#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"

#include "Checkpoint.h"
#include "GameCompleteWidget.h"
#include "GameOverWidget.h"
#include "ReviveSpot.h"
#include "SportVehicle.h"

ARacerPlayerController::ARacerPlayerController()
{
	bAutoManageActiveCameraTarget = true;

	// UI
	static ConstructorHelpers::FClassFinder<UUserWidget> GameWidgetBP(TEXT("/Game/UI/GameComplete"));
	check(GameWidgetBP.Succeeded());
	GameCompleteWidgetBPClass = GameWidgetBP.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidgetBP(TEXT("/Game/UI/GameOver"));
	check(GameOverWidgetBP.Succeeded());
	GameOverWidgetBPClass = GameOverWidgetBP.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> MinimapWidgetBP(TEXT("/Game/UI/Minimap"));
	check(MinimapWidgetBP.Succeeded());
	MinimapWidgetBPClass = MinimapWidgetBP.Class;
}

void ARacerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Retrieve input control
	SetInputMode(FInputModeGameOnly());

	// Timing
	GameStartTime = FDateTime::Now();

	// Checkpoints generation
	FString MapName = GetWorld()->GetMapName();
	FVector CheckpointStartLocation;
	FVector CheckpointMiddleLocation;
	FVector CheckpointEndLocation;
	FRotator Rotation;
	if (MapName.Find(TEXT("LoopTrack")) != -1)
	{
		//CheckpointStartLocation = FVector(-9150.0f, -4490.0f, 260.0f);
		//CheckpointMiddleLocation = FVector(-8150.0f, -4490.0f, 260.0f);
		//CheckpointEndLocation = FVector(-7150.0f, -4490.0f, 260.0f);

		CheckpointStartLocation = FVector(-1290.0f, -5000.0f, 0.0f);
		CheckpointMiddleLocation = FVector(110.0f, 5000.0f, 500.0f);
		CheckpointEndLocation = FVector(-1510.0f, -5000.0f, 0.0f);
		Rotation = FRotator(0.0f, 0.0f, 0.0f);
	}
	else if (MapName.Find(TEXT("SpaceTrack")) != -1)
	{
		CheckpointStartLocation = FVector(-3100.0f, -4860.0f, 3000.0f);
		CheckpointMiddleLocation = FVector(-1830.0f, 420.0f, 2800.0f);
		CheckpointEndLocation = FVector(-3360.0f, -4860.0f, 3000.0f);
		Rotation = FRotator(0.0f, 0.0f, 0.0f);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, MapName);
	}
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
	GameOverWidget = CreateWidget<UGameOverWidget>(this, GameOverWidgetBPClass);
	MinimapWidget = CreateWidget<UUserWidget>(this, MinimapWidgetBPClass);
	MinimapWidget->AddToViewport();

	// Pause flag
	bVehicleIsRunning = true;
}

void ARacerPlayerController::Tick(float Delta)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%d"), FDateTime::Now().ToUnixTimestamp() - GameStartTime));
	
	if (bVehicleIsRunning)
	{
		// Terminate game when vehicle go out of bound
		FVector PawnLocation = GetPawn()->GetActorLocation();
		float MaxHeightDiff = 2000.0f;
		if (PawnLocation.Z < GetWorld()->GetAuthGameMode()->FindPlayerStart(this)->GetActorLocation().Z - MaxHeightDiff) {
			TerminateGame();
		}
	}
}

void ARacerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Revive", IE_Pressed, this, &ARacerPlayerController::Revive);
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
	// Destroy pawn
	GetPawn()->Destroy();

	// Construct & Display UI
	bShowMouseCursor = true; 
	SetInputMode(FInputModeUIOnly());
	GameCompleteWidget->TextTimeRecord->SetText(FText::AsTimespan(FDateTime::Now() - GameStartTime));
	GameCompleteWidget->AddToViewport();

	// Pause flag
	bVehicleIsRunning = false;
}

void ARacerPlayerController::TerminateGame()
{
	// Destroy pawn
	GetPawn()->Destroy();

	// Construct & Display UI
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
	GameOverWidget->AddToViewport();

	// Pause flag
	bVehicleIsRunning = false;
}

void ARacerPlayerController::RestartGame()
{
	// Close UI
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
	GameCompleteWidget->RemoveFromViewport();
	GameOverWidget->RemoveFromViewport();

	// Spawn & Possess new actor
	FTransform PlayerStartTransform = GetWorld()->GetAuthGameMode()->FindPlayerStart(this)->GetActorTransform();

	ASportVehicle* NewVehicle = GetWorld()->SpawnActor<ASportVehicle>(ASportVehicle::StaticClass(), PlayerStartTransform);
	Possess(NewVehicle);

	// Reset game start time
	GameStartTime = FDateTime::Now();

	// Pause flag
	bVehicleIsRunning = true;
}

void ARacerPlayerController::Revive()
{
	TArray<AActor*> ReviveSpots;
	float MinDistance = 100000.0f;
	int32 MinIndex = -1;
	int32 Index = 0;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AReviveSpot::StaticClass(), ReviveSpots);
	
	for (AActor* ReviveSpot: ReviveSpots)
	{
		float Distance = FVector::Dist(ReviveSpot->GetActorLocation(), GetPawn()->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinIndex = Index;
			MinDistance = Distance;
		}
		++Index;
	}

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, GetPawn()->GetActorLocation().ToString());
	// Destroy current vehicle, Then respawn & possess new vehicle
	GetPawn()->Destroy();
	FTransform ReviveTransform = ReviveSpots[MinIndex]->GetActorTransform();

	ASportVehicle* NewVehicle = GetWorld()->SpawnActor<ASportVehicle>(ASportVehicle::StaticClass(), ReviveTransform);
	Possess(NewVehicle);
}
