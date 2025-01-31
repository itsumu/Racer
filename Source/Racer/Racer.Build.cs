// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Racer : ModuleRules
{
	public Racer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
			{"Core", "CoreUObject", "Engine", "InputCore", "PhysXVehicles", "HeadMountedDisplay"});

		PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{"Paper2D"});
	}
}
