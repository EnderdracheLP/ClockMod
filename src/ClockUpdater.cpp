#include "main.hpp"                                 // Well the main.hpp what else, this wouldn't be the same without.
#include "ClockViewController.hpp"                  // Part of the config. Do I actually need this in here? idk.

#include "ClockUpdater.hpp"                         // For the ClockUpdater, like the stuff you see down there

#include "ClockModConfig.hpp"                       // Just the Config

#include "UnityEngine/GameObject.hpp"               // For GameObject Stuff
#include "TMPro/TextMeshProUGUI.hpp"                // For Text Stuff.
#include <ctime>                                    // For Time stuff, idk if I actually need it.
#include "GlobalNamespace/OVRPlugin_OVRP_1_1_0.hpp" // Where I get the Battery Percentage from as float
#include "RainbowClock.hpp"                         // Where the magic stuff is that makes the Clock Rainbowy (is that actually a word?)
using namespace UnityEngine;
using namespace TMPro;

DEFINE_CLASS(ClockMod::ClockUpdater);               // Here the class is defined, look inside ClockUpdater.hpp for more stuff.

// /*         
// Function for getting time. Checks config Settings for 12/24 Hour time and if Show Seconds is toggled on or off.
std::string getTimeString(struct tm* timeinfo) {
    char time[20];

    if (!getModConfig().TwelveToggle.GetValue()) {
        if (getModConfig().SecToggle.GetValue()) {    //Check if seconds should be shown
            strftime(time, 20, "%H:%M:%S", timeinfo);
        }
        else {
            strftime(time, 20, "%H:%M", timeinfo);
        }
    }
    else {      // If set to show 12 Hour Format
        if (getModConfig().SecToggle.GetValue()) {    //Check if seconds should be shown
            strftime(time, 20, "%l:%M:%S %p", timeinfo);
        }
        else {
            strftime(time, 20, "%l:%M %p", timeinfo);
        }
    }
    return time;
}
// */

// /*
// Battery Percentage Formatting, thanks to RedBrumbler on #quest-mod-dev
std::string getBatteryString(int level)
{
    std::string percent = string_format("%d%%", level);
    if (level < 10) return string_format("<color=#ff0000>%s</color>", percent.c_str());
    else if (level > 49) return string_format("<color=#00ff00>%s</color>", percent.c_str());
    else return string_format("<color=#ff8800>%s</color>", percent.c_str());
}
// */

// RGB Clock gives more FPS, Gamers will love that.
std::string RainbowClock::rainbowify(std::string input)
{
    std::string result;

    for (auto c : input)
    {
        result += string_format("<color=%s>%c</color>", colors[rainbowIndex].c_str(), c);
//        result += string_format("<color=%s%>%c%</color>", colors[rainbowIndex].c_str(), c);
        rainbowIndex++;
        rainbowIndex %= colors.size();
    }

    return result;
}

