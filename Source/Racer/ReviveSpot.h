// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReviveSpot.generated.h"

class UStaticMeshComponent;

UCLASS()
class RACER_API AReviveSpot : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	UStaticMeshComponent* Proxy;
	
public:	
	// Sets default values for this actor's properties
	AReviveSpot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
