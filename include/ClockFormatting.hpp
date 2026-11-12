#pragma once

#include <ctime>
#include <string>

namespace ClockMod {
    inline std::string FormatTime(const std::tm* time, const std::string& format) {
        if (!time) return "Time unavailable";
        char buffer[256]{};
        const auto length = std::strftime(buffer, sizeof(buffer), format.c_str(), time);
        return length ? std::string(buffer, length) : "Time unavailable";
    }

}
