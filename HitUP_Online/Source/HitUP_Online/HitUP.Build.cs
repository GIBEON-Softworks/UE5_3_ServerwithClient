// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HitUP : ModuleRules
{
    public HitUP(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.Add(ModuleDirectory);

        //아래 추가해서 사용하면 됩니다. 
        PrivateDependencyModuleNames.Add("Core");
        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");

        PrivateDependencyModuleNames.Add("Slate");
        PrivateDependencyModuleNames.Add("SlateCore");

        //동작
        PrivateDependencyModuleNames.Add("InputCore");
        PrivateDependencyModuleNames.Add("EnhancedInput");

        //위젯
        PrivateDependencyModuleNames.Add("UMG");
        //PrivateDependencyModuleNames.Add("UMGEditor"); !packaging error!

        //통신
        PrivateDependencyModuleNames.Add("Sockets");
        PrivateDependencyModuleNames.Add("Networking");

        //Controller for AI
        PublicDependencyModuleNames.Add("AIModule");
        PublicDependencyModuleNames.Add("GameplayTasks");
        PublicDependencyModuleNames.Add("NavigationSystem");

        PublicDependencyModuleNames.Add("ProceduralMeshComponent");

    }

}