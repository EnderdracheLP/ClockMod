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

// Updates the Clock.

void ClockMod::ClockUpdater::Update() {
    time_t rawtime;
    char timestr[20];
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

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
    float fontsize = getConfig().config["FontSize"].GetFloat();
       auto text = get_gameObject()->GetComponent<TextMeshProUGUI*>();
       text->set_fontSize(fontsize);
       // Sets position
//       text->get_transform()->set_position(UnityEngine::Vector3(0, 1, 2.6));
        // Get current Battery Level
       if (getConfig().config["BattToggle"].GetBool() == true) {
           float batterylvl = GlobalNamespace::OVRPlugin::OVRP_1_1_0::ovrp_GetSystemBatteryLevel();
           batterylvl = batterylvl * 100;
           std::string tandb = timestr;
           tandb += " - ";
           std::string batterylevel = std::to_string(batterylvl);
           batterylevel.erase(batterylevel.find_last_not_of('0') + 1, std::string::npos);
           batterylevel.erase(batterylevel.find_last_not_of('.') + 1, std::string::npos);
           tandb += batterylevel;
           tandb += "%";

           text->set_text(il2cpp_utils::createcsstr(tandb));
       }
       else {
           text->set_text(il2cpp_utils::createcsstr(timestr));
       }
}