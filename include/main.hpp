#pragma once

// Include the modloader header, which allows us to tell the modloader which mod this is, and the version etc.
#include "scotland2/shared/loader.hpp"
#include "paper/shared/logger.hpp"

// beatsaber-hook is a modding framework that lets us call functions and fetch field values from in the game
// It also allows creating objects, configuration, and importantly, hooking methods to modify their values
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

// Define these functions here so that we can easily read configuration and log information from other files
Paper::ConstLoggerContext<9UL> logger();

// Defining values here, so I can use them wherever the fuck I want to.
struct Config_t {
    bool InMPLobby = false;    // Checks if in MPLobby
    bool InMP = false;        // Checks if in MP
    float ClockY = -1.7;
    float ClockZ = 3.85;
    bool IsInSong = false;    // Checks if in a Song
//    bool InSettings = false;
    bool InRotationMap = false; // Checks if in a 360/90 Map
    bool noTextAndHUD = false;
};
extern Config_t Config;
#define MOD_EXPORT extern "C" __attribute__((visibility("default")))

// extern bool InMPLobby;
