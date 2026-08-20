@echo off
REM Launch Unreal MCP Server + Unreal Editor + Claude

echo Starting MCP Server...
start "MCP Server" /min cmd /c "cd /d C:\Users\snayl\Documents\unreal-mcp\Python && uv run python unreal_mcp_server.py"

echo Waiting for MCP server to initialize...
timeout /t 3 /nobreak >nul

echo Starting Unreal Editor...
start "" "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe" "C:\Users\snayl\Documents\Unreal Projects\UnrealMCP\UnrealMCP.uproject"

echo Launching Claude...
start "" cmd /k "cd /d C:\Users\snayl\Documents\Unreal Projects\UnrealMCP && claude --continue"
