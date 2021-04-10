# Builds a .qmod file for loading with QP
if ($args.Count -eq 0) {
$BSHook = "1_3_0"
$ModID = "clockmod"

echo "Compiling Mod"
& $PSScriptRoot/build.ps1
}

if ($args[0] -eq "--package") {
    $ModID = $env:module_id
    $BSHook = $env:bs_hook
    echo "Actions: Packaging QMod with ModID: $ModID"
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\mod.json", ".\Cover.png" -DestinationPath "./Temp$ModID.zip" -Update
    Move-Item "./Temp$ModID.zip" "./$ModID.qmod" -Force
}
if ($? -And ($args.Count -eq 0)) {
    echo "Packaging QMod with ModID: $ModID"
    Compress-Archive -Path "./libs/arm64-v8a/lib$ModID.so", "./libs/arm64-v8a/libbeatsaber-hook_$BSHook.so", ".\mod.json", ".\Cover.png" -DestinationPath "./Temp$ModID.zip" -Update
    Move-Item "./Temp$ModID.zip" "./$ModID.qmod" -Force
}
echo "Task Completed"