& $PSScriptRoot/build.ps1
if ($?) {
    qpm-rust qmod build
    $modJson = Get-Content "./mod.json" -Raw | ConvertFrom-Json
    foreach ($mod in $modJson.modFiles)
    {
        $path = "./build/" + $mod
        if (-not (Test-Path $path))
        {
            $path = "./extern/libs/" + $mod
        }
        adb push $path /sdcard/Android/data/com.beatgames.beatsaber/files/mods/$mod
    }

    # foreach ($lib in $modJson.libraryFiles)
    # {
    #     $path = "./extern/libs/" + $lib
    #     if (-not (Test-Path $path))
    #     {
    #         $path = "./build/" + $lib
    #     }
    #     adb push $path /sdcard/Android/data/com.beatgames.beatsaber/files/libs/$lib
    # }
    if ($?) {
        adb shell am force-stop com.beatgames.beatsaber
        adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
        if ($args[0] -eq "--log") {
        #Start-Process PowerShell -ArgumentList '-NoExit',"./logging.ps1 --file"
        Start-Process PowerShell -ArgumentList "./logging.ps1 --file"
        }
        if ($args[0] -eq "--log-simple") {
        #Start-Process PowerShell -ArgumentList '-NoExit',"./logging.ps1 --file"
        Start-Process PowerShell -ArgumentList "./logging.ps1 --file-simple"
        }
    }
}
echo "Exiting Copy.ps1"