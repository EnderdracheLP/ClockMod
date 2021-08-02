$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt
if ($args.Count -eq 0  -or $args[0] -eq "--release") {
$ModID = "clockmod"
if ($args[0] -eq "--release") {
    $VERSION = "1.4.0"
} else {
    $VERSION = "1.4.0-InDev"
}
$BSHook = "2_2_4"
$codegen_ver = "0_12_5"
}

if ($args[0] -eq "--actions") {
    $ModID = $env:module_id
    $BSHook = $env:bs_hook
    $VERSION = $env:version
    $codegen_ver = $env:codegen
}
echo "Building mod with ModID: $ModID version: $VERSION, BS-Hook version: $BSHook"
Copy-Item "./Android_Template.mk" "./Android.mk" -Force
(Get-Content "./Android.mk").replace('{BS_Hook}',   "$BSHook")        | Set-Content "./Android.mk"
(Get-Content "./Android.mk").replace('{VERSION}',   "$VERSION")       | Set-Content "./Android.mk"
(Get-Content "./Android.mk").replace('{CG_VER}',    "$codegen_ver")   | Set-Content "./Android.mk"

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk -j 4 --output-sync=none

echo Done
