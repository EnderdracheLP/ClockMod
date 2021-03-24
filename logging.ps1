Clear-Host
$timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
adb logcat -T "$timestamp" main-modloader:W QuestHook[codegen`|v0.6.3]:* QuestHook[QuestSounds`|v0.2.1]:* QuestHook[config-utils`|v0.2.2]:* QuestHook[questui`|v0.0.1]:* QuestHook[clockmod`|v1.4.0]:* AndroidRuntime:E *:S QuestHook[UtilsLogger`|v1.1.6]:E QuestHook[UtilsLogger`|v1.2.4]:E QuestHook[UtilsLogger`|v1.2.3]:E
        if ($args[0] -eq "--debug") {
            echo "Debug Logging"
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -T "$timestamp" main-modloader:W QuestHook[codegen`|v0.6.3]:* QuestHook[QuestSounds`|v0.2.1]:* QuestHook[config-utils`|v0.2.2]:* QuestHook[questui`|v0.0.1]:* QuestHook[clockmod`|v1.4.0]:* AndroidRuntime:E *:S QuestHook[UtilsLogger`|v1.1.6]:* QuestHook[UtilsLogger`|v1.2.4]:* QuestHook[UtilsLogger`|v1.2.3]:*
        }
        if ($args[0] -eq "--insane") {
            echo "INSANE Logging"
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -T "$timestamp" main-modloader:W QuestHook[codegen`|v0.6.3]:* QuestHook[QuestSounds`|v0.2.1]:* QuestHook[config-utils`|v0.2.2]:* QuestHook[questui`|v0.0.1]:* QuestHook[clockmod`|v1.4.0]:* AndroidRuntime:E *:S QuestHook[UtilsLogger`|v1.1.6]:* QuestHook[UtilsLogger`|v1.2.4]:* QuestHook[UtilsLogger`|v1.2.3]:* DEBUG:*
        }

echo "Exiting logging.ps1"