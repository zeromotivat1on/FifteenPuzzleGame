// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FifteenPuzzleGame : ModuleRules
{
	public FifteenPuzzleGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PublicIncludePaths.AddRange(new string[] 
        {
            "FifteenPuzzleGame/Public/Player",
            "FifteenPuzzleGame/Public/Puzzle",
            "FifteenPuzzleGame/Public/UI",
        });
    }
}
