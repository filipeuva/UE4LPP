using System.IO;
using UnrealBuildTool;

public class LivePP : ModuleRules
{
    public LivePP(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true;

        PrivateIncludePaths.AddRange(new string[] { "Private", "../LivePP/API" });
        PublicIncludePaths.AddRange(new string[] { "Public", "../LivePP/API" });
 
        PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"Projects"
			}
		);
	}
}