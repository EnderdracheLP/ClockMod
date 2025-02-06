#pragma once
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Color32.hpp"

using namespace UnityEngine;

namespace ClockMod {
    class ColorUtility {
        ColorUtility() = delete;
    public:
        static constexpr uint8_t conv(float colorVal) {
            return std::clamp(static_cast<int>(round(colorVal * 255.0f)), 0, UINT8_MAX);
        }

        static std::string ToHtmlStringRGB(Color color) {
            Color32 color2(0, conv(color.r), conv(color.g), conv(color.b), 1);
            return fmt::format("#{0:02x}{1:02x}{2:02x}", color2.r, color2.g, color2.b);
        }

    };
}