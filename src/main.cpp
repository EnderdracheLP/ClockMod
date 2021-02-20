#include "main.hpp"

#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/PauseMenuManager.hpp"
#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/CampaignFlowCoordinator.hpp"          // For setting clock in Campaign back to normal
#include "GlobalNamespace/CoreGameHUDController.hpp"            // For checking if in Map.
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"          // For checking the characteristic 360/90.
#include "GlobalNamespace/GameplayCoreInstaller.hpp"            // Also part of the check.
#include "GlobalNamespace/MultiplayerLobbyController.hpp"
#include "GlobalNamespace/HostLobbySetupViewController.hpp"
#include "GlobalNamespace/ClientLobbySetupViewController.hpp"
#include "GlobalNamespace/QuickPlaySetupViewController.hpp"
#include "GlobalNamespace/BeatmapObjectCallbackController.hpp"  // For checking characteristic
#include "GlobalNamespace/IReadonlyBeatmapData.hpp"             // To read the BeatmapData
using namespace GlobalNamespace;

#include "TMPro/TextMeshPro.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "TMPro/TextAlignmentOptions.hpp"
#include "TMPro/TMP_Text.hpp"
using namespace TMPro;

#include "UnityEngine/Canvas.hpp"
#include "unityEngine/CanvasRenderer.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/UI/CanvasScaler.hpp"
#include "UnityEngine/CanvasRenderer.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
using namespace UnityEngine;
using namespace UnityEngine::UI;

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
using namespace QuestUI;

#include "HMUI/CurvedCanvasSettings.hpp"
using namespace HMUI;

#include "ClockUpdater.hpp"
#include "ClockViewController.hpp"
#include "ClockRotationUpdater.hpp"  // Added for 360/90 Rotation Update
using namespace ClockMod;

#include "custom-types/shared/register.hpp"
using namespace custom_types;

#include "ClockModConfig.hpp"

// Clock Default Position and other stuff stored there
Config_t Config;

ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Returns a logger, useful for printing debug messages
Logger& logger() {
    static auto logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

//Define Config
DEFINE_CONFIG(ModConfig);


UnityEngine::Canvas* canvas;
UnityEngine::UI::VerticalLayoutGroup* layout;

MAKE_HOOK_OFFSETLESS(MainMenuViewController_DidActivate, void, MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (firstActivation) {
        auto canvas_object = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("Canvas"));
        canvas = canvas_object->AddComponent<UnityEngine::Canvas*>();
        auto canvas_scaler = canvas_object->AddComponent<CanvasScaler*>();
        auto canvas_renderer = canvas_object->AddComponent<CanvasRenderer*>();

        // For reference on adding Button to clock
        //        UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "", "SettingsButton", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), (Il2CppObject*)nullptr, +[](Il2CppObject* obj, UnityEngine::UI::Button* button){}));
        //button->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), (Il2CppObject*)nullptr, OnClockButtonClick));
        //
        // button->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), (Il2CppObject*)nullptr, OnClockButtonClick));

        canvas_object->AddComponent<CurvedCanvasSettings*>();
        canvas_object->AddComponent<ClockMod::ClockRotationUpdater*>();         // This will add the Componnent from ClockRotationUpdater to the Clock Canvas_object
        canvas_object->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
        canvas_object->get_transform()->set_position(UnityEngine::Vector3(0, 0.5, 0)); // 0, 0.5, 3
        canvas_object->get_transform()->set_localScale(UnityEngine::Vector3(0.1, 0.1, 0.1));

        // This makes sure, the Clock isn't deleted when the scene changes.
        Object::DontDestroyOnLoad(canvas_object);

        canvas->set_renderMode(UnityEngine::RenderMode::WorldSpace);

        layout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(canvas_object->get_transform());
        auto clock_text = QuestUI::BeatSaberUI::CreateText(layout->get_rectTransform(), "");

        layout->GetComponent<LayoutElement*>()->set_minWidth(7);
        layout->GetComponent<LayoutElement*>()->set_minHeight(80);
        layout->set_childAlignment(TMPro::TextAlignmentOptions::Center);
        layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY, Config.ClockZ));

//        clock_text->get_transform()->set_position(UnityEngine::Vector3(0 + getModConfig().ClockXOffset.GetValue(),Config.ClockY + getModConfig().ClockYOffset.GetValue(), Config.ClockZ + getModConfig().ClockZOffset.GetValue()));
//        clock_text->get_transform()->set_position(UnityEngine::Vector3(ClockX + getModConfig().ClockXOffset.GetValue(), ClockY + getModConfig().ClockYOffset.GetValue(), ClockZ + getModConfig().ClockZOffset.GetValue()));
        clock_text->get_gameObject()->AddComponent<ClockMod::ClockUpdater*>();
        Config.IsInSong = false;
        Config.InMPLobby = false;
    }
    layout->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
    // Enables the Clock, I mean what did you expect?
    canvas->get_gameObject()->SetActive(true);
