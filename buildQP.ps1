# Builds a .qmod file for loading with QP
& $PSScriptRoot/build.ps1

$ModID = "clockmod"
$BSHook = "1_2_4"

if ($?) {
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\mod.json" -DestinationPath "./Temp$ModID.zip" -Update
    Remove-Item "./$ModID.qmod"
    Rename-Item "./Temp$ModID.zip" "./$ModID.qmod"
}
echo "Task Completed"