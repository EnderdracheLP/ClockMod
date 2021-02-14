#include "main.hpp"
#include "ClockViewController.hpp"

#include "ClockUpdater.hpp"

#include "ClockModConfig.hpp"

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

    if (!getModConfig().TwelveToggle.GetValue()) {
        if (getModConfig().SecToggle.GetValue()) {    //Check if seconds should be shown
            strftime(timestr, 20, "%H:%M:%S", timeinfo);
        }
        else {
            strftime(timestr, 20, "%H:%M", timeinfo);
        }
    }
    else {      // If set to show 24 Hour Format
        if (getModConfig().SecToggle.GetValue()) {    //Check if seconds should be shown
            strftime(timestr, 20, "%l:%M:%S %p", timeinfo);
        }
        else {
            strftime(timestr, 20, "%l:%M %p", timeinfo);
        };
    }

       auto text = get_gameObject()->GetComponent<TextMeshProUGUI*>();

        // Get current Battery Level
       if (getModConfig().BattToggle.GetValue()) {
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

       text->set_color(getModConfig().ClockColor.GetValue());
       text->set_fontSize(getModConfig().FontSize.GetValue());
//       text->get_transform()->set_position(UnityEngine::Vector3(0 + getModConfig().ClockXOffset.GetValue(), 0.5 + getModConfig().ClockYOffset.GetValue(), 3.85 + getModConfig().ClockZOffset.GetValue()));
       if (Config.InMPLobby == false) {
           get_transform()->GetParent()->set_position(UnityEngine::Vector3(getModConfig().ClockX.GetValue() + getModConfig().ClockXOffset.GetValue(), getModConfig().ClockY.GetValue() + getModConfig().ClockYOffset.GetValue(), getModConfig().ClockZ.GetValue() + getModConfig().ClockZOffset.GetValue())); }
}