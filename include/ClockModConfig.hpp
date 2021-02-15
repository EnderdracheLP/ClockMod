#pragma once
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,

    DECLARE_VALUE(InSong, bool, "Show During Song", true);
    DECLARE_VALUE(TwelveToggle, bool, "24/12 Toggle", false);
    DECLARE_VALUE(SecToggle, bool, "Show Seconds", false);
    DECLARE_VALUE(BattToggle, bool, "Show Battery Percentage", false);
    DECLARE_VALUE(RainbowClock, bool, "Rainbowify it", false);
    DECLARE_VALUE(FontSize, double, "Font Size", 3.5);

    DECLARE_VALUE(ClockXOffset, double, "Clock X Offset", 0);
    DECLARE_VALUE(ClockYOffset, double, "Clock Y Offset", 0);
    DECLARE_VALUE(ClockZOffset, double, "Clock Z Offset", 0);
    DECLARE_VALUE(ClockColor, UnityEngine::Color, "Clock Color", UnityEngine::Color(1.0f, 1.0f, 1.0f, 1.0f));

    DECLARE_VALUE(ClockX, float, "Clock X Pos", 0);
    DECLARE_VALUE(ClockY, float, "Clock Y Pos", -1.7);
    DECLARE_VALUE(ClockZ, float, "Clock Z Pos", 3.85);

    INIT_FUNCTION(
        INIT_VALUE(InSong);
        INIT_VALUE(TwelveToggle);
        INIT_VALUE(SecToggle);
        INIT_VALUE(BattToggle);
        INIT_VALUE(RainbowClock);
        INIT_VALUE(FontSize);
        INIT_VALUE(ClockXOffset);
        INIT_VALUE(ClockYOffset);
        INIT_VALUE(ClockZOffset);
        INIT_VALUE(ClockColor);
        INIT_VALUE(ClockX);
        INIT_VALUE(ClockY);
        INIT_VALUE(ClockZ);
    )
)