#pragma once
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,

    DECLARE_VALUE(InSong, bool, "Show During Song", true);
    DECLARE_VALUE(TwelveToggle, bool, "24/12 Toggle", false);
    DECLARE_VALUE(SecToggle, bool, "Show Seconds", false);
    DECLARE_VALUE(BattToggle, bool, "Show Battery Percentage", false);
    DECLARE_VALUE(FontSize, float, "Font Size", 4);

    DECLARE_VALUE(ClockXOffset, float, "In Song Clock X Offset", 0);
    DECLARE_VALUE(ClockYOffset, float, "In Song Clock Y Offset", 0);
    DECLARE_VALUE(ClockZOffset, float, "In Song Clock Z Offset", 0);
    DECLARE_VALUE(ClockColor, UnityEngine::Color, "Clock Color", UnityEngine::Color(0.0f, 0.0f, 0.0f, 1.0f));

    INIT_FUNCTION(
        INIT_VALUE(InSong);
        INIT_VALUE(TwelveToggle);
        INIT_VALUE(SecToggle);
        INIT_VALUE(BattToggle);
        INIT_VALUE(FontSize);
        INIT_VALUE(ClockXOffset);
        INIT_VALUE(ClockYOffset);
        INIT_VALUE(ClockZOffset);
        INIT_VALUE(ClockColor);
    )
)