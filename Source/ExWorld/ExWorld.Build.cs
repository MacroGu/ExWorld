// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ExWorld : ModuleRules
{
	public ExWorld(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.AddRange(
			new string[] {
						// ... add public include paths required here ...
						ModuleDirectory,
						"ExWorld/Public",
						"ExWorld/Public/UI"
			}
		);
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
	}
}
