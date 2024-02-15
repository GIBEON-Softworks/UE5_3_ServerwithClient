// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HitUP : ModuleRules
{
	public HitUP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
      
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
	
}

