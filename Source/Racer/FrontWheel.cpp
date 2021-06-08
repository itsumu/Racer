// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontWheel.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"

UFrontWheel::UFrontWheel()
{
	ShapeRadius = 18.0f;
	ShapeWidth = 15.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 40.0f;
	DampingRate = 0.0f;

	// Find the tire object and set the data for it
	static ConstructorHelpers::FObjectFinder<UTireConfig> TireConf(TEXT("/Game/Vehicle/Wheel/Wheel_FrontTire.Wheel_FrontTire"));
	TireConfig = TireConf.Object;
}