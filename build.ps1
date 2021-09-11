$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt
if ($args.Count -eq 0  -or $args[0] -eq "--release") {
$ModID = "clockmod"
$VERSION = "1.5.1"
if ($args[0] -ne "--release") {
    $VERSION += "-Dev"
}
$BSHook = "2_3_0"
}

if ($args[0] -eq "--actions") {
    $ModID = $env:module_id
    $BSHook = $env:bs_hook
    $VERSION = $env:version
}
echo "Building mod with ModID: $ModID version: $VERSION, BS-Hook version: $BSHook"
Copy-Item "./Android_Template.mk" "./Android.mk" -Force
(Get-Content "./Android.mk").replace('{BS_Hook}',   "$BSHook")        | Set-Content "./Android.mk"
(Get-Content "./Android.mk").replace('{VERSION}',   "$VERSION")       | Set-Content "./Android.mk"

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk -j 4 --output-sync=none

echo Done
