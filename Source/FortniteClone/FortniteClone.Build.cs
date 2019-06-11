// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FortniteClone : ModuleRules
{
	public FortniteClone(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;
        bEnforceIWYU = true;
        if (Target.Configuration != UnrealTargetConfiguration.Shipping)
        {
            bFasterWithoutUnity = true;
        }
        //bForceEnableExceptions = true;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "GameLiftServerSDK", "GameLiftClientSDK"});
        if(Target.Configuration != UnrealTargetConfiguration.Shipping)
        {
            MinFilesUsingPrecompiledHeaderOverride = 1;
            bFasterWithoutUnity = true;
        }
    }
}