//    layout->get_transform()->set_position(UnityEngine::Vector3(ClockX, ClockY, ClockZ));
}

MAKE_HOOK_OFFSETLESS(AudioTimeSyncController_StartSong, void, AudioTimeSyncController* self, float startTimeOffset) {
    AudioTimeSyncController_StartSong(self, startTimeOffset);

    if (!getModConfig().InSong.GetValue()) {
        canvas->get_gameObject()->SetActive(false);
        logger().info("SetActive false");
    }
    if (Config.InRotationMap) { // Checks if in a map with RotationEvents (360/90)
        if (getModConfig().ClockPosition.GetValue()) { // Then checks if the clock is set to be at the Top or the Bottom
            layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY-1, Config.ClockZ + 0.2));
        }
        else { layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY + 2, Config.ClockZ + 5)); }
    }
    layout->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
    Config.InMPLobby = false;
    Config.IsInSong = true;
//    if (UnityEngine::Object::FindObjectOfType<GlobalNamespace::BeatmapCharacteristicSO*>()->get_containsRotationEvents) {
//        logger().debug("In 360/90 Mode");
//    layout->get_transform()->SetParent(UnityEngine::Object::FindObjectOfType<CoreGameHUDController*>()->get_transform());
//    };
//    layout->get_transform()->set_position(UnityEngine::Vector3(ClockX, ClockY, ClockZ));
//    UnityEngine::Object::FindObjectOfType<CoreGameHUDController*>()->get_transform()->SetParent(layout->get_transform());

//    logger().debug("Object Coords", "%g", clockXOffset, clockYOffset, clockZOffset);
}


//MAKE_HOOK_OFFSETLESS(AudioTimeSyncController_StopSong, void, AudioTimeSyncController* self) {
//    AudioTimeSyncController_StopSong(self);
//    Config.IsInSong = false;
//    layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY, Config.ClockZ));
//}

MAKE_HOOK_OFFSETLESS(CampaignFlowCoordinator_DidActivate, void, CampaignFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    CampaignFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (!getModConfig().InSong.GetValue()) {
        canvas->get_gameObject()->SetActive(true);
    }
    if (Config.noTextAndHUD) {
        canvas->get_gameObject()->SetActive(true);
    }

    Config.IsInSong = false;
    Config.InRotationMap = false;
    layout->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
    layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY, Config.ClockZ));
}

MAKE_HOOK_OFFSETLESS(PartyFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate, void, PartyFreePlayFlowCoordinator* self, bool firstActivation, bool addedToHierarchy) {
    PartyFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate(self, firstActivation, addedToHierarchy);

    if (!getModConfig().InSong.GetValue()) {
        canvas->get_gameObject()->SetActive(true);
    //    logger().info("SetActive true");
    }
    if (Config.noTextAndHUD){
        canvas->get_gameObject()->SetActive(true);
    }

    Config.IsInSong = false;
    Config.InRotationMap = false;
//    layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY, Config.ClockZ));
    layout->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
    layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY, Config.ClockZ));
}

MAKE_HOOK_OFFSETLESS(SoloFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate, void, SoloFreePlayFlowCoordinator* self, bool firstActivation, bool addedToHierarchy) {
    SoloFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate(self, firstActivation, addedToHierarchy);

    if (!getModConfig().InSong.GetValue()) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true");
    }
    canvas->get_gameObject()->SetActive(true);

    Config.IsInSong = false;
    Config.InRotationMap = false;
    //    layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY, Config.ClockZ));
    layout->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
    layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY, Config.ClockZ));
}

MAKE_HOOK_OFFSETLESS(PauseMenuManager_ShowMenu, void, PauseMenuManager* self) {
    PauseMenuManager_ShowMenu(self);

    if (!getModConfig().InSong.GetValue()) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true PauseMenu");
    }
    //if (Config.noTextAndHUD) {
    //    canvas->get_gameObject()->SetActive(false);
    //    logger().info("SetActive false (NoTextAndHUD)");
    //}
//    layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY, Config.ClockZ));
//    Config.IsInSong = false;
}

