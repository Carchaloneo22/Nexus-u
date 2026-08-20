@echo off
REM Run PowerShell setup script with execution policy bypass for this session
echo Running UnrealMCP plugin setup (requires Administrator)...
powershell -ExecutionPolicy Bypass -File "%~dp0setup_mcp_plugin.ps1"
pause
