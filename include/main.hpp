#pragma once

// Include the modloader header, which allows us to tell the modloader which mod this is, and the version etc.
#include "modloader/shared/modloader.hpp"

// beatsaber-hook is a modding framework that lets us call functions and fetch field values from in the game
// It also allows creating objects, configuration, and importantly, hooking methods to modify their values
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "UnityEngine/Vector3.hpp"

// Define these functions here so that we can easily read configuration and log information from other files
Logger& logger();

// Defining values here, so I can use them wherever the fuck I want to.
struct Config_t {
	bool InMPLobby = false;
	float ClockY = -1.7;
	float ClockZ = 3.85;
	bool IsInSong = false;
//	bool InSettings = false;
	bool InRotationMap = false;
	bool noTextAndHUD = false;
};
extern Config_t Config;

// This containts all the ClockPositions
struct ClockPos_t {
	// Clock InMenu Positions Top
	float MenuXtop = 0;
	float MenuYtop = -1.7;
	float MenuZtop = 3.85;
	UnityEngine::Vector3 MenuPosTop = UnityEngine::Vector3(0, -1.7, 3.85);
	// Clock InMenu Positions Bottom	// UnityEngine::Vector3(0, Config.ClockY, Config.ClockZ)
	float MenuXdown = 0;
	float MenuYdown = 0;
	float MenuZdown = 0;

	// Clock InNormalSong Positions Top 0, Config.ClockY + 1.3, Config.ClockZ + 5
	UnityEngine::Vector3 NormalSongPosTop = UnityEngine::Vector3(0, 0, 0);
	// Clock InNormalSong Positions Bottom
	UnityEngine::Vector3 NormalSongPosDown = UnityEngine::Vector3(0, 0, 0);
	float NormalSongScaleDown = 0;
	UnityEngine::Vector3 NormalSongRotationDown = UnityEngine::Vector3(0, 0, 0);

	// Clock In360/90 Song Positions Top
	UnityEngine::Vector3 RotateSongPosTop = UnityEngine::Vector3(0, 0.4, -1.15);
	float RotateSongScaleTop = 1;														// Scale of the Clock in 360/90 Songs Bottom
	//Clock in360/90 Song Positions/Scale/Rotations Bottom
	UnityEngine::Vector3 RotateSongPosDown = UnityEngine::Vector3(0, -0.9, 1.8);		// Position of the Clock in 360/90 Songs Bottom
	float RotateSongScaleDown = 0.8;
	UnityEngine::Vector3 RotateSongRotationDown = UnityEngine::Vector3(-40, 0, 0);		// Rotation of the Clock in 360/90 Songs Bottom

	// Clock Position for MPLobby
	float MPLobbyX = 0;
	float MPLobbyY = 0;
	float MPLobbyZ = 0;

	// Clock Position MPSong Top
	float MPSongXtop = 0;
	float MPSongYtop = 0;
	float MPSongZtop = 0;
	// Clock Position MPSong Bottom
	float MPSongXdown = 0;
	float MPSongYdown = 0;
	float MPSongZdown = 0;
};
extern ClockPos_t ClockPos;

/*
if (getModConfig().ClockPosition.GetValue()) { // Then checks if the clock is set to be at the Top or the Bottom
	layout->get_transform()->set_position(UnityEngine::Vector3(0, -0.4, 1.8)); // Y = -2.26 Z = 0.2
	layout->get_transform()->set_localEulerAngles(UnityEngine::Vector3(-40, 0, 0)); // X=40
	layout->get_transform()->set_localScale(UnityEngine::Vector3(0.8, 0.8, 0.8));   // Scale 0.8
}
else {
	layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY + 1.3, Config.ClockZ + 5)); // Y =  + 2
	layout->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
}
*/
// extern bool InMPLobby;