# Builds a .qmod file for loading with QP
& $PSScriptRoot/build.ps1

$ModID = "clockmod"

if ($?) {
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", ".\mod.json" -DestinationPath "./$ModID.zip" -Update
    Remove-Item "./$ModID.qmod"
    Rename-Item "./$ModID.zip" "./$ModID.qmod"
}