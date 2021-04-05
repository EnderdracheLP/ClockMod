#pragma once
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,

    DECLARE_VALUE(InSong,           bool,   "Show During Song",                     true,   "If the Clock should be shown while playing a Beatmap.");
    DECLARE_VALUE(TwelveToggle,     bool,   "24/12 Toggle",                         false,  "If time should be in 12 or 24 Hour format.");
    DECLARE_VALUE(SecToggle,        bool,   "Show Seconds",                         false,  "If Seconds should be displayed.");
    DECLARE_VALUE(BattToggle,       bool,   "Show Battery Percentage",              false,  "Displays Battery percentage next to the clock.");
    DECLARE_VALUE(RainbowClock,     bool,   "Rainbowify it",                        false,  "Makes the Clock beautiful.");
    DECLARE_VALUE(FontSize,         float,  "Font Size",                            3.5,    "Changes the Font Size of the Clock (Default: 3.5)");
    DECLARE_VALUE(ClockPosition,    bool,   "In-Menu Clock Position (Top/Bottom)",  false,  "If the Clock should be at the Top or the Bottom while in the Menu");

//    DECLARE_VALUE(ClockXOffset, double, "Clock X Offset", 0);
//    DECLARE_VALUE(ClockYOffset, double, "Clock Y Offset", 0);
//    DECLARE_VALUE(ClockZOffset, double, "Clock Z Offset", 0);
    DECLARE_VALUE(ClockColor, UnityEngine::Color, "Clock Color", UnityEngine::Color(1.0f, 1.0f, 1.0f, 1.0f), "The color of the clock text.");

    INIT_FUNCTION(
        INIT_VALUE(InSong);
        INIT_VALUE(TwelveToggle);
        INIT_VALUE(SecToggle);
        INIT_VALUE(BattToggle);
        INIT_VALUE(RainbowClock);
        INIT_VALUE(FontSize);
//        INIT_VALUE(ClockXOffset);
//        INIT_VALUE(ClockYOffset);
//        INIT_VALUE(ClockZOffset);
        INIT_VALUE(ClockPosition);
        INIT_VALUE(ClockColor);
    )
)