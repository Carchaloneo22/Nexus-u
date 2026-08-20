using UnrealBuildTool;

public class MCPGame : ModuleRules
{
	public MCPGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Json", "JsonUtilities", "Projects", "UMG", "Slate", "SlateCore", "HTTP" });
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
