using System.IO;
using UnrealBuildTool;

public class LivePP : ModuleRules
{
    public LivePP(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true;

        PrivateIncludePaths.AddRange(new string[] { "Private", "../API" });
        PublicIncludePaths.AddRange(new string[] { "Public", "../API" });
 
        PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"Projects"
			}
		);
	}
}