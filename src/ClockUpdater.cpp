#include "main.hpp"
#include "ClockViewController.hpp"

#include "ClockUpdater.hpp"

#include "ClockModConfig.hpp"

#include "UnityEngine/GameObject.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include <ctime>
#include "GlobalNamespace/OVRPlugin_OVRP_1_1_0.hpp"
#include "RainbowClock.hpp" // Needed to make the Clock Rainbowy (is that actually a word?)
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

// RainbowClock, makes the Clock beautiful, yeah really it does.
 std::string RainbowClock::rainbowify(std::string input)
{
    std::string result = "";

    for (auto c : input)
    {
        result += string_format("<color=%s>%c</color>", colors[rainbowIndex].c_str(), c);
        rainbowIndex++;
        rainbowIndex %= colors.size();
    }

    return result;
}

// Updates the Clock.
 int wait = 0; // Sometimes you just need to take a deep breath and slow the fuck down, I'm looking at you ClockUpdater, also probably the dumbest way to slow it down.

 void ClockMod::ClockUpdater::Update() {
     time_t rawtime;
     char timechar[20];
     struct tm* timeinfo;
     time(&rawtime);
     timeinfo = localtime(&rawtime);

     if (wait == 10) {
         wait = 0;

         //     std::string timestr;  // Added this because I have to declare it

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
                 strftime(timechar, 20, "%H:%M:%S", timeinfo);
             }
             else {
                 strftime(timechar, 20, "%H:%M", timeinfo);
             }
         }
         else {      // If set to show 24 Hour Format
             if (getModConfig().SecToggle.GetValue()) {    //Check if seconds should be shown
                 strftime(timechar, 20, "%l:%M:%S %p", timeinfo);
             }
             else {
                 strftime(timechar, 20, "%l:%M %p", timeinfo);
             };
         }
         std::string timestr = timechar;
         if (getModConfig().RainbowClock.GetValue()) {
             timestr = RainbowClock::rainbowify((std::string)timestr);
         };
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
         if (getModConfig().RainbowClock.GetValue() == false) {
             text->set_color(getModConfig().ClockColor.GetValue());
         };
         text->set_fontSize(getModConfig().FontSize.GetValue());
         //       text->get_transform()->set_position(UnityEngine::Vector3(0 + getModConfig().ClockXOffset.GetValue(), 0.5 + getModConfig().ClockYOffset.GetValue(), 3.85 + getModConfig().ClockZOffset.GetValue()));
         if (Config.InMPLobby == false) {
             get_transform()->GetParent()->set_position(UnityEngine::Vector3(getModConfig().ClockX.GetValue() + getModConfig().ClockXOffset.GetValue(), getModConfig().ClockY.GetValue() + getModConfig().ClockYOffset.GetValue(), getModConfig().ClockZ.GetValue() + getModConfig().ClockZOffset.GetValue()));
         }
     } else { wait = wait + 1; }
}