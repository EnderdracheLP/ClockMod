# Builds a .zip file for loading with BMBF
& $PSScriptRoot/build.ps1

if ($?) {
    Compress-Archive -Path "./libs/arm64-v8a/libclockmod.so", "./libs/arm64-v8a/libbeatsaber-hook_1_0_12.so", "./bmbfmod.json","./Cover.png" -DestinationPath "./Clockmod.zip" -Update
}
