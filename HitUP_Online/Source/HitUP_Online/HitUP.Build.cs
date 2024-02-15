// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HitUP : ModuleRules
{
	public HitUP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Sockets", "Networking", "AIModule", "NavigationSystem" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
	
}

