"C:\Users\%username%\AppData\Roaming\SideQuest\platform-tools\adb.exe" shell am force-stop com.beatgames.beatsaber

powershell ./build.ps1

"C:\Users\%username%\AppData\Roaming\SideQuest\platform-tools\adb.exe" push libs/arm64-v8a/libclockmod.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libclockmod.so

"C:\Users\%username%\AppData\Roaming\SideQuest\platform-tools\adb.exe" shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
echo Done
