// Fill out your copyright notice in the Description page of Project Settings.


#include "SportVehicle.h"

#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"
#include "WheeledVehicleMovementComponent4W.h"

#include "BackWheel.h"
#include "FrontWheel.h"

const float ASportVehicle::MaxAudioRPM = 2500.0f;
const FName ASportVehicle::EngineAudioParamName("RPM");

ASportVehicle::ASportVehicle() 
{
	// Vehicle mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VehicleMesh(TEXT("/Game/Vehicle/Vehicle_SkelMesh.Vehicle_SkelMesh"));
	GetMesh()->SetSkeletalMesh(VehicleMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> VehicleAnim(TEXT("/Game/Vehicle/Vehicle_Animation"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetAnimInstanceClass(VehicleAnim.Class);

	// Camera control
		// Arm spring for chase camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(-150.0f, 0.0f, 100.0f));
	SpringArm->SetWorldRotation(FRotator(-10.0f, 0.0f, 0.0f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 100.0f;

		// Chase camera
	ChaseCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	ChaseCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	ChaseCamera->bUsePawnControlRotation = false;

	// Wheels
	Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovementComponent());
	
	Vehicle4W->WheelSetups[0].WheelClass = UFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("PhysWheel_FL");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("PhysWheel_FR");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = UBackWheel::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("PhysWheel_BL");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UBackWheel::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("PhysWheel_BR");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	Vehicle4W->bDeprecatedSpringOffsetMode = true;

	// Wheel trail effects
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailEffect(TEXT("/Game/VFX/Vehicle_Trail.Vehicle_Trail"));
	LeftTrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LeftTrailEffect"));
	LeftTrailEffect->SetRelativeLocation(FVector(-40.0f, -32.0f, 0.0f));
	LeftTrailEffect->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
	LeftTrailEffect->SetTemplate(TrailEffect.Object);
	LeftTrailEffect->SetupAttachment(RootComponent);
	LeftTrailEffect->SetAutoActivate(false);

	RightTrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RightTrailEffect"));
	RightTrailEffect->SetRelativeLocation(FVector(-40.0f, 32.0f, 0.0f));
	RightTrailEffect->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
	RightTrailEffect->SetTemplate(TrailEffect.Object);
	RightTrailEffect->SetupAttachment(RootComponent);
	RightTrailEffect->SetAutoActivate(false);

	// Engine sound effects
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/Sounds/Engine_Loop_Cue.Engine_Loop_Cue"));
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(SoundCue.Object);
	EngineSoundComponent->SetupAttachment(RootComponent);
}

void ASportVehicle::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASportVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASportVehicle::MoveRight);
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ASportVehicle::PressBrake);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ASportVehicle::ReleaseBrake);
}

void ASportVehicle::MoveForward(float Value)
{
	//Vehicle4W->SetThrottleInput(Value);
	Vehicle4W->SetThrottleInput(Value * (1.0f - DampingRate));
	Vehicle4W->SetBrakeInput(Value * DampingRate);
}

void ASportVehicle::MoveRight(float Value)
{
	Vehicle4W->SetSteeringInput(Value);
}

void ASportVehicle::PressBrake()
{
	Vehicle4W->SetHandbrakeInput(true);
	Vehicle4W->Wheels[2]->TireConfig->SetFrictionScale(DriftFrictionScale);
	Vehicle4W->Wheels[3]->TireConfig->SetFrictionScale(DriftFrictionScale);
	LeftTrailEffect->Activate(true);
	RightTrailEffect->Activate(true);
}

void ASportVehicle::ReleaseBrake()
{
	Vehicle4W->SetHandbrakeInput(false);
	Vehicle4W->Wheels[2]->TireConfig->SetFrictionScale(DefaultFrictionScale);
	Vehicle4W->Wheels[3]->TireConfig->SetFrictionScale(DefaultFrictionScale);
	LeftTrailEffect->Deactivate();
	RightTrailEffect->Deactivate();
}

void ASportVehicle::DampWheels(float Rate)
{
	//for (auto& wheel : Vehicle4W->Wheels)
	//{
		//wheel->DampingRate = DampingRate;
	//}
	DampingRate = Rate;
}

void ASportVehicle::CheckCurrentGround()
{
	FVector RayStart = GetActorLocation() + FVector(0.0f, 0.0f, 1.0f); //todo: Calculate in location space, then transform to world space
	FVector RayEnd = GetActorLocation() - GetActorUpVector() * 100.0f;
	TArray< AActor* > ActorsToIgnore;
	FHitResult OutHit;

	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		RayStart,
		RayEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		//EDrawDebugTrace::ForOneFrame,
		EDrawDebugTrace::None,
		OutHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		0.0f);

	if (OutHit.Actor != nullptr)
	{
		if (OutHit.Actor->GetName() == TEXT("Floor"))
		{
			//if (GetName() == TEXT("Vehicle_Straight"))
			//{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("On floor"));
			//}
			DampWheels(FloorDampingRate);
		}
		else
		{
			//if (GetName() == TEXT("Vehicle_Straight"))
			//{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("On track"));
			//}
			DampWheels(0.0f);
		}
	}
}

void ASportVehicle::UpdateEngineSound()
{
	// Engine RPM to sound parameters
	float RPMToAudioScale = MaxAudioRPM / Vehicle4W->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter(EngineAudioParamName, Vehicle4W->GetEngineRotationSpeed() * RPMToAudioScale);
}

void ASportVehicle::BeginPlay()
{
	Super::BeginPlay();

	// Record default sliding friction
	DefaultFrictionScale = Vehicle4W->Wheels[2]->TireConfig->GetFrictionScale();

	// Play engine sound
	EngineSoundComponent->Play();
}

void ASportVehicle::Tick(float Delta)
{
	Super::Tick(Delta);

	// Check the ground material & Decide the damping rate
	CheckCurrentGround();

	// Update engine sound according to Engine RPM
	UpdateEngineSound();
}
