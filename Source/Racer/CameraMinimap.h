// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "CameraMinimap.generated.h"

class USceneCaptureComponent2D;

UCLASS()
class RACER_API ACameraMinimap : public ACameraActor
{
	GENERATED_BODY()

	UPROPERTY(Category = Capture, EditAnywhere)
	USceneCaptureComponent2D* CaptureComponent;
	
public:
	ACameraMinimap();
};