// Updates the Clock.
 int wait = 18; // Sometimes you just need to take a deep breath and slow the fuck down, I'm looking at you ClockUpdater, also probably the dumbest way to slow it down.

 void ClockMod::ClockUpdater::Update() {
     time_t rawtime;
     //char timechar[20];
     struct tm* timeinfo;
     time(&rawtime);
     timeinfo = localtime(&rawtime);

     // Yes all the "wait" stuff here is for slowing it down
     if (wait == 18) {
              // TODO: Change the clock code to something like:
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

          // Gets the time using the function at the top.
         auto clockresult = getTimeString((struct tm*)timeinfo);
         wait = 0;

         // TODO: Fix this rainbow stuff
         // Checks, if the clock is set to rainbowify
         if (getModConfig().RainbowClock.GetValue()) {
             clockresult = RainbowClock::rainbowify(clockresult);
         };

         // Checks, the below condition and if it retunrs true, gets the current Battery Percentage Level and adds it to the clockresult variable.
         if (getModConfig().BattToggle.GetValue()) {
             // Gets the Battery Percentage as float 1.00, multiplies it by 100, and then uses the getBatteryString function defined above to format the percentage.
             auto battery = getBatteryString((int)(GlobalNamespace::OVRPlugin::OVRP_1_1_0::ovrp_GetSystemBatteryLevel() * 100)); 
             clockresult += " - ";                // Adds  -  to it with spaces, before and after the - .
             clockresult += battery;         // Here is where the Battery gets added to the tandb string.
         }

         // This is where the Text and Clock Position is set.
         auto text = get_gameObject()->GetComponent<TextMeshProUGUI*>();
         text->set_text(il2cpp_utils::createcsstr(clockresult));        // This sets the Text
         text->set_color(getModConfig().ClockColor.GetValue());         // Sets the clocks color, will only color in the "-" if rainbowifier is enabled.
         text->set_fontSize(getModConfig().FontSize.GetValue());
                //text->get_transform()->set_position(UnityEngine::Vector3(0 + getModConfig().ClockXOffset.GetValue(), 0.5 + getModConfig().ClockYOffset.GetValue(), 3.85 + getModConfig().ClockZOffset.GetValue()));
         if (Config.InMPLobby == false) {
             // TODO: Get Position Offset working. Trying to set the Position offset here, messes with the 360/90 Map stuff. 
             //get_transform()->set_localPosition(UnityEngine::Vector3(getModConfig().ClockXOffset.GetValue(), getModConfig().ClockYOffset.GetValue(), getModConfig().ClockZOffset.GetValue()));
             if (Config.IsInSong == false) {
                 // Checks if the clock should be at the Top or Bottom
                 if (getModConfig().ClockPosition.GetValue()) {
                     // If set to be at the Bottom do this.
                     get_transform()->GetParent()->set_position(UnityEngine::Vector3(0, -1.26, 0));
                     text->get_transform()->set_localEulerAngles(UnityEngine::Vector3(60, 0, 0));
                     text->get_transform()->set_localScale(UnityEngine::Vector3(0.6, 0.6, 0.6));
                 }
                 else {
                     // Otherwise it will do this.
                     get_transform()->GetParent()->set_position(UnityEngine::Vector3(0, -1.7, 4.6));
                     text->get_transform()->set_localEulerAngles(UnityEngine::Vector3(-10, 0, 0));
                     text->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
                 }
             //                     text->get_transform()->set_position(UnityEngine::Vector3(0+getModConfig().ClockXOffset.GetValue(), Config.ClockY+getModConfig().ClockYOffset.GetValue(), Config.ClockZ+getModConfig().ClockZOffset.GetValue()));
             //                     get_transform()->GetParent()->set_position(UnityEngine::Vector3(0 + getModConfig().ClockXOffset.GetValue(), Config.ClockY + getModConfig().ClockYOffset.GetValue(), Config.ClockZ + getModConfig().ClockZOffset.GetValue()));
             //                     get_transform()->set_position(UnityEngine::Vector3(getModConfig().ClockXOffset.GetValue(), getModConfig().ClockYOffset.GetValue(), getModConfig().ClockZOffset.GetValue()));
             //                      Gets the parent objects X, Y, and Z coordinates so, they can be used when setting the offset, there are surely better ways of doing it.
             //                     auto ClockX = get_transform()->GetParent()->get_position().x;
             //                     auto ClockY = get_transform()->GetParent()->get_position().y;
             //                     auto ClockZ = get_transform()->GetParent()->get_position().z;
             //                     text->get_transform()->set_position(UnityEngine::Vector3(ClockX+getModConfig().ClockXOffset.GetValue(), ClockY+getModConfig().ClockYOffset.GetValue(), ClockZ+getModConfig().ClockZOffset.GetValue()));
             //                     text->get_transform()->LookAt(get_transform()->GetParent());

             //                     text->get_transform()->GetParent()->GetParent()->set_localEulerAngles(UnityEngine::Vector3(getModConfig().ClockYOffset.GetValue() * 10-5, 0, 0));
             }  
             // TODO: Reduce the amount of code here, like make stuff a variable or a function
             //            When not in a Map with RotationEvents (360/90).
             else if (Config.InRotationMap == false) {
                 if (getModConfig().ClockPosition.GetValue()) {
                     // If set to be at the Bottom do this.
                     get_transform()->GetParent()->set_position(UnityEngine::Vector3(0, -4.45, 2));
                     text->get_transform()->set_localEulerAngles(UnityEngine::Vector3(45, 0, 0));
                     text->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
                     //logger().debug("In Normal Map set to Bottom");
                 }
                 else {
                     // Otherwise it will do this.
                     get_transform()->GetParent()->set_position(UnityEngine::Vector3(0, -1.7, 5.6));
                     text->get_transform()->set_localEulerAngles(UnityEngine::Vector3(-10, 0, 0));
                     text->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
                     //logger().debug("In Normal Map set to Top");
                 }
             }
         }
         else { // If in MP Lobby, unset all this.
             text->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0, 0, 0));
             text->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
         }
     } else { wait++; }
 }


//if (Config.InRotationMap == false) {
//    if (getModConfig().ClockPosition.GetValue()) {
//        text->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0, 0, 0));
//        text->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
//    }
//    else { // If in a 360 Map, unset all of this.
//        text->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0, 0, 0));
//        text->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
//    }
//}