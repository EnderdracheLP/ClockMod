#include "main.hpp"
#include "ClockViewController.hpp"

#include "ClockUpdater.hpp"
#include "UnityEngine/GameObject.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include <ctime>
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
    else {      // If set to show 12 Hour Format
        if (getConfig().config["SecToggle"].GetBool() == true) {    //Check if seconds should be shown
            strftime(timestr, 20, "%l:%M:%S %p", timeinfo);
        }
        else {
            strftime(timestr, 20, "%l:%M %p", timeinfo);
        };
    }
       auto text = get_gameObject()->GetComponent<TextMeshProUGUI*>();
       text->set_text(il2cpp_utils::createcsstr(timestr));
}
