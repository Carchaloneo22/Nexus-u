using UnrealBuildTool;
using System.Collections.Generic;

public class UnrealMCPTarget : TargetRules
{
	public UnrealMCPTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		ExtraModuleNames.AddRange(new string[] { "MCPGame" });
	}
}
