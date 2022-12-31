#pragma once
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,

    CONFIG_VALUE(InSong,           bool,   "Show During Song",                     true,   "If the Clock should be shown while playing a Beatmap.");
    CONFIG_VALUE(TwelveToggle,     bool,   "24/12 Toggle",                         false,  "If time should be in 12 or 24 Hour format.");
    CONFIG_VALUE(SecToggle,        bool,   "Show Seconds",                         false,  "If Seconds should be displayed.");
    CONFIG_VALUE(BattToggle,       bool,   "Show Battery Percentage",              false,  "Displays Battery percentage next to the clock.");
    CONFIG_VALUE(RainbowClock,     bool,   "Rainbowify it",                        false,  "Makes the Clock beautiful.");
    CONFIG_VALUE(FontSize,         float,  "Font Size",                            3.5,    "Changes the Font Size of the Clock (Default: 3.5)");
    CONFIG_VALUE(ClockPosition,    bool,   "Clock Position (Top/Bottom)",          false,  "If the Clock should be at the Top or the Bottom");

//    CONFIG_VALUE(ClockXOffset, double, "Clock X Offset", 0);
//    CONFIG_VALUE(ClockYOffset, double, "Clock Y Offset", 0);
//    CONFIG_VALUE(ClockZOffset, double, "Clock Z Offset", 0);
    CONFIG_VALUE(ClockColor, UnityEngine::Color, "Clock Color", UnityEngine::Color(1.0f, 1.0f, 1.0f, 1.0f), "The color of the clock text.");
)
