& $PSScriptRoot/build.ps1
if ($?) {
    adb push libs/arm64-v8a/libclockmod.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libclockmod.so
#    adb push libs/arm64-v8a/libbeatsaber-hook_1_2_3.so /sdcard/Android/data/com.beatgames.beatsaber/files/libs/libbeatsaber-hook_1_2_3.so
#    adb push libs/arm64-v8a/libquestui.so /sdcard/Android/data/com.beatgames.beatsaber/files/libs/libquestui.so
#    adb push libs/arm64-v8a/libcodegen_0_7_0.so /sdcard/Android/data/com.beatgames.beatsaber/files/libs/libcodegen_0_7_0.so
#    adb push libs/arm64-v8a/libcustom-types.so /sdcard/Android/data/com.beatgames.beatsaber/files/libs/libcustom-types.so
    if ($?) {
        adb shell am force-stop com.beatgames.beatsaber
        adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
        if ($args[0] -eq "--log") {
        #Start-Process PowerShell -ArgumentList '-NoExit',"./logging.ps1 --file"
        Start-Process PowerShell -ArgumentList "./logging.ps1 --file"
        }
        if ($args[0] -eq "--debug") {
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -T "$timestamp" main-modloader:W QuestHook[clockmod`|v1.4.0]:* AndroidRuntime:E *:S QuestHook[UtilsLogger`|v1.0.12]:*
        }
    }
}
echo "Exiting Copy.ps1"