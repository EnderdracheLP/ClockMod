# Builds a .zip file for loading with BMBF
if ($args.Count -eq 0) {
$VERSION = "1.4.0-InDev"
& $PSScriptRoot/build.ps1
}

    Copy-Item "./bmbfmod_Template.json" "./bmbfmod.json" -Force

if ($args[0] -eq "--package") {
echo "Actions: Packaging BMBF Mod"
    (Get-Content "./bmbfmod.json").replace('{VERSION_NUMBER_PLACEHOLDER}', "$env:version") | Set-Content "./bmbfmod.json"
    Compress-Archive -Path "./libs/arm64-v8a/libclockmod.so", "./libs/arm64-v8a/libbeatsaber-hook_1_0_12.so", "./bmbfmod.json","./Cover.png" -DestinationPath "./Clockmod.zip" -Update
}

if ($? -And $args.Count -eq 0) {
    echo "Packaging BMBF Mod"
    (Get-Content "./bmbfmod.json").replace('{VERSION_NUMBER_PLACEHOLDER}', "$env:version") | Set-Content "./bmbfmod.json"
    Compress-Archive -Path "./libs/arm64-v8a/libclockmod.so", "./libs/arm64-v8a/libbeatsaber-hook_1_0_12.so", "./bmbfmod.json","./Cover.png" -DestinationPath "./Clockmod.zip" -Update
}
echo "Task Completed"
