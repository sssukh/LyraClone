// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LyraClone : ModuleRules
{
	public LyraClone(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			// GAS
			"GameplayTags", 
			"GameplayTasks",
			"GameplayAbilities",
			// Game Features
			"ModularGameplay", 
			"GameFeatures" ,
			"ModularGameplayActors",
			// Input
			"InputCore",
			"EnhancedInput",
			// CommonUser
			"CommonUser",
			// CommonGame
			"CommonGame",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
