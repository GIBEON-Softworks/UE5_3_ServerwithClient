// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HitUP : ModuleRules
{
    public HitUP(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.Add(ModuleDirectory);

        //�Ʒ� �߰��ؼ� ����ϸ� �˴ϴ�.
        PrivateDependencyModuleNames.Add("Core");
        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");


        PrivateDependencyModuleNames.Add("Slate");
        PrivateDependencyModuleNames.Add("SlateCore");

        //����
        PrivateDependencyModuleNames.Add("InputCore");
        PrivateDependencyModuleNames.Add("EnhancedInput");

        //����
        PrivateDependencyModuleNames.Add("UMG");
        //PrivateDependencyModuleNames.Add("UMGEditor"); !packaging error!

        //���
        PrivateDependencyModuleNames.Add("Sockets");
        PrivateDependencyModuleNames.Add("Networking");
        PrivateDependencyModuleNames.Add("HTTP");
        PrivateDependencyModuleNames.Add("Json");
        PrivateDependencyModuleNames.Add("JsonUtilities");
        PrivateDependencyModuleNames.Add("OnlineSubsystem");
       


        //Controller for AI
        PublicDependencyModuleNames.Add("AIModule");
        PublicDependencyModuleNames.Add("GameplayTasks");
        PublicDependencyModuleNames.Add("NavigationSystem");
        PublicDependencyModuleNames.Add("WebBrowser");

        PublicDependencyModuleNames.Add("ProceduralMeshComponent");

    }

}