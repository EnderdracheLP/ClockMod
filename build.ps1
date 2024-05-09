$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt
for ($i = 0; $i -le $args.Length-1; $i++) {
echo "Arg $($i) is $($args[$i])"
    if ($args[$i] -eq "--actions") { $actions = $true }
    elseif ($args[$i] -eq "--release") { $release = $true }
}
if ($args.Count -eq 0 -or $actions -ne $true) {
$ModID = "ClockMod"
$VERSION = "1.8.0"
    if ($release -ne $true) {
        $VERSION += "-Dev"
    }
}


if ($actions -eq $true) {
    $ModID = $env:module_id
    $BSHook = $env:bs_hook
    $VERSION = $env:version
    $codegen_ver = $env:codegen
} else {
        & qpm-rust package edit --version $VERSION
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
echo "Building mod with ModID: $ModID version: $VERSION"

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

cd build
& cmake -G "Ninja" -DCMAKE_BUILD_TYPE="RelWithDebInfo" ../
& cmake --build . -j 6
$ExitCode = $LastExitCode
cd ..
exit $ExitCode
echo Done