Param (
[Parameter(Mandatory=$false, HelpMessage="The version the mod should be compiled with")][Alias("ver")][string]$Version, # Currently not used
[Parameter(Mandatory=$false, HelpMessage="Switch to create a clean compilation")][Alias("rebuild")][Switch]$clean,
[Parameter(Mandatory=$false, HelpMessage="To create a release build")][Alias("publish")][Switch]$release,
[Parameter(Mandatory=$false, HelpMessage="To create a github actions build, assumes specific Environment variables are set")][Alias("github-build")][Switch]$actions,
[Parameter(Mandatory=$false, HelpMessage="To create a debug build (Does not strip other libs)")][Alias("debug-build")][Switch]$debugbuild # Not implemented yet
)


$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt

# Legacy arg check, might remove
for ($i = 0; $i -le $args.Length-1; $i++) {
echo "Arg $($i) is $($args[$i])"
    if ($args[$i] -eq "--actions") { $actions = $true }
    elseif ($args[$i] -eq "--release") { $release = $true }
}


if ($args.Count -eq 0 -or $actions -ne $true) {
    $qpmshared = "./qpm.shared.json"
    $qpmsharedJson = Get-Content $qpmshared -Raw | ConvertFrom-Json
    $ModID = $qpmsharedJson.config.info.id
    $VERSION = $qpmsharedJson.config.info.version.replace("-Dev", "")
    if ([string]::IsNullOrEmpty($VERSION)) {
        $VERSION = "0.0.1"
    }
    if ($release -ne $true) {
        $VERSION += "-Dev"
    }
}


if ($actions -eq $true) {
    $ModID = $env:module_id
    $VERSION = $env:version
    if ([string]::IsNullOrEmpty($VERSION)) {
        $qpmsharedJson = Get-Content $qpmshared -Raw | ConvertFrom-Json
        $VERSION = $qpmsharedJson.config.info.version.replace("-Dev", "")
    }
} else {
        & qpm package edit --version $VERSION
        # Temp Fix for qpm adding version.txt as library in extern.cmake
        #$cmakeFilePath = "./extern.cmake"; $externcmake = Get-Content $cmakeFilePath -Raw; $externcmake -replace [regex]::Escape('add_library(${COMPILE_ID} SHARED ${EXTERN_DIR}/includes/bs-cordl/version.txt)'), '' | Set-Content $cmakeFilePath
}

if ((Test-Path "./extern/includes/beatsaber-hook/shared/inline-hook/And64InlineHook.cpp", "./extern/includes/beatsaber-hook/shared/inline-hook/inlineHook.c", "./extern/includes/beatsaber-hook/shared/inline-hook/relocate.c") -contains $false) {
    Write-Host "Critical: Missing inline-hook"
    if (!(Test-Path "./extern/includes/beatsaber-hook/shared/inline-hook/And64InlineHook.cpp")) {
        Write-Host "./extern/includes/beatsaber-hook/shared/inline-hook/And64InlineHook.cpp"
    }
    if (!(Test-Path "./extern/includes/beatsaber-hook/shared/inline-hook/inlineHook.c")) {
        Write-Host "./extern/includes/beatsaber-hook/shared/inline-hook/inlineHook.c"
    }
        if (!(Test-Path "./extern/includes/beatsaber-hook/inline-hook/shared/relocate.c")) {
        Write-Host "./extern/includes/beatsaber-hook/shared/inline-hook/relocate.c"
    }
    Write-Host "Task Failed"
    exit 1;
}
Write-Output "Building mod with ModID: $ModID version: $VERSION"

if ($clean.IsPresent)
{
    if (Test-Path -Path "build")
    {
        remove-item build -R
    }
}

if (($clean.IsPresent) -or (-not (Test-Path -Path "build")))
{
    $out = new-item -Path build -ItemType Directory
}

Set-Location build
& cmake -G "Ninja" -DCMAKE_BUILD_TYPE="RelWithDebInfo" ../
& cmake --build . -j 6
$ExitCode = $LastExitCode
Set-Location ..
exit $ExitCode
Write-Output Done