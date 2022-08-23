#pragma once
#include "main.hpp"
#include "beatsaber-hook/shared/utils/byref.hpp"
#include "UnityEngine/Color32.hpp"
#include "UnityEngine/Color.hpp"

namespace ClockMod {
    class ColorUtility {
        ColorUtility() = delete;
    public:
        static constexpr uint8_t conv(float colorVal) {
            return std::clamp(static_cast<int>(round(colorVal * 255.0f)), 0, UINT8_MAX);
        }

        static std::string ToHtmlStringRGB(Color color) {
            Color32 color2(conv(color.r), conv(color.g), conv(color.b), 1);
            return string_format("#%02X%02X%02X", color2.r, color2.g, color2.b);
        }

    };
}