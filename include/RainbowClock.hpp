// /*
// Rainbow Clock thanks to Redbrumbler
#pragma once
#include <string>
#include <vector>
#include "UnityEngine/Color.hpp"

namespace ClockMod {
    class RainbowClock
    {
    public:
        static std::string rainbowify(std::string);
        static void textRainbowifier(std::string);
        static inline int rainbowIndex = rand() % 12;
    private:
        static inline const std::vector<std::string> colors = {
            "#ff6060",
            "#ffa060",
            "#ffff60",
            "#a0ff60",
            "#60ff60",
            "#60ffa0",
            "#60ffff",
            "#60a0ff",
            "#6060ff",
            "#a060ff",
            "#ff60ff",
            "#ff60a0"
        };
    };
}
// */