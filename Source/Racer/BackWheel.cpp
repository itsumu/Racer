// Fill out your copyright notice in the Description page of Project Settings.


#include "BackWheel.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"

UBackWheel::UBackWheel()
{
	ShapeRadius = 18.0f;
	ShapeWidth = 15.0f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.0f;
	DampingRate = 0.0f;

	// Find the tire object and set the data for it
	static ConstructorHelpers::FObjectFinder<UTireConfig> TireData(TEXT("/Game/Vehicle/Wheel/Wheel_BackTire.Wheel_BackTire"));
	TireConfig = TireData.Object;
}