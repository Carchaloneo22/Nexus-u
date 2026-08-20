#Requires -RunAsAdministrator
<#
.SYNOPSIS
    Setup UnrealMCP plugin as junction and compile for UE 5.7.
.DESCRIPTION
    1. Closes/rename old binary-only plugin.
    2. Creates junction to external MCP source.
    3. Generates Visual Studio project files.
    4. Compiles Development Editor.
.NOTES
    Close Unreal Editor before running.
#>

$ErrorActionPreference = "Stop"

$ProjectRoot = "C:\Users\snayl\Documents\Unreal Projects\UnrealMCP"
$PluginDir = Join-Path $ProjectRoot "Plugins\UnrealMCP"
$PluginOld = Join-Path $ProjectRoot "Plugins\UnrealMCP_old"
$McpSource = "C:\Users\snayl\Documents\unreal-mcp\MCPGameProject\Plugins\UnrealMCP"
$Uproject = Join-Path $ProjectRoot "UnrealMCP.uproject"
$EngineBuildBat = "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat"

Write-Host "=== UnrealMCP Plugin Setup ===" -ForegroundColor Cyan

# 1. Check Unreal Editor is closed
$ueProc = Get-Process -Name "UnrealEditor" -ErrorAction SilentlyContinue
if ($ueProc) {
    Write-Warning "UnrealEditor is running. Please close it first, then rerun this script."
    exit 1
}

# 2. Remove old backup if exists
if (Test-Path $PluginOld) {
    Write-Host "Removing old backup plugin folder..."
    Remove-Item -LiteralPath $PluginOld -Recurse -Force
}

# 3. Rename current plugin folder if it exists and is NOT a junction
if (Test-Path $PluginDir) {
    $item = Get-Item $PluginDir
    if ($item.Attributes -match "ReparsePoint") {
        Write-Host "Removing existing junction..."
        Remove-Item -LiteralPath $PluginDir -Force
    } else {
        Write-Host "Renaming current plugin folder to UnrealMCP_old..."
        Rename-Item -LiteralPath $PluginDir -NewName "UnrealMCP_old" -Force
    }
}

# 4. Verify source exists
if (-not (Test-Path $McpSource)) {
    Write-Error "MCP source not found at: $McpSource"
    exit 1
}

# 5. Create junction
Write-Host "Creating junction from Plugins\UnrealMCP -> $McpSource"
New-Item -ItemType Junction -Path $PluginDir -Target $McpSource | Out-Null
Write-Host "Junction created successfully." -ForegroundColor Green

# 6. Generate project files
Write-Host "Generating Visual Studio project files..."
& "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="$Uproject" -game -engine -progress
if ($LASTEXITCODE -ne 0) {
    Write-Error "Failed to generate project files."
    exit 1
}
Write-Host "Project files generated." -ForegroundColor Green

# 7. Ensure minimal Source folder exists for UBT
$SourceDir = Join-Path $ProjectRoot "Source\MCPGame"
if (-not (Test-Path $SourceDir)) {
    Write-Host "Creating minimal C++ module stub for UBT..."
    New-Item -ItemType Directory -Path "$SourceDir\Public" -Force | Out-Null
    New-Item -ItemType Directory -Path "$SourceDir\Private" -Force | Out-Null
    @"
using UnrealBuildTool;
public class MCPGame : ModuleRules
{
    public MCPGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
    }
}
"@ | Set-Content "$SourceDir\MCPGame.Build.cs"
    "#pragma once`n#include `"CoreMinimal.h`"" | Set-Content "$SourceDir\Public\MCPGame.h"
    "#include `"MCPGame.h`"`n#include `"Modules/ModuleManager.h`"`nclass FMCPGameModule : public IModuleInterface`n{`npublic:`n    virtual void StartupModule() override {}`n    virtual void ShutdownModule() override {}`n};`nIMPLEMENT_MODULE(FMCPGameModule, MCPGame)" | Set-Content "$SourceDir\Private\MCPGame.cpp"
    $TargetDir = Join-Path $ProjectRoot "Source"
    @"
using UnrealBuildTool;
public class UnrealMCPTarget : TargetRules
{
    public UnrealMCPTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.AddRange(new string[] { "MCPGame" });
    }
}
"@ | Set-Content "$TargetDir\UnrealMCP.Target.cs"
    @"
using UnrealBuildTool;
public class UnrealMCPEditorTarget : TargetRules
{
    public UnrealMCPEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.AddRange(new string[] { "MCPGame" });
    }
}
"@ | Set-Content "$TargetDir\UnrealMCPEditor.Target.cs"
}

# 8. Compile Development Editor (use UE auto-compile compatible command)
Write-Host "Compiling Development Editor..."
& $EngineBuildBat Development Win64 -Project="$Uproject" -TargetType=Editor -waitmutex
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed. Check output above."
    exit 1
}
Write-Host "Build successful!" -ForegroundColor Green
Write-Host "You can now launch the project with launch_project.bat" -ForegroundColor Cyan
