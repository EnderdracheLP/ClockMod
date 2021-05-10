$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt
if ($args.Count -eq 0) {
$ModID = "clockmod"
$VERSION = "1.4.0-InDev"
}

if ($args[0] -eq "--actions") {
    $ModID = $env:module_id
    $VERSION = $env:version
}
echo "Building mod with ModID: $ModID version: $VERSION"
Copy-Item "./Android_Template.mk" "./Android.mk" -Force
(Get-Content "./Android.mk").replace('{VERSION}',   "$VERSION")       | Set-Content "./Android.mk"

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk

echo Done