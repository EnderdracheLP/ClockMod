#pragma once
#include "UnityEngine/Vector3.hpp"


// This containts all the ClockPositions
struct ClockPos_t {
	// Clock InMenu Positions Top
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