// Copyright Epic Games, Inc. All Rights Reserved.

#include "RacerGameMode.h"

#include "RacerHud.h"
#include "RacerPlayerController.h"
#include "SportVehicle.h"

ARacerGameMode::ARacerGameMode()
{
	PlayerControllerClass = ARacerPlayerController::StaticClass();
	DefaultPawnClass = ASportVehicle::StaticClass();
	//HUDClass = ARacerHud::StaticClass();
}
