#include "main.hpp"                      // Well the main.hpp what else, this wouldn't be the same without.

#include "ClockModConfig.hpp"            // Just the Config
#include "ClockUpdater.hpp"              // For the ClockUpdater, like the stuff you see down there
#include "ClockValues.hpp"
#include "ClockFormatting.hpp"
#include <algorithm>
#include <cmath>
#include "ColorUtility.hpp"
#include "RainbowClock.hpp"              // Where the magic stuff is that makes the Clock Rainbowy (is that actually a word?)

#include "GlobalNamespace/OVRPlugin.hpp" // Where I get the Battery Percentage from as float
#include "UnityEngine/BatteryStatus.hpp"
#include "UnityEngine/GradientColorKey.hpp"
#include "UnityEngine/SystemInfo.hpp"
#include "UnityEngine/Time.hpp"

using namespace UnityEngine;
using namespace TMPro;

#if !defined(DEFINE_TYPE)
#error Custom-types macro missing, make sure you have ran: 'qpm restore' and that you have a compatible version of custom-types
#endif

DEFINE_TYPE(ClockMod, ClockUpdater);

namespace ClockMod {
    std::string ClockUpdater::getTimeFormat() {
        std::string TFormat;
        int switch_int = (int)getModConfig().TwelveToggle.GetValue() + ((int)getModConfig().SecToggle.GetValue() * 2);
        switch (switch_int) {
        case 0:
            TFormat = "%OH:%OM";          // Time in 24 Hour Format without Seconds
            break;
        case 1:
            TFormat = "%Ol:%OM %p";       // Time in 12 Hour Format without Seconds
            break;
        case 2:
            TFormat = "%OH:%OM:%OS";       // Time in 24 Hour Format with Seconds
            break;
        case 3:
            TFormat = "%Ol:%OM:%OS %p";    // Time in 12 Hour Format with Seconds
            break;
        default:
            logger().warn("Using default formatting!");
            TFormat = "%EX";          // Time in 24 Hour Format without Seconds
            break;
        }
        return TFormat;
    }

    safe_ptr<ClockUpdater*, true> ClockUpdater::instance;
    // Function for getting time. Checks config Settings for 12/24 Hour time and if Show Seconds is toggled on or off.
    std::string getTimeString(struct tm* timeinfo) {
        return FormatTime(timeinfo, ClockUpdater::getTimeFormat());
    }

    // Turns an uncapped duration in seconds into a nicely formatted string
    std::string ClockUpdater::getStopwatchString(const double totalSeconds) {
        const int wholeSeconds = static_cast<int>(totalSeconds);
        int seconds = wholeSeconds % 60;
        int minutes = wholeSeconds / 60 % 60;
        int hours = wholeSeconds / 3600 % 24;
        int days = wholeSeconds / 86400;

        bool showSeconds = getModConfig().SecToggle.GetValue();

        std::string stopwatchStr;
        if(days > 0) stopwatchStr += fmt::format("{}:", days);
        if(hours > 0 || !stopwatchStr.empty() || !showSeconds) stopwatchStr += stopwatchStr.empty() ? fmt::format("{}:", hours) : fmt::format("{:02}:", hours);
        stopwatchStr += stopwatchStr.empty() ? fmt::format("{}", minutes) : fmt::format("{:02}", minutes);
        if(showSeconds) stopwatchStr += fmt::format(":{:02}", seconds);

        return stopwatchStr;
    }

