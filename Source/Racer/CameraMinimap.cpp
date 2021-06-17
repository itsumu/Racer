// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraMinimap.h"

#include "Camera/CameraComponent.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

ACameraMinimap::ACameraMinimap()
{
	SetActorRotation(FRotator(-90.0f, 0.0f, 0.0f));

	// Debug camera
	GetCameraComponent()->SetAspectRatio(1.0f);

	// Capture
	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderTarget(
		TEXT("/Game/Materials/Minimap_RenderTarget.Minimap_RenderTarget"));
	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	CaptureComponent->ProjectionType = ECameraProjectionMode::Type::Orthographic;
	CaptureComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CaptureComponent->ShowFlags.SkeletalMeshes = false;
	CaptureComponent->TextureTarget = RenderTarget.Object;
	CaptureComponent->TextureTarget->TargetGamma = 2.0f;
	CaptureComponent->SetupAttachment(RootComponent);
}
