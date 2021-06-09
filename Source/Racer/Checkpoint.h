// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

class UBoxComponent;

UCLASS()
class RACER_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	UBoxComponent* Collider;

public:	
	ACheckpoint();
	void Enable();
	void Disable();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
