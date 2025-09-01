#include "main.hpp"                      // Well the main.hpp what else, this wouldn't be the same without.

#include "ClockModConfig.hpp"            // Just the Config
#include "ClockUpdater.hpp"              // For the ClockUpdater, like the stuff you see down there
#include "ClockValues.hpp"
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

    ClockUpdater* ClockUpdater::instance;
    // Function for getting time. Checks config Settings for 12/24 Hour time and if Show Seconds is toggled on or off.
    std::string getTimeString(struct tm* timeinfo) {
        char time[20];
        strftime(time, sizeof(time), ClockUpdater::getTimeFormat().c_str(), timeinfo);
        return time;
    }

    // Turns an uncapped duration in seconds into a nicely formatted string
    std::string ClockUpdater::getTimerString(const double totalSeconds) {
        int seconds = (int)totalSeconds % 60;
        int minutes = (int)(totalSeconds / 60) % 60;
        int hours = (int)(totalSeconds / 60 / 60) % 24;
        int days = (int)(totalSeconds / 60 / 60 / 24);

        bool showSeconds = getModConfig().SecToggle.GetValue();

        std::string timerStr;
        if(days > 0) timerStr += fmt::format("{}:", days);
        if(hours > 0 || !timerStr.empty() || !showSeconds) timerStr += fmt::format("{:02}:", hours);
        timerStr += fmt::format("{:02}", minutes);
        if(showSeconds) timerStr += fmt::format(":{:02}", seconds);

        return timerStr;
    }

    // New Battery Percentage Formatting
    std::string getBatteryString(float level, UnityEngine::BatteryStatus status, ClockMod::ClockUpdater* instance)
    {
        std::string percent = fmt::format("{}%", (int)(level * 100));
        if (!(instance && instance->batteryGradient)) {
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

    void ClockUpdater::Start() {
        //JNIEnv* javaEnv = Modloader::getJni();

        //jclass clazz = javaEnv->FindClass("java/text/SimpleDateFormat");
        //jmethodID dateMethod = javaEnv->GetMethodID(clazz, "SimpleDateFormat");


        text = get_gameObject()->GetComponent<TextMeshProUGUI*>();
        clockParent = get_transform()->GetParent();
        getModConfig().ClockColor.AddChangeEvent([this](UnityEngine::Color color) {
            if (text)
                text->set_color(color);
            }
        );
        getModConfig().FontSize.AddChangeEvent([this](float value) {
            if (text)
                text->set_fontSize(value);
            }
        );
        if (text) {
            text->set_color(getModConfig().ClockColor.GetValue());         // Sets the clocks color, will only color in the "-" if rainbowifier is enabled.
            text->set_fontSize(getModConfig().FontSize.GetValue());
            // text->set_alignment(TMPro::TextAlignmentOptions::Center);
        }
        instance = this;
    }

    // Updates the Clock.
    void ClockUpdater::Update() {


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

        this_time = clock();

        time_counter += (double)(this_time - last_time);

        last_time = this_time;

        sessionTimeSeconds = Time::get_realtimeSinceStartup();

        if (!(time_counter > (double)(NUM_SECONDS * CLOCKS_PER_SEC)))
        {
            return;
        }
        time_counter = 0;

        if (getModConfig().InSong.GetValue() || !Config.noTextAndHUD) {
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            //auto clockParent = get_transform();

            if (timeinfo && timeinfo->tm_mon == 0 && timeinfo->tm_mday == 1 && timeinfo->tm_hour == 0 && timeinfo->tm_min == 0 && timeinfo->tm_sec == 0 && messageShowing == 0)
            {
                this->ShowMessage("Happy New Year!", 10);
            }

            ClockPos.ap1 = (timeinfo && timeinfo->tm_mon == 3 && timeinfo->tm_mday == 1);

            std::string clockresult;
            if(!_message.empty()) clockresult = _message;
            else if(getModConfig().ClockType.GetValue() == static_cast<int>(ClockTypes::SessionTime)) clockresult = getTimerString(sessionTimeSeconds);
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
                    auto battery = getBatteryString(UnityEngine::SystemInfo::get_batteryLevel(), UnityEngine::SystemInfo::get_batteryStatus(), this);

                    // static float testPercentage = 1.0f;
                    // auto battery = getBatteryString(testPercentage, this);
                    // if (testPercentage > 0.0f) testPercentage += -0.01f;
                    // else testPercentage = 1.0f;

                    clockresult += " - ";                // Adds  -  to it with spaces, before and after the - .
                    clockresult += battery;         // Here is where the Battery gets added to the tandb string.
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
        time_t time = rawtime;
        return localtime(&time);
    }

    struct tm* ClockUpdater::getTimeInfoUTC() {
        time_t time = rawtime;
        return gmtime(&time);
    }

    const double ClockUpdater::getSessionTimeSeconds() const {
        return sessionTimeSeconds;
    }

    TMPro::TextMeshProUGUI* ClockUpdater::getTextMesh() {
        return text;
    }

    void ClockUpdater::SetColor(UnityEngine::Color color) {
        if (text){
            text->set_color(color);
        }
    }

    ClockUpdater* ClockUpdater::getInstance() {
        return instance;
    }

    void ClockUpdater::ShowMessage(std::string message, int duration) {
        this->messageShowing = duration;
        this->_message = message;
    }
}