Param(
    [string]$Configuration = "Debug",
    [switch]$Clean,
    [switch]$Force
)

$ErrorActionPreference = "Stop"

$WorkingDirectory="$PSScriptRoot\Build"
$ENV:Path = $ENV:Path + ";$($Env:ProgramFiles)\CMake\bin"

function Find-Tool
{
    param(
        [Parameter(Mandatory=$true)]
        [string]$Command
    )
    Get-Command $Command | Select-Object -ExpandProperty Definition
}

function Install-Prerequisites
{
    $CMakeExe = (& Find-Tool "cmake.exe")
    if (-not $CMakeExe)
    {
        $WinGetExe = (& Find-Tool "winget.exe")
        if (-not $WinGetExe)
        {
            Write-Host -ForegroundColor "This script uses WinGet to install prerequisites, please install winget.exe."
            exit 1
        }

        Write-Host -ForegroundColor Green "This script uses WinGet to install prerequisites."
        $YN = Read-Host -Prompt "Are you okay with this? (y/n):"
        if ($YN -ieq "y" -or $YN -ieq "yes")
        {
            Write-Host -ForegroundColor Yellow "Installing CMake..."
            Start-Process $WinGetExe -ArgumentList `
                @("install", "-e", "--id", "Kitware.CMake", "--source", "winget", "--scope", "machine") -Verb RunAs -Wait
            Write-Host -ForegroundColor Yellow "Close this window and open a new PowerShell window to be sure your path is up to date."
            exit 0
        }
        else
        {
            Write-Host -ForegroundColor Yellow "Goodbye!"
            exit 1
        }
    }
}

function Start-Clean
{
    Write-Host -ForegroundColor Yellow "Cleaning working directory..."
    Remove-Item -Recurse -Force "$WorkingDirectory" -EA SilentlyContinue
}

function Start-Configure
{
    if (-not (Test-Path "$WorkingDirectory\CMakeCache.txt"))
    {
        Write-Host -ForegroundColor Yellow "Configuring CMake..."
        Remove-Item -Recurse -Force "$WorkingDirectory" -EA SilentlyContinue
        New-Item -Path "$WorkingDirectory" -ItemType Directory -Force
        Set-Location "$WorkingDirectory"
        # TODO: Detect and support latest Visual Studio that is installed here
        # Right now we are just hard-coded to VS 2017
        cmake.exe ../src -G "Visual Studio 15 2017 Win64"
    }
}

function Start-Build
{
    Set-Location "$WorkingDirectory"
    # TODO: Run build automatically
    # For now, just open Visual Studio 2017
    Write-Host -ForegroundColor Yellow "Opening Magenta.sln file..."
    & explorer.exe "$WorkingDirectory\Magenta.sln"
}

# Main Script
try
{
    Install-Prerequisites
    Push-Location
    if ($Clean -or $Force)
    {
        Start-Clean
    }
    Start-Configure
    Start-Build
}
finally
{
    Pop-Location
}