MAKE_HOOK_OFFSETLESS(PauseMenuManager_StartResumeAnimation, void, PauseMenuManager* self) {
    PauseMenuManager_StartResumeAnimation(self);

    if (!getModConfig().InSong.GetValue()) {
        canvas->get_gameObject()->SetActive(false);
        logger().info("SetActive false PauseMenu");
    }
//    if (Config.InRotationMap) {
//        layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY+2, Config.ClockZ+5));
//    }
//    Config.IsInSong = true;
}

// TODO: Use the below information wisely
// Check if it there are RotationEvents within the map, and if there are, declare it a 360/90 Map.
MAKE_HOOK_OFFSETLESS(BeatmapObjectCallbackController_SetNewBeatmapData, void, BeatmapObjectCallbackController* self, IReadonlyBeatmapData* beatmapData) {
    BeatmapObjectCallbackController_SetNewBeatmapData(self, beatmapData);
    int RotationEvents = beatmapData->get_spawnRotationEventsCount();
       	if (RotationEvents > 0) { 
            logger().debug("Loaded 360/90 Map");
            Config.InRotationMap = true;
//            if (getModConfig().ClockPosition.GetValue()) {
//                layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY + 2, Config.ClockZ + 10));
//            } else { layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY + 2, Config.ClockZ + 5)); }
        }
    	else { 
            logger().debug("Loaded Normal Map");
            Config.InRotationMap = false;
//            layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY + 2, Config.ClockZ + 5));
        }
        //if (Config.noTextAndHUD) {
        //    canvas->get_gameObject()->SetActive(false);
        //    logger().info("SetActive false (NoTextAndHUD)");
        //}
}

//MAKE_HOOK_OFFSETLESS(CoreGameHUDController_Start, void, GlobalNamespace::CoreGameHUDController* self) {
//    CoreGameHUDController_Start(self);
//}

// Multiplayer Lobby Specific Code

//MAKE_HOOK_OFFSETLESS(MultiplayerLobbyController_ActivateMultiplayerLobby, void, MultiplayerLobbyController* self) {
//    MultiplayerLobbyController_ActivateMultiplayerLobby(self);
//
 //   layout->get_transform()->set_position(UnityEngine::Vector3(0, -0.05, 1.62));
 //   layout->get_transform()->set_localScale(UnityEngine::Vector3(0.35, 0.35, 0.35));
//}

MAKE_HOOK_OFFSETLESS(QuickPlaySetupViewController_DidActivate, void, QuickPlaySetupViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    QuickPlaySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

        canvas->get_gameObject()->SetActive(true);

    auto MLobbyVCPosY = UnityEngine::Object::FindObjectOfType<QuickPlaySetupViewController*>()->get_transform()->get_position().y;
    MLobbyVCPosY = MLobbyVCPosY - 1;
    Config.InMPLobby = true;
    Config.IsInSong = false;
    Config.InRotationMap = false;
//    logger().debug("%g", MLobbyVCPosY);
    layout->get_transform()->set_position(UnityEngine::Vector3(0, MLobbyVCPosY, 1.62));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(0.35, 0.35, 0.35));
    layout->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
}

MAKE_HOOK_OFFSETLESS(ClientLobbySetupViewController_DidActivate, void, ClientLobbySetupViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    ClientLobbySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

        canvas->get_gameObject()->SetActive(true);

    auto MLobbyVCPosY = UnityEngine::Object::FindObjectOfType<ClientLobbySetupViewController*>()->get_transform()->get_position().y;
    MLobbyVCPosY = MLobbyVCPosY - 1;
    Config.InMPLobby = true;
    Config.IsInSong = false;
    Config.InRotationMap = false;
//    logger().debug("%g", MLobbyVCPosY);
    layout->get_transform()->set_position(UnityEngine::Vector3(0, MLobbyVCPosY, 1.62));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(0.35, 0.35, 0.35));
    layout->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
}

MAKE_HOOK_OFFSETLESS(HostLobbySetupViewController_DidActivate, void, HostLobbySetupViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    HostLobbySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (!getModConfig().InSong.GetValue()) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true");
    }
    if (!Config.noTextAndHUD) {
        canvas->get_gameObject()->SetActive(true);
    }

    auto MLobbyVCPosY = UnityEngine::Object::FindObjectOfType<HostLobbySetupViewController*>()->get_transform()->get_position().y;
    MLobbyVCPosY = MLobbyVCPosY - 1;
    Config.InMPLobby = true;
    Config.IsInSong = false;
    Config.InRotationMap = false;
