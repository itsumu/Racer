// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class RacerEditorTarget : TargetRules
{
	public RacerEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Racer");
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            bOverrideBuildEnvironment = true;
            AdditionalCompilerArguments = "/Zm2000";
        }
    }
}
