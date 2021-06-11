// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"

#include "Components/BoxComponent.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
    SetRootComponent(Collider);
	Collider->SetWorldScale3D(FVector(10.0f, 10.0f, 10.0f));
	Collider->SetVisibility(true);
	Collider->bHiddenInGame = false;
	Collider->SetGenerateOverlapEvents(false);
}

void ACheckpoint::Enable()
{
	Collider->SetGenerateOverlapEvents(true);
}

void ACheckpoint::Disable()
{
	Collider->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

