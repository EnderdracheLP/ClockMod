#include "main.hpp"                                 // Well the main.hpp what else, this wouldn't be the same without.

#include "ClockUpdater.hpp"                         // For the ClockUpdater, like the stuff you see down there

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"               // For GameObject Stuff
#include "TMPro/TextMeshProUGUI.hpp"                // For Text Stuff.
#include <ctime>                                    // For Time stuff, idk if I actually need it.
#include "GlobalNamespace/OVRPlugin_OVRP_1_1_0.hpp" // Where I get the Battery Percentage from as float
//#include "RainbowClock.hpp"                         // Where the magic stuff is that makes the Clock Rainbowy (is that actually a word?) [NOT IMPLEMENTED]
using namespace UnityEngine;
using namespace TMPro;

DEFINE_CLASS(ClockMod::ClockUpdater);

std::string getTimeString(struct tm* timeinfo) {
    char time[20];
    const char* TFormat;

    // If set to show 24 Hour Format
    if (getConfig().config["12Toggle"].GetBool() == false) {
        if (getConfig().config["SecToggle"].GetBool() == true) {    //Check if seconds should be shown
            TFormat = "%H:%M:%S";       // Time in 24 Hour Format with Seconds
            strftime(time, sizeof(time), TFormat, timeinfo);
            return time;
        }
        else {
            TFormat = "%H:%M";          // Time in 24 Hour Format without Seconds
            strftime(time, sizeof(time), TFormat, timeinfo);
            return time;
        }
    }
    else {      // If set to show 12 Hour Format
        if (getConfig().config["SecToggle"].GetBool() == true) {    //Check if seconds should be shown
            TFormat = "%l:%M:%S %p";    // Time in 24 Hour Format with Seconds
            strftime(time, sizeof(time), TFormat, timeinfo);
            return time;
        }
        else {
            TFormat = "%l:%M %p";       // Time in 24 Hour Format without Seconds
            strftime(time, sizeof(time), TFormat, timeinfo);
            return time;
        }
    }
    strftime(time, sizeof(time), "%H:%M", timeinfo);
    return time;
}

// Battery Percentage Formatting, thanks to RedBrumbler on #quest-mod-dev
std::string getBatteryString(int level)
{
    std::string percent = string_format("%d%%", level);
    if (level < 20) return string_format("<color=#FF0000>%s</color>", percent.c_str()); 		// Red          (if lower than 20 %)
    else if (level < 35) return string_format("<color=#FF8800>%s</color>", percent.c_str());	// Orange       (if lower than 35 %)
    else if (level < 50) return string_format("<color=#FFD700>%s</color>", percent.c_str());	// Yellow       (if lower than 50 %)
    else if (level >= 75) return string_format("<color=#00FF00>%s</color>", percent.c_str());	// Green        (if higher or equal to 75 %)
    else return string_format("<color=#9ACD32>%s</color>", percent.c_str());					// YellowGreen  (if none of the above)
}

 /*
// RGB Clock gives more FPS, Gamers will love that.
std::string RainbowClock::rainbowify(std::string input)
{
    std::string result;

    for (auto c : input)
    {
        result += string_format("<color=%s>%c</color>", colors[rainbowIndex].c_str(), c);
                result += string_format("<color=%s%>%c%</color>", colors[rainbowIndex].c_str(), c);
        rainbowIndex++;
        rainbowIndex %= colors.size();
    }
    return result;
}
 */

// Function for updating the Clock Position, temp
void SetClockPos(UnityEngine::Transform* ClockParent, TMPro::TextMeshProUGUI* text, UnityEngine::Vector3 Pos, UnityEngine::Vector3 Angle, UnityEngine::Vector3 Scale) {
    ClockParent->set_position(Pos);
    text->get_transform()->set_localEulerAngles(Angle);
    text->get_transform()->set_localScale(Scale);
}

// Updates the Clock.
int wait = 18; // Sometimes you just need to take a deep breath and slow the fuck down, I'm looking at you ClockUpdater, also probably the dumbest way to slow it down.

void ClockMod::ClockUpdater::Update() {
    if (getModConfig().InSong.GetValue() || !Config.noTextAndHUD) {
        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        auto text = get_gameObject()->GetComponent<TextMeshProUGUI*>();
        auto clockParent = get_transform()->GetParent();
        //auto clockParent = get_transform();

        // Yes all the "wait" stuff here is for slowing it down
        if (wait == 18) {
            // Gets the time using the function at the top.
            auto clockresult = getTimeString((struct tm*)timeinfo);
            wait = 0;

             /*
            // TODO: Fix this rainbow stuff
            // Checks, if the clock is set to rainbowify (Not implemented in LegacyBranch)
            if (getModConfig().RainbowClock.GetValue()) {
                clockresult = RainbowClock::rainbowify(clockresult);
            };
             */
            // Checks, the below condition and if it retunrs true, gets the current Battery Percentage Level and adds it to the clockresult variable.
            if (getConfig().config["BattToggle"].GetBool() == true) {
                // Gets the Battery Percentage as float 1.00, multiplies it by 100, and then uses the getBatteryString function defined above to format the percentage.
                auto battery = getBatteryString((int)(GlobalNamespace::OVRPlugin::OVRP_1_1_0::ovrp_GetSystemBatteryLevel() * 100));
                clockresult += " - ";                // Adds  -  to it with spaces, before and after the - .
                clockresult += battery;         // Here is where the Battery gets added to the tandb string.
            }

            // This is where the Text and Clock Position is set.
            text->set_text(il2cpp_utils::newcsstr(clockresult));        // This sets the Text
            //text->set_color(getModConfig().ClockColor.GetValue());         // Sets the clocks color, will only color in the "-" if rainbowifier is enabled.
            text->set_fontSize(getConfig().config["FontSize"].GetFloat());
        }
        else { wait++; }

        // Temp Code for updating Position.
        if (Config.IsInSong == false && Config.InMPLobby == false) {
            // TODO: Get Position Offset working. Trying to set the Position offset here, messes with the 360/90 Map stuff. 
            //if (Config.IsInSong == false) {
                // Checks if the clock should be at the Top or Bottom
            if (getModConfig().ClockPosition.GetValue()) {
                // If set to be at the Bottom do this.
                auto Pos = UnityEngine::Vector3(0, -1.26, 0);
                auto Angle = UnityEngine::Vector3(60, 0, 0);
                auto Scale = UnityEngine::Vector3(0.6, 0.6, 0.6);
                SetClockPos(clockParent, text, Pos, Angle, Scale);
            }
            else {
                // Otherwise it will do this.
                auto Pos = UnityEngine::Vector3(0, -1.7, 4.6);
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
    }
}