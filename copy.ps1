& $PSScriptRoot/build.ps1
if ($?) {
    adb push libs/arm64-v8a/libclockmod.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libclockmod.so
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