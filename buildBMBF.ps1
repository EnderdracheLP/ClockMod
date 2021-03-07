# Builds a .zip file for loading with BMBF
& $PSScriptRoot/build.ps1

if ($?) {
    Compress-Archive -Path "./libs/arm64-v8a/libclockmod.so", "./bmbfmod.json","./Cover.png" -DestinationPath "./Clockmod.zip" -Update
}
echo "Task Completed"
