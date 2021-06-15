// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "SportVehicle.generated.h"

class UAudioComponent;
class UCameraComponent;
class UParticleSystemComponent;
class UPhysicalMaterial;
class USpringArmComponent;
class UCapsuleComponent;
class UWheeledVehicleMovementComponent4W;

/**
 * 
 */
UCLASS()
class RACER_API ASportVehicle : public AWheeledVehicle
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	// Default chase camera
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ChaseCamera;

	// Friction scale for back wheels
		// Friction scale editable to override the default back tire friction
	UPROPERTY(Category = WheelFritionScale, EditAnywhere)
	float DefaultFrictionScale;
		// Friction scale for drifting purpose
	UPROPERTY(Category = WheelFritionScale, EditAnywhere)
	float DriftFrictionScale = 1.85f;

	// 4 wheel vehicle movement component
	UPROPERTY()
	UWheeledVehicleMovementComponent4W* Vehicle4W;

	// Damping rate for area out of track
	UPROPERTY(Category = DampingRate, EditAnywhere)
	float FloorDampingRate = 0.6f;

	UPROPERTY(Category = ParticleEffects, EditAnywhere)
	UParticleSystemComponent* LeftTrailEffect;

	UPROPERTY(Category = ParticleEffects, EditAnywhere)
	UParticleSystemComponent* RightTrailEffect;

	UPROPERTY(Category = ParticleEffects, EditAnywhere)
	UParticleSystemComponent* BoostEffect;
	
	UPROPERTY()
	UCapsuleComponent* Collider;

	UPROPERTY(Category = Sound, EditAnywhere)
	UAudioComponent* EngineSoundComponent;

	UPROPERTY(Category = Sound, EditAnywhere)
	UAudioComponent* SkidSoundComponent;

	UPROPERTY(Category = Sound, EditAnywhere)
	UAudioComponent* BoostSoundComponent;
	
	// Factor which is multiplied by mass to form final impulse value
	UPROPERTY(Category = Boost, EditAnywhere)
	float BoostFactor = 20.0f;

	// Max boost time in seconds
	UPROPERTY(Category = Boost, EditAnywhere)
	float MaxBoostTime = 0.25f;

	// Min speed to trigger or stop drifting
	UPROPERTY(Category = Drift, EditAnywhere)
	float MinDriftSpeed = 5.0f;
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	ASportVehicle();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void PressBrake();
	void ReleaseBrake();

private:
	static const float MaxAudioRPM;
	static const FName EngineAudioParamName;

	float DampingRate = 0.0f;
	bool bIsBoosted = false;
	bool bIsSkidding = false;
	bool bIsPressingBrake = false;
	float PreBoostTime = 0.0f;
	float BoostTime = 0.0f;
	void CheckCurrentGround();
	void DampWheels(float Rate);
	void DisableSlideFX();
	void EnableBoostFX();
	void UpdateEngineSound();
};