    // New Battery Percentage Formatting
    std::string getBatteryString(float level, UnityEngine::BatteryStatus status, ClockMod::ClockUpdater* instance)
    {
        if (!instance || !std::isfinite(level) || level < 0) return "Battery unavailable";
        level = std::clamp(level, 0.0f, 1.0f);
        std::string percent = fmt::format("{}%", (int)(level * 100));
        if (!instance->batteryGradient) {
            instance->batteryGradient = UnityEngine::Gradient::New_ctor();
            instance->batteryGradient->set_colorKeys(
                {
                // Red
                UnityEngine::GradientColorKey({1, 0, 0, 1}, 0.0f), 
                // Orange
                UnityEngine::GradientColorKey({1, 0.53 , 0, 1}, 0.35f),
                // Yellow
                UnityEngine::GradientColorKey({1, 0.84, 0, 1}, 0.5f), 
                // YellowGreen
                UnityEngine::GradientColorKey({0.6f, 0.8f , 0.14, 1}, 0.75f),
                // Green
                UnityEngine::GradientColorKey({0, 1, 0, 1}, 1.0f)
                }
            );
        }
        
        if (status == UnityEngine::BatteryStatus::Charging || 
            status == UnityEngine::BatteryStatus::Full ||
            (status != UnityEngine::BatteryStatus::NotCharging && status != UnityEngine::BatteryStatus::Discharging)) {
            // return fmt::format("<color=#00FF00>%s</color>", percent.c_str());
            return fmt::format("<color=#00FFFF>{}</color>", percent.c_str());
        }
        UnityEngine::Color color = instance->batteryGradient->Evaluate(level);
        std::string colorStr = ClockMod::ColorUtility::ToHtmlStringRGB(color);
        return fmt::format("<color={}>{}</color>", colorStr.c_str(), percent.c_str());
    }
    /*
    std::string RainbowClock::rainbowify(std::string input) {
        for(char& c : input) {
            std::string result
            result += fmt::format("<color=%s>%c</color>", colors[rainbowIndex].c_str(), c);
            rainbowIndex++;
            rainbowIndex %= colors.size();
        }
    return result;
    }
    */

    // /*
    // RGB Clock gives more FPS, Gamers will love that.
    std::string RainbowClock::rainbowify(std::string input)
    {
        std::string result;

        for (auto c : input)
        {
            result += fmt::format("<color={}>{}</color>", colors[rainbowIndex].c_str(), c);
            rainbowIndex++;
            rainbowIndex %= colors.size();
        }
	    int addValue = (colors.size() - 1) - input.length();
        if (input.length() < 10) {
            rainbowIndex += addValue;
            if (rainbowIndex > (colors.size() - 1)) rainbowIndex -= colors.size();
        }
        return result;
    }
    // */

    // Function for updating the Clock Position, temp
    void SetClockPos(UnityEngine::Transform* ClockParent, TMPro::TextMeshProUGUI* text, UnityEngine::Vector3 Pos, UnityEngine::Vector3 Angle, UnityEngine::Vector3 Scale) {
        ClockParent->set_position(Pos);
        text->get_transform()->set_localEulerAngles(Angle);
        text->get_transform()->set_localScale(Scale);
    }

    #define NUM_SECONDS 0.25

    void ClockUpdater::Awake() {
        instance = this;
    }

    void ClockUpdater::Start() {
        //JNIEnv* javaEnv = Modloader::getJni();

        //jclass clazz = javaEnv->FindClass("java/text/SimpleDateFormat");
        //jmethodID dateMethod = javaEnv->GetMethodID(clazz, "SimpleDateFormat");


        text = get_gameObject()->GetComponent<TextMeshProUGUI*>();
        clockParent = get_transform()->GetParent();
        stopwatch1Seconds = getModConfig().Stopwatch1Seconds.GetValue();
        stopwatch2Seconds = getModConfig().Stopwatch2Seconds.GetValue();
        sessionTimeSeconds = Time::get_realtimeSinceStartupAsDouble();
        lastSessionTimeSeconds = sessionTimeSeconds;
        nextStopwatchSaveTime = sessionTimeSeconds + 5.0;
        rawtime = std::time(nullptr);
        if (text) {
            text->set_color(getModConfig().ClockColor.GetValue());         // Sets the clocks color, will only color in the "-" if rainbowifier is enabled.
            auto fontSize = getModConfig().FontSize.GetValue();
            text->set_fontSize(std::isfinite(fontSize) ? std::clamp(fontSize, 1.0f, 5.0f) : 3.5f);
            // text->set_alignment(TMPro::TextAlignmentOptions::Center);
        }
        logger().info("Clock updater started");
    }

