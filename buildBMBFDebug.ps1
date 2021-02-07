# Builds a .zip file for loading with BMBF
& $PSScriptRoot/build.ps1

if ($?) {
    Compress-Archive -Path "./obj/local/arm64-v8a/libclockmod.so", "./obj/local/arm64-v8a/libbeatsaber-hook_1_0_12.so", "./bmbfmod.json","./Cover.png" -DestinationPath "./ClockmodDebug.zip" -Update
}
echo "Task Completed"
