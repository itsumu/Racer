// Fill out your copyright notice in the Description page of Project Settings.


#include "ReviveSpot.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AReviveSpot::AReviveSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
	    TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));
	
	Proxy = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Proxy"));
	SetRootComponent(Proxy);
    Proxy->SetStaticMesh(CubeMesh.Object);
    SetActorEnableCollision(false);
    SetActorHiddenInGame(true);
}

// Called when the game starts or when spawned
void AReviveSpot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReviveSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