    void ClockUpdater::OnDestroy() {
        if (instance.unchecked_ptr() == this) {
            instance.clear();
            ReleaseClockObjects();
        }
        text = nullptr;
        clockParent = nullptr;
        batteryGradient = nullptr;
    }

    // Updates the Clock.
    void ClockUpdater::Update() {
        if (!text || !clockParent) return;

        // Temp Code for updating Position.
        if (Config.IsInSong == false && Config.InMPLobby == false) {
            // TODO: Get Position Offset working. Trying to set the Position offset here, messes with the 360/90 Map stuff. 
            //if (Config.IsInSong == false) {
                // Checks if the clock should be at the Top or Bottom
            if (getModConfig().ClockPosition.GetValue()) {
                // If set to be at the Bottom do this.
                auto Pos = ClockPos.MenuPosBottom;
                auto Angle = UnityEngine::Vector3(60, 0, 0);
                auto Scale = UnityEngine::Vector3(0.6, 0.6, 0.6);
                SetClockPos(clockParent, text, Pos, Angle, Scale);
            }
            else {
                // Otherwise it will do this.
                // auto Pos = UnityEngine::Vector3(0, -1.7, 5.0);
                auto Pos = ClockPos.MenuPosTop;
                auto Angle = UnityEngine::Vector3(-10, 0, 0);
                auto Scale = UnityEngine::Vector3(1, 1, 1);
                SetClockPos(clockParent, text, Pos, Angle, Scale);
            }
        }
        else { // If in MP Lobby or a Song, unset all this.
            //clockParent->set_position(UnityEngine::Vector3(0, 0, 0));
            text->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0, 0, 0));
            text->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
        }

        sessionTimeSeconds = Time::get_realtimeSinceStartupAsDouble();
        if(!getModConfig().Stopwatch1Paused.GetValue()) stopwatch1Seconds += sessionTimeSeconds - lastSessionTimeSeconds;
        if(!getModConfig().Stopwatch2Paused.GetValue()) stopwatch2Seconds += sessionTimeSeconds - lastSessionTimeSeconds;
        lastSessionTimeSeconds = sessionTimeSeconds;

        if (sessionTimeSeconds < nextTextUpdateTime)
        {
            return;
        }
        nextTextUpdateTime = sessionTimeSeconds + NUM_SECONDS;

        if (sessionTimeSeconds >= nextStopwatchSaveTime && !Config.IsInSong) { // Avoid dropping frames during gameplay
            auto& config = getModConfig();
            const int seconds1 = static_cast<int>(stopwatch1Seconds);
            const int seconds2 = static_cast<int>(stopwatch2Seconds);
            if (config.Stopwatch1Seconds.GetValue() != seconds1 || config.Stopwatch2Seconds.GetValue() != seconds2) {
                config.Stopwatch1Seconds.SetValue(seconds1, false);
                config.Stopwatch2Seconds.SetValue(seconds2, false);
                config.Save();
            }
            nextStopwatchSaveTime = sessionTimeSeconds + 5.0;
        }

        if (!Config.IsInSong || getModConfig().InSong.GetValue() || !Config.noTextAndHUD) {
            time(&rawtime);
            timeinfo = localtime_r(&rawtime, &localTime);
            //auto clockParent = get_transform();

            if (timeinfo && timeinfo->tm_mon == 0 && timeinfo->tm_mday == 1 && timeinfo->tm_hour == 0 && timeinfo->tm_min == 0 && timeinfo->tm_sec == 0 && messageShowing == 0)
            {
                this->ShowMessage("Happy New Year!", 10);
            }

            ClockPos.ap1 = (timeinfo && timeinfo->tm_mon == 3 && timeinfo->tm_mday == 1);

            std::string clockresult;
            if(!_message.empty()) clockresult = _message;
            else if(getModConfig().ClockType.GetValue() == static_cast<int>(ClockTypes::SessionTime)) clockresult = getStopwatchString(sessionTimeSeconds);
            else if(getModConfig().ClockType.GetValue() == static_cast<int>(ClockTypes::Stopwatch1)) clockresult = getStopwatchString(stopwatch1Seconds);
            else if(getModConfig().ClockType.GetValue() == static_cast<int>(ClockTypes::Stopwatch2)) clockresult = getStopwatchString(stopwatch2Seconds);
            else clockresult = getTimeString((struct tm*)timeinfo);

            // Checks, if the clock is set to rainbowify
            if (getModConfig().RainbowClock.GetValue()) {
                clockresult = RainbowClock::rainbowify(clockresult);
                //text->set_color(UnityEngine::Color::HSVToRGB(std::fmod(UnityEngine::Time::get_time() * 0.35f, 1), 1, 1));
            }


                // Checks, the below condition and if it retunrs true, gets the current Battery Percentage Level and adds it to the clockresult variable.
                if (getModConfig().BattToggle.GetValue() && _message.empty()) {
                    // Gets the Battery Percentage as float 1.00, multiplies it by 100, and then uses the getBatteryString function defined above to format the percentage.
                    // auto battery = getBatteryString((int)(GlobalNamespace::OVRPlugin::OVRP_1_1_0::ovrp_GetSystemBatteryLevel() * 100));
                    if (sessionTimeSeconds >= nextBatteryUpdateTime) {
                        cachedBatteryText = getBatteryString(UnityEngine::SystemInfo::get_batteryLevel(), UnityEngine::SystemInfo::get_batteryStatus(), this);
                        nextBatteryUpdateTime = sessionTimeSeconds + 5.0;
                    }

                    // static float testPercentage = 1.0f;
                    // auto battery = getBatteryString(testPercentage, this);
                    // if (testPercentage > 0.0f) testPercentage += -0.01f;
                    // else testPercentage = 1.0f;

                    clockresult += " - ";                // Adds  -  to it with spaces, before and after the - .
                    clockresult += cachedBatteryText;
                }

                if (!_message.empty() && messageShowing > 0) messageShowing--;
                else _message.clear();


                // This is where the Text and Clock Position is set.
                text->set_text(StringW(clockresult.c_str()));        // This sets the Text
            //}
        }
    }

    const time_t ClockUpdater::getRawTime() const {
        return rawtime;
    }

    struct tm* ClockUpdater::getTimeInfo() {
        rawtime = std::time(nullptr);
        return localtime_r(&rawtime, &localTime);
    }

    struct tm* ClockUpdater::getTimeInfoUTC() {
        return gmtime_r(&rawtime, &utcTime);
    }

    const double ClockUpdater::getSessionTimeSeconds() const {
        return sessionTimeSeconds;
    }

    const double ClockUpdater::getStopwatch1Seconds() const {
        return stopwatch1Seconds;
    }

    const double ClockUpdater::getStopwatch2Seconds() const {
        return stopwatch2Seconds;
    }

    void ClockUpdater::resetStopwatch1() {
        stopwatch1Seconds = 0;
        getModConfig().Stopwatch1Seconds.SetValue(0);
    }

    void ClockUpdater::resetStopwatch2() {
        stopwatch2Seconds = 0;
        getModConfig().Stopwatch2Seconds.SetValue(0);
    }

    TMPro::TextMeshProUGUI* ClockUpdater::getTextMesh() {
        return text ? text.ptr() : nullptr;
    }

    void ClockUpdater::SetColor(UnityEngine::Color color) {
        if (text){
            text->set_color(color);
        }
    }

    ClockUpdater* ClockUpdater::getInstance() {
        return instance ? instance.ptr() : nullptr;
    }

    void ClockUpdater::ShowMessage(std::string message, int duration) {
        this->messageShowing = duration;
        this->_message = message;
    }
}
