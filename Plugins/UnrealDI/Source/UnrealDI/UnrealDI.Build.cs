namespace UnrealBuildTool.Rules
{
	public class UnrealDI : ModuleRules
	{
		public UnrealDI(ReadOnlyTargetRules Target)
            : base(Target)
		{
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
            bEnforceIWYU = true;

            PrivateIncludePaths.AddRange(
                new string[]
                {
                    "UnrealDI/Private"
                });

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine"
                });
		}
	}
}
