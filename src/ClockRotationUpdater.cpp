#include "main.hpp"
#include "ClockRotationUpdater.hpp" // Part of making this a class
#include "ClockModConfig.hpp"		// Just the config
using namespace ClockMod;

#include "GlobalNamespace/CoreGameHUDController.hpp"  // Just take the damn rotation from CoreGameHUDController, and it'll rotate smooth as fuck.
#include "GlobalNamespace/PlayerDataModel.hpp"					// For checking if noTextandHUDs is enabled
#include "GlobalNamespace/PlayerData.hpp"						// For checking if noTextandHUDs is enabled
#include "GlobalNamespace/PlayerSpecificSettings.hpp"			// For checking if noTextandHUDs is enabled
using namespace GlobalNamespace;

// Needed for the Transform stuff.
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Canvas.hpp"
using namespace UnityEngine;

DEFINE_CLASS(ClockMod::ClockRotationUpdater);

// Updates every frame and runs the following code.
// Setting any Position does not work well here, please don't try, it is only for Realtime Rotation.
void ClockMod::ClockRotationUpdater::Update() {
	if (Config.IsInSong) {
		// Instance of PlayerDataModel the noTextAndHUDs variable specifically
		bool NoTextAndHUD = UnityEngine::Object::FindObjectOfType<PlayerDataModel*>()->playerData->playerSpecificSettings->noTextsAndHuds;
		if (NoTextAndHUD == false) {
			Config.noTextAndHUD = false;
			// This will get an instance of CoreGameHUDController
			auto coreGameHUDController = UnityEngine::Object::FindObjectOfType<CoreGameHUDController*>();
			// Checks if the CoreGameHUDControlller exists, and will run the following code.
			if (coreGameHUDController) {
				if (Config.InRotationMap) {
					get_transform()->set_rotation(coreGameHUDController->get_transform()->get_rotation());
				}
				//			else if (getModConfig().ClockPosition.GetValue()) {
				//				get_transform()->set_eulerAngles(UnityEngine::Vector3(60, 0, 0));
				//			}
				//			else { get_transform()->set_eulerAngles(UnityEngine::Vector3(0, 0, 0)); }
							// Gets the rotation from the CoreGameHUDController and applies it to the Clock
			}
			//		else if (getModConfig().ClockPosition.GetValue() == false) {
			//			get_transform()->set_position(UnityEngine::Vector3(0, -1.25, -0.5));
			//			get_transform()->set_eulerAngles(UnityEngine::Vector3(60, 0, 0));
			//			get_transform()->set_localScale(UnityEngine::Vector3(0.6, 0.6, 0.6));
			//			get_transform()->set_localPosition(UnityEngine::Vector3(getModConfig().ClockXOffset.GetValue(), getModConfig().ClockYOffset.GetValue(), getModConfig().ClockZOffset.GetValue()));
			//		}
			else {
				//			get_transform()->GetParent()->set_position(UnityEngine::Vector3(0, 0, 0));
				get_transform()->set_eulerAngles(UnityEngine::Vector3(0, 0, 0));
			}
		} 
		else { 
		Config.noTextAndHUD = true;
		GetComponent<UnityEngine::Canvas*>()->get_gameObject()->SetActive(false);
		}
	}
}

//		get_transform()->set_rotation(UnityEngine::Object::FindObjectOfType<NoteController*>()->get_worldRotation());
//auto ClockRot = UnityEngine::Object::FindObjectOfType<NoteController*>()->get_worldRotation();
//layout->get_transform()->GetParent()->set_rotation(ClockRot);