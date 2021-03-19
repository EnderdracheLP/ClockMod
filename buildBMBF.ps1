# Builds a .zip file for loading with BMBF
& $PSScriptRoot/build.ps1

$ModID = "clockmod"
$BSHook = "1_2_3"

if ($?) {
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\bmbfmod.json" -DestinationPath "./$ModID.zip" -Update
}
echo "Task Completed"
