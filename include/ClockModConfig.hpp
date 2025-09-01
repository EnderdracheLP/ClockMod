#pragma once
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig) {

    CONFIG_VALUE(ClockType,          int,    "Clock Type",                  0,     "Which time the Clock should display.");
    CONFIG_VALUE(StopwatchSeconds,   int,    "Stopwatch Seconds",           0,     "The saved time on the stopwatch.");
    CONFIG_VALUE(StopwatchPaused,    bool,   "Stopwatch Paused",            true,  "If the stopwatch is locked from incrementing.")
    CONFIG_VALUE(InSong,             bool,   "Show During Song",            true,  "If the Clock should be shown while playing a beatmap.");
    CONFIG_VALUE(InReplay,           bool,   "Show During Replay",          true,  "If the Clock should be shown while playing a replay.");
    CONFIG_VALUE(TwelveToggle,       bool,   "24/12 Toggle",                false, "If time should be in 12 or 24 hour format.");
    CONFIG_VALUE(SecToggle,          bool,   "Show Seconds",                false, "If seconds should be displayed.");
    CONFIG_VALUE(BattToggle,         bool,   "Show Battery Percentage",     false, "Displays Battery percentage next to the clock.");
    CONFIG_VALUE(RainbowClock,       bool,   "Rainbowify it",               false, "Makes the Clock beautiful.");
    CONFIG_VALUE(FontSize,           float,  "Font Size",                   3.5,   "Changes the Font Size of the Clock (Default: 3.5)");
    CONFIG_VALUE(ClockPosition,      bool,   "Clock Position (Top/Bottom)", false, "If the Clock should be at the top or the bottom");

//    CONFIG_VALUE(ClockXOffset, double, "Clock X Offset", 0);
//    CONFIG_VALUE(ClockYOffset, double, "Clock Y Offset", 0);
//    CONFIG_VALUE(ClockZOffset, double, "Clock Z Offset", 0);
    CONFIG_VALUE(ClockColor, UnityEngine::Color, "Clock Color", UnityEngine::Color(1.0f, 1.0f, 1.0f, 1.0f), "The color of the clock text.");
};