//  logger().debug("%g", MLobbyVCPosY);
    layout->get_transform()->set_position(UnityEngine::Vector3(0, MLobbyVCPosY, 1.62));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(0.35, 0.35, 0.35));
    layout->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
}

MAKE_HOOK_OFFSETLESS(MultiplayerLobbyController_DeactivateMultiplayerLobby, void, MultiplayerLobbyController* self) {
    MultiplayerLobbyController_DeactivateMultiplayerLobby(self);

    layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY, Config.ClockZ));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(1.0, 1.0, 1.0));
    Config.InMPLobby = false;
    Config.IsInSong = false;
    layout->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo & info) {
    info.id = "clockmod";
    info.version = VERSION;
    modInfo = info;

    //Init/Load Config
    getModConfig().Init(modInfo);

    logger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    QuestUI::Init();

    logger().info("Installing Clockmod hooks...");

    Logger& hkLog = logger();

    custom_types::Register::RegisterType<ClockMod::ClockUpdater>();
    custom_types::Register::RegisterType<ClockMod::ClockRotationUpdater>();
    custom_types::Register::RegisterType<ClockMod::ClockViewController>();
    QuestUI::Register::RegisterModSettingsViewController<ClockMod::ClockViewController*>(modInfo);

    INSTALL_HOOK_OFFSETLESS(hkLog, MainMenuViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "MainMenuViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(hkLog, CampaignFlowCoordinator_DidActivate, il2cpp_utils::FindMethodUnsafe("", "CampaignFlowCoordinator", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(hkLog, AudioTimeSyncController_StartSong, il2cpp_utils::FindMethodUnsafe("", "AudioTimeSyncController", "StartSong", 1));
    // Third attempt at clock rotation
//    INSTALL_HOOK_OFFSETLESS(hkLog, BeatmapObjectManager_SpawnBasicNote, il2cpp_utils::FindMethodUnsafe("", "BeatmapObjectManager", "SpawnBasicNote", 4));
//    INSTALL_HOOK_OFFSETLESS(hkLog, NoteController_Update, il2cpp_utils::FindMethodUnsafe("", "NoteController", "Update", 0));
    //INSTALL_HOOK_OFFSETLESS(hkLog, AudioTimeSyncController_StopSong, il2cpp_utils::FindMethodUnsafe("", "AudioTimeSyncController", "StopSong", 0));
    INSTALL_HOOK_OFFSETLESS(hkLog, SoloFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate, il2cpp_utils::FindMethodUnsafe("", "SoloFreePlayFlowCoordinator", "SinglePlayerLevelSelectionFlowCoordinatorDidActivate", 2));
    // Added by ELP
    INSTALL_HOOK_OFFSETLESS(hkLog, PartyFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate, il2cpp_utils::FindMethodUnsafe("", "PartyFreePlayFlowCoordinator", "SinglePlayerLevelSelectionFlowCoordinatorDidActivate", 2));
    INSTALL_HOOK_OFFSETLESS(hkLog, PauseMenuManager_ShowMenu, il2cpp_utils::FindMethodUnsafe("", "PauseMenuManager", "ShowMenu", 0));
    INSTALL_HOOK_OFFSETLESS(hkLog, PauseMenuManager_StartResumeAnimation, il2cpp_utils::FindMethodUnsafe("", "PauseMenuManager", "StartResumeAnimation", 0));
    INSTALL_HOOK_OFFSETLESS(hkLog, BeatmapObjectCallbackController_SetNewBeatmapData, il2cpp_utils::FindMethodUnsafe("", "BeatmapObjectCallbackController", "SetNewBeatmapData", 1));
//    INSTALL_HOOK_OFFSETLESS(hkLog, CoreGameHUDController_Start, il2cpp_utils::FindMethodUnsafe("", "CoreGameHUDController", "Start", 0));
//    INSTALL_HOOK_OFFSETLESS(hookLogger, MultiplayerLobbyController_ActivateMultiplayerLobby, il2cpp_utils::FindMethodUnsafe("", "MultiplayerLobbyController", "ActivateMultiplayerLobby", 0));
    // Multiplayer specific Hooks
    INSTALL_HOOK_OFFSETLESS(hkLog, HostLobbySetupViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "HostLobbySetupViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(hkLog, ClientLobbySetupViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "ClientLobbySetupViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(hkLog, QuickPlaySetupViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "QuickPlaySetupViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(hkLog, MultiplayerLobbyController_DeactivateMultiplayerLobby, il2cpp_utils::FindMethodUnsafe("", "MultiplayerLobbyController", "DeactivateMultiplayerLobby", 0));

    logger().info("Installed all ClockMod hooks!");
}