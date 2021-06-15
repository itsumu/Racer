// Fill out your copyright notice in the Description page of Project Settings.


#include "SportVehicle.h"

#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/InputComponent.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/BoxSphereBounds.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"
#include "WheeledVehicleMovementComponent4W.h"

#include "BackWheel.h"
#include "Checkpoint.h"
#include "FrontWheel.h"
#include "RacerPlayerController.h"

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

	// Visual effects
		// Wheel trails
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailEffect(TEXT("/Game/VFX/Vehicle_Trail.Vehicle_Trail"));
	LeftTrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LeftTrailEffect"));
	LeftTrailEffect->SetTemplate(TrailEffect.Object);
	LeftTrailEffect->SetRelativeLocation(FVector(-40.0f, -32.0f, 0.0f));
	LeftTrailEffect->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
	LeftTrailEffect->SetupAttachment(RootComponent);

	RightTrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RightTrailEffect"));
	RightTrailEffect->SetTemplate(TrailEffect.Object);
	RightTrailEffect->SetRelativeLocation(FVector(-40.0f, 32.0f, 0.0f));
	RightTrailEffect->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
	RightTrailEffect->SetupAttachment(RootComponent);

		// Boost
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BoostVFX(
		TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	BoostEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BoostVisualEffect"));
	BoostEffect->SetTemplate(BoostVFX.Object);
	BoostEffect->SetRelativeLocation(FVector(-80.0f, 0.0f, 0.0f));
	BoostEffect->SetupAttachment(RootComponent);
	
	// Sound effects
		// Engine 
	static ConstructorHelpers::FObjectFinder<USoundCue> EngineSoundCue(
		TEXT("/Game/Sounds/Engine_Loop_Cue.Engine_Loop_Cue"));
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(EngineSoundCue.Object);
	EngineSoundComponent->SetupAttachment(RootComponent);

		// Tire skid
	static ConstructorHelpers::FObjectFinder<USoundWave> SkidSoundWave(TEXT("/Game/Sounds/tires_skid.tires_skid"));
	SkidSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SkidSound"));
	SkidSoundComponent->SetSound(SkidSoundWave.Object);
	SkidSoundComponent->SetupAttachment(RootComponent);

		// Speed boost
	static ConstructorHelpers::FObjectFinder<USoundCue> BoostSoundCue(
		TEXT("/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue"));
	BoostSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BoostSound"));
	BoostSoundComponent->SetSound(BoostSoundCue.Object);
	BoostSoundComponent->SetupAttachment(RootComponent);
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
	bIsPressingBrake = true;
	
	// Vehicle4W->SetHandbrakeInput(true);
	Vehicle4W->Wheels[2]->TireConfig->SetFrictionScale(DriftFrictionScale);
	Vehicle4W->Wheels[3]->TireConfig->SetFrictionScale(DriftFrictionScale);
	if (Vehicle4W->GetForwardSpeed() * 0.036f > MinDriftSpeed)
	{
		bIsSkidding = true;
		LeftTrailEffect->ActivateSystem();
		RightTrailEffect->ActivateSystem();
		SkidSoundComponent->Play();
	}
}

void ASportVehicle::ReleaseBrake()
{
	bIsPressingBrake = false;
	bIsSkidding = false;

	// Vehicle4W->SetHandbrakeInput(false);
	Vehicle4W->Wheels[2]->TireConfig->SetFrictionScale(DefaultFrictionScale);
	Vehicle4W->Wheels[3]->TireConfig->SetFrictionScale(DefaultFrictionScale);
	DisableSlideFX();

	EnableBoostFX();
	bIsBoosted = true;
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
	FVector RayStart = GetActorTransform().TransformPosition(FVector(0.0f, 0.0f, 10.0f));
	FVector RayEnd = GetActorLocation() - GetActorUpVector() * 100.0f;
	TArray< AActor* > ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult OutHit;

	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		RayStart,
		RayEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		// EDrawDebugTrace::ForOneFrame,
		EDrawDebugTrace::None,
		OutHit,
		false,
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
	else
	{ // In air, adjust angular velocity to balance the vehicle
		Cast<UPrimitiveComponent>(Vehicle4W->UpdatedComponent)->SetPhysicsAngularVelocityInDegrees(
		FVector(0, 0.1f, 0));
	}
}

void ASportVehicle::DisableSlideFX()
{
	LeftTrailEffect->DeactivateSystem();
	RightTrailEffect->DeactivateSystem();
	SkidSoundComponent->Stop();
}

void ASportVehicle::EnableBoostFX()
{
	BoostSoundComponent->Play();
	BoostEffect->ActivateSystem();
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

	// Overlap events
		// Checkpoint check
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ASportVehicle::BeginOverlap);

	// Audios
	BoostSoundComponent->Stop();
	SkidSoundComponent->Stop();

	// Particles
	BoostEffect->DeactivateSystem();
	LeftTrailEffect->DeactivateSystem();
	RightTrailEffect->DeactivateSystem();
	
	// Debugging area
	// FVector Origin;
	// FVector Extent;
	// float SphereRadius;
	//
	// UKismetSystemLibrary::GetComponentBounds(GetMesh(), Origin, Extent, SphereRadius);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, Origin.ToString());
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, Extent.ToString());	
}

void ASportVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check the ground material & Decide the damping rate
	CheckCurrentGround();

	// Update engine sound according to Engine RPM
	UpdateEngineSound();

	// Speed boost
	if (bIsBoosted)
	{
		if (PreBoostTime <= 0.0f)
		{
			PreBoostTime += DeltaTime;
		}
		else
		{
			FVector Impulse = GetActorForwardVector() * Vehicle4W->Mass * BoostFactor;
			FVector ImpulsePosition = GetActorTransform().TransformPosition(FVector(0.0f, 0.0f, 30.0f));
			GetMesh()->AddImpulseAtLocation(Impulse, ImpulsePosition);
			BoostTime += DeltaTime;
		}
	}
	if (BoostTime >= MaxBoostTime)
	{
		PreBoostTime = 0.0f;
		BoostTime = 0.0f;
		bIsBoosted = false;
	}

	// Slide FX update
	if (bIsPressingBrake && bIsSkidding)
	{
		if (Vehicle4W->GetForwardSpeed() * 0.036f < MinDriftSpeed)
		{
			bIsSkidding = false;
			DisableSlideFX();
		}
	}

	// Debugging area
	// FVector AngularVelocity = GetActorTransform().Transform;
	
	// GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Blue, FString::Printf(TEXT("%f"), Vehicle4W->GetForwardSpeed()));
}

void ASportVehicle::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, OtherActor->GetName());
	if (OtherActor->GetClass()->IsChildOf(ACheckpoint::StaticClass()))
	{
		ARacerPlayerController* PlayerController = Cast<ARacerPlayerController>(GetController());
		PlayerController->UpdateCheckpoint();
	}
}