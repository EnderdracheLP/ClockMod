#include "main.hpp"
#include "ClockViewController.hpp"

#include "ClockUpdater.hpp"
#include "UnityEngine/GameObject.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include <ctime>
#include "GlobalNamespace/OVRPlugin_OVRP_1_1_0.hpp"
using namespace UnityEngine;
using namespace TMPro;

DEFINE_CLASS(ClockMod::ClockUpdater);

// Battery Percentage Formatting, thanks to RedBrumbler on #quest-mod-dev

std::string getBatteryString(int level)
{
    std::string percent = string_format("%d%%", level);
    if (level < 5) return string_format("<color=#ff0000>%s</color>", percent.c_str());
    else if (level > 60) return string_format("<color=#00ff00>%s</color>", percent.c_str());
    else return string_format("<color=#ff8800>%s</color>", percent.c_str());
}

// Updates the Clock.

void ClockMod::ClockUpdater::Update() {
    time_t rawtime;
    char timestr[20];
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // TODO: Change the clock code to:
//    std::string getTimeString(timeInfo);
//    std::string result = getTimeString();
//
//    if (batterytoggle)
//    {
//        result += " - ";
//        result += getBatteryString(batterylvl);
//    }
//
//    text->set_text(il2cpp_utils::createcsstr(result));

// Checks config Settings for 12/24 Hour time and if Show Seconds is toggled on or off.

    if (getConfig().config["12Toggle"].GetBool() == false) {
        if (getConfig().config["SecToggle"].GetBool() == true) {    //Check if seconds should be shown
            strftime(timestr, 20, "%H:%M:%S", timeinfo);
        }
        else {
            strftime(timestr, 20, "%H:%M", timeinfo);
        }
    }
    else {      // If set to show 24 Hour Format
        if (getConfig().config["SecToggle"].GetBool() == true) {    //Check if seconds should be shown
            strftime(timestr, 20, "%l:%M:%S %p", timeinfo);
        }
        else {
            strftime(timestr, 20, "%l:%M %p", timeinfo);
        };
    }

    // Sets the fontsize 
    //TODO: somehow manage to have that code only running while the player is in the config or only updating the values when the player changes them in the config
//    float fontsize = getConfig().config["FontSize"].GetFloat();
       auto text = get_gameObject()->GetComponent<TextMeshProUGUI*>();
//       text->set_fontSize(fontsize);

        // Get current Battery Level
       if (getConfig().config["BattToggle"].GetBool() == true) {
           float batterylvl = GlobalNamespace::OVRPlugin::OVRP_1_1_0::ovrp_GetSystemBatteryLevel();
           batterylvl = batterylvl * 100;
           auto batterylevel = getBatteryString((int)batterylvl);
           std::string tandb = timestr;
           tandb += " - ";
           tandb += batterylevel;

           text->set_text(il2cpp_utils::createcsstr(tandb));
       }
       else {
           text->set_text(il2cpp_utils::createcsstr(timestr));
       }
}