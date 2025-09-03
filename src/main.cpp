#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp" // --------------------- For checking the characteristic 360/90.
#include "GlobalNamespace/BeatmapKey.hpp"
#include "GlobalNamespace/CampaignFlowCoordinator.hpp" // --------------------- For setting clock in Campaign back to normal
#include "GlobalNamespace/FlyingGameHUDRotation.hpp" // ----------------------- Take rotation from this instead lol
#include "GlobalNamespace/LobbySetupViewController.hpp" // -------------------- NEW: For 1.16.4
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/MultiplayerLevelScenesTransitionSetupDataSO.hpp" // - For checking it it's an MP Song
#include "GlobalNamespace/MultiplayerLobbyController.hpp"
#include "GlobalNamespace/MultiplayerResultsViewController.hpp"
#include "GlobalNamespace/OVRPlugin.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/PauseMenuManager.hpp"
#include "GlobalNamespace/PlayerData.hpp" // ---------------------------------- For checking if noTextandHUDs is enabled
#include "GlobalNamespace/PlayerDataModel.hpp" // ----------------------------- For checking if noTextandHUDs is enabled
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp" // ---------------------- For checking if noTextandHUDs is enabled
#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp" // ---- For checking if it's a 360/90 Map
using namespace GlobalNamespace;

#include "TMPro/FontStyles.hpp"
#include "TMPro/TMP_Text.hpp"
#include "TMPro/TextMeshPro.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
using namespace TMPro;

#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/CanvasRenderer.hpp"
#include "UnityEngine/CanvasRenderer.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/TextAnchor.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
using namespace UnityEngine;

#include "UnityEngine/UI/CanvasScaler.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
using namespace UnityEngine::UI;

#include "HMUI/CurvedCanvasSettings.hpp"
using namespace HMUI;

#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "bsml/shared/BSML-Lite/Creation/Layout.hpp"
#include "bsml/shared/BSML-Lite/Creation/Text.hpp"
#include "bsml/shared/BSML.hpp"
#include "conditional-dependencies/shared/main.hpp"
#include "custom-types/shared/register.hpp"
#include "scotland2/shared/loader.hpp"

#include "ClockModConfig.hpp"
#include "ClockUpdater.hpp"
#include "ClockValues.hpp"
#include "ClockViewController.hpp"
#include "main.hpp"
using namespace ClockMod;




// Clock Default Position and other stuff stored there
Config_t Config;

// Clock Positions
ClockPos_t ClockPos;

#if !defined(MAKE_HOOK_MATCH)
#error No supported HOOK macro found, make sure that you have ran: 'qpm-rust restore' and that you have a compatible version of bs-hook
#endif

inline modloader::ModInfo modInfo = {MOD_ID, VERSION, 0}; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Returns a logger, useful for printing debug messages
Paper::ConstLoggerContext<9UL> logger() {
    static auto fastContext = Paper::Logger::WithContext<"ClockMod">();
    return fastContext;
}

float RotateSongX;

bool ClockModInit;

UnityEngine::Canvas* canvas;
UnityEngine::UI::VerticalLayoutGroup* layout;

// Function that sets the ClockPosition for Multiplayer Lobbies.
void MPLobbyClockPos(float MLobbyVCPosY) {
    layout->get_transform()->set_position(UnityEngine::Vector3(0, MLobbyVCPosY-1, 1.75));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(0.35, 0.35, 0.35));
    layout->get_gameObject()->get_transform()->GetParent()->set_eulerAngles(UnityEngine::Vector3(0, 0, 0));
}

MAKE_HOOK_MATCH(MainMenuViewController_DidActivate, &MainMenuViewController::DidActivate, void, MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) 
{
    MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (firstActivation && ClockModInit) {
        auto canvas_object = UnityEngine::GameObject::New_ctor("ClockCanvas");
        canvas = canvas_object->AddComponent<UnityEngine::Canvas*>();
        auto canvas_scaler = canvas_object->AddComponent<CanvasScaler*>();
        auto canvas_renderer = canvas_object->AddComponent<CanvasRenderer*>();

        //canvas_object->AddComponent<QuestUI::Backgroundable*>()->ApplyBackgroundWithAlpha(il2cpp_utils::createcsstr("round-rect-panel"), 0.75f);

        // For reference on adding Button to clock
        //        UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "", "SettingsButton", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), (Il2CppObject*)nullptr, +[](Il2CppObject* obj, UnityEngine::UI::Button* button){}));
        //button->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), (Il2CppObject*)nullptr, OnClockButtonClick));
        //
        // button->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), (Il2CppObject*)nullptr, OnClockButtonClick));

        canvas_object->AddComponent<CurvedCanvasSettings*>();
        canvas_object->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
        canvas_object->get_transform()->set_position(UnityEngine::Vector3(0, 0.5, 0)); // 0, 0.5, 3
        canvas_object->get_transform()->set_localScale(UnityEngine::Vector3(0.1, 0.1, 0.1));

        // This makes sure, the Clock isn't deleted when the scene changes.
        Object::DontDestroyOnLoad(canvas_object);

        // Makes the clock visible for every camera in the scene, if there ever should be more than one.
        canvas->set_renderMode(UnityEngine::RenderMode::WorldSpace);

        layout = BSML::Lite::CreateVerticalLayoutGroup(canvas_object->get_transform());
        //layout = CreateVerticalLayoutGroup(canvas_object->get_transform());
        auto clock_text = BSML::Lite::CreateText(layout->get_rectTransform(), "", TMPro::FontStyles::Normal);
        //auto clock_text = CreateText(layout->get_rectTransform(), "");

        layout->GetComponent<LayoutElement*>()->set_minWidth(7);
        layout->GetComponent<LayoutElement*>()->set_minHeight(80);
        layout->set_childAlignment(UnityEngine::TextAnchor::MiddleCenter);

        clock_text->get_gameObject()->AddComponent<ClockMod::ClockUpdater*>();

        // Check all Mod Config Values

        //if (getModConfig().InSong.GetValue()) { logger().debug("InSong true"); } else { logger().debug("InSong false"); }
        //if (getModConfig().SecToggle.GetValue()) { logger().debug("SecToggle true"); } else { logger().debug("SecToggle false"); }
        //if (getModConfig().FontSize.GetValue()) { logger().debug("FontSize true value %s", getModConfig().FontSize.GetValue()); } else { logger().debug("FontSize false"); }
        //if (getModConfig().BattToggle.GetValue()) { logger().debug("BattToggle true"); } else { logger().debug("BattToggle false"); }
        //if (getModConfig().TwelveToggle.GetValue()) { logger().debug("TwelveToggle true"); } else { logger().debug("TwelveToggle false"); }
        //if (getModConfig().RainbowClock.GetValue()) { logger().debug("RainbowClock true"); } else { logger().debug("RainbowClock false"); }
        ////if (getModConfig().ClockColor.GetValue()) { logger().debug("ClockColor true"); } else { logger().debug("ClockColor false"); }
        //if (getModConfig().ClockPosition.GetValue()) { logger().debug("ClockPosition bottom"); } else { logger().debug("ClockPosition top"); }
        ClockModInit = false;
    }
    Config.IsInSong = false;
    Config.InMPLobby = false;
    // Resets Rotation/Position/Scale
    layout->get_transform()->set_position(ClockPos.MenuPosTop);
    layout->get_gameObject()->get_transform()->GetParent()->set_eulerAngles(UnityEngine::Vector3(0, 0, 0));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
    layout->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0, 0, 0));
    // Enables the Clock, I mean what did you expect?
    canvas->get_gameObject()->SetActive(true);
    Config.InMP = false;
}

// Function for updating the Clock Position
void SetClockPos(UnityEngine::Vector3 Pos, UnityEngine::Vector3 Angle, float ScaleFloat) {
    layout->get_gameObject()->get_transform()->GetParent()->set_eulerAngles(UnityEngine::Vector3(0, 0, 0));
    layout->get_transform()->GetParent()->set_position(UnityEngine::Vector3(0, 0.5, 0));
    layout->get_transform()->set_position(Pos);
    if(Config.InRotationMap) layout->get_transform()->set_localEulerAngles(Angle); else layout->get_gameObject()->get_transform()->GetParent()->set_eulerAngles(Angle);
    layout->get_transform()->set_localScale(UnityEngine::Vector3(ScaleFloat, ScaleFloat, ScaleFloat));
}

// TODO: Use a different hook, this one is too small and causes issues
MAKE_HOOK_MATCH(AudioTimeSyncController_StartSong, &AudioTimeSyncController::StartSong, void, AudioTimeSyncController* self, float startTimeOffset) {
    // Instance of PlayerDataModel the noTextAndHUDs variable specifically
    Config.noTextAndHUD = UnityEngine::Object::FindObjectOfType<PlayerDataModel*>()->playerData->get_playerSpecificSettings()->noTextsAndHuds;

    //float LayoutClockPosX = layout->get_transform()->get_position().x;
    //float LayoutClockPosY = layout->get_transform()->get_position().y;
    //float LayoutClockPosZ = layout->get_transform()->get_position().z;
    //float LayoutClockRotX = layout->get_transform()->get_eulerAngles().x;
    //logger().debug("ClockPos before X %f", LayoutClockPosX);
    //logger().debug("ClockPos before Y %f", LayoutClockPosY);
    //logger().debug("ClockPos before Z %f", LayoutClockPosZ);
    //logger().debug("ClockRot before X %f", LayoutClockRotX);

    Config.InMPLobby = false;
    Config.IsInSong = true;

    AudioTimeSyncController_StartSong(self, startTimeOffset);


    bool isInReplay;
    static std::optional<bool (*)()> isInReplayFunc  = CondDeps::Find<bool>("replay", "IsInReplay");
    if (isInReplayFunc.has_value())
    {
        static std::optional<bool (*)()> isInRenderFunc  = CondDeps::Find<bool>("replay", "IsInRender");
        bool isInRender = false;
        isInReplay = isInReplayFunc.value()();
        if (isInRenderFunc.has_value())
                isInRender = isInRenderFunc.value()();
        logger().info("Found replay mod, check in replay: {}, check in render: {}", isInReplay, isInRender);
        Config.noTextAndHUD |=  (isInReplay && !getModConfig().InReplay.GetValue()) || isInRender;
    }

    if (!getModConfig().InSong.GetValue() || Config.noTextAndHUD) {
        canvas->get_gameObject()->SetActive(false);
        logger().info("SetActive false");
    }
    // TODO: Tweak values
    if (/*Config.InMP ||*/ Config.InRotationMap) { // Checks if in a map with RotationEvents (360/90) or in a MP Song.
        if (getModConfig().ClockPosition.GetValue()) { // Then checks if the clock is set to be at the Top or the Bottom
            SetClockPos(ClockPos.RotateSongPosDown, ClockPos.RotateSongRotationDown, ClockPos.RotateSongScaleDown);
            logger().info("SetPos 360/MP Bottom");
        }
        else { 
        logger().info("SetPos 360/MP Top");
        SetClockPos(ClockPos.RotateSongPosTop, ClockPos.RotateSongRotationTop, ClockPos.RotateSongScaleTop);

        //LayoutClockPosX = layout->get_transform()->get_position().x;
        //LayoutClockPosY = layout->get_transform()->get_position().y;
        //LayoutClockPosZ = layout->get_transform()->get_position().z;
        //logger().debug("ClockPos X %f", LayoutClockPosX);
        //logger().debug("ClockPos Y %f", LayoutClockPosY);
        //logger().debug("ClockPos Z %f", LayoutClockPosZ);
        }
    }
    else if (getModConfig().ClockPosition.GetValue()) { // When in a normal Map do this.
        // If set to be at the Bottom do this.
        //auto Pos = UnityEngine::Vector3(0, -4.45, 2);
        //auto Angle = UnityEngine::Vector3(45, 0, 0);
        //auto Scale = UnityEngine::Vector3(ClockPos.NormalSongScaleDown, ClockPos.NormalSongScaleDown, ClockPos.NormalSongScaleDown);
        SetClockPos(ClockPos.NormalSongPosDown, ClockPos.NormalSongRotationDown, ClockPos.NormalSongScaleDown);
        //LayoutClockPosX = layout->get_transform()->get_position().x;
        //LayoutClockPosY = layout->get_transform()->get_position().y;
        //LayoutClockPosZ = layout->get_transform()->get_position().z;
        //LayoutClockRotX = layout->get_transform()->get_eulerAngles().x;
        //logger().debug("ClockPos after X %f", LayoutClockPosX);
        //logger().debug("ClockPos after Y %f", LayoutClockPosY);
        //logger().debug("ClockPos after Z %f", LayoutClockPosZ);
        //logger().debug("ClockRot after X %f", LayoutClockRotX);
        logger().info("In Normal Map set to Bottom");
    }
    else {
        // Otherwise it will do this.
        //auto Pos = UnityEngine::Vector3(0, -1.7, 5.6);
        //auto Angle = UnityEngine::Vector3(-10, 0, 0);
        //auto Scale = UnityEngine::Vector3(1, 1, 1);
        auto pos = ClockPos.NormalSongPosTop;
        if (isInReplay) pos.y += 0.4f;
        SetClockPos(pos, ClockPos.NormalSongRotationTop, ClockPos.NormalSongScaleTop);
        logger().info("In Normal Map set to Top");
    } 
}

//MAKE_HOOK_MATCH(SceneManager_Internal_ActiveSceneChanged, &UnityEngine::SceneManagement::SceneManager::Internal_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene prevScene, UnityEngine::SceneManagement::Scene nextScene) {
//    SceneManager_Internal_ActiveSceneChanged(prevScene, nextScene);
//    if (nextScene.IsValid()) {
//        std::string sceneName = to_utf8(csstrtostr(nextScene.get_name()));
//        logger().info("Scene found: %s", sceneName.data());
//        if (sceneName.find("GameCore") != std::string::npos) {
//            // Instance of PlayerDataModel the noTextAndHUDs variable specifically
//            Config.noTextAndHUD = UnityEngine::Object::FindObjectOfType<PlayerDataModel*>()->playerData->playerSpecificSettings->noTextsAndHuds;
//
//            float LayoutClockPosX = layout->get_transform()->get_position().x;
//            float LayoutClockPosY = layout->get_transform()->get_position().y;
//            float LayoutClockPosZ = layout->get_transform()->get_position().z;
//            float LayoutClockRotX = layout->get_transform()->get_eulerAngles().x;
//            logger().debug("ClockPos before X %f", LayoutClockPosX);
//            logger().debug("ClockPos before Y %f", LayoutClockPosY);
//            logger().debug("ClockPos before Z %f", LayoutClockPosZ);
//            logger().debug("ClockRot before X %f", LayoutClockRotX);
//
//            Config.InMPLobby = false;
//            Config.IsInSong = true;
//
//            if (!getModConfig().InSong.GetValue() || Config.noTextAndHUD) {
//                canvas->get_gameObject()->SetActive(false);
//                logger().info("SetActive false");
//            }
//            // TODO: Tweak values
//            if (/*Config.InMP ||*/ Config.InRotationMap) { // Checks if in a map with RotationEvents (360/90) or in a MP Song.
//                if (getModConfig().ClockPosition.GetValue()) { // Then checks if the clock is set to be at the Top or the Bottom
//                    SetClockPos(ClockPos.RotateSongPosDown, ClockPos.RotateSongRotationDown, ClockPos.RotateSongScaleDown);
//                    logger().debug("SetPos 360/MP Bottom");
//                }
//                else {
//                    logger().debug("SetPos 360/MP Top");
//                    SetClockPos(ClockPos.RotateSongPosTop, ClockPos.RotateSongRotationTop, ClockPos.RotateSongScaleTop);
//
//                    LayoutClockPosX = layout->get_transform()->get_position().x;
//                    LayoutClockPosY = layout->get_transform()->get_position().y;
//                    LayoutClockPosZ = layout->get_transform()->get_position().z;
//                    logger().debug("ClockPos X %f", LayoutClockPosX);
//                    logger().debug("ClockPos Y %f", LayoutClockPosY);
//                    logger().debug("ClockPos Z %f", LayoutClockPosZ);
//                }
//            }
//            else if (getModConfig().ClockPosition.GetValue()) { // When in a normal Map do this.
//                // If set to be at the Bottom do this.
//                //auto Pos = UnityEngine::Vector3(0, -4.45, 2);
//                //auto Angle = UnityEngine::Vector3(45, 0, 0);
//                //auto Scale = UnityEngine::Vector3(ClockPos.NormalSongScaleDown, ClockPos.NormalSongScaleDown, ClockPos.NormalSongScaleDown);
//                SetClockPos(ClockPos.NormalSongPosDown, ClockPos.NormalSongRotationDown, ClockPos.NormalSongScaleDown);
//                LayoutClockPosX = layout->get_transform()->get_position().x;
//                LayoutClockPosY = layout->get_transform()->get_position().y;
//                LayoutClockPosZ = layout->get_transform()->get_position().z;
//                LayoutClockRotX = layout->get_transform()->get_eulerAngles().x;
//                logger().debug("ClockPos after X %f", LayoutClockPosX);
//                logger().debug("ClockPos after Y %f", LayoutClockPosY);
//                logger().debug("ClockPos after Z %f", LayoutClockPosZ);
//                logger().debug("ClockRot after X %f", LayoutClockRotX);
//                //logger().debug("In Normal Map set to Bottom");
//            }
//            else {
//                // Otherwise it will do this.
//                //auto Pos = UnityEngine::Vector3(0, -1.7, 5.6);
//                //auto Angle = UnityEngine::Vector3(-10, 0, 0);
//                //auto Scale = UnityEngine::Vector3(1, 1, 1);
//                SetClockPos(ClockPos.NormalSongPosTop, ClockPos.NormalSongRotationTop, ClockPos.NormalSongScaleTop);
//                //logger().debug("In Normal Map set to Top");
//            }
//        }
//    }
//}

//MAKE_HOOK_MATCH(AudioTimeSyncController_StopSong, &AudioTimeSyncController::StopSong, void, AudioTimeSyncController* self) {
//    AudioTimeSyncController_StopSong(self);
//    Config.IsInSong = false;
//    layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY, Config.ClockZ));
//}

MAKE_HOOK_MATCH(CampaignFlowCoordinator_DidActivate, &CampaignFlowCoordinator::DidActivate, void, CampaignFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    CampaignFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (!getModConfig().InSong.GetValue() || Config.noTextAndHUD) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true Campaign");
    }

    Config.IsInSong = false;
    Config.InRotationMap = false;

    //layout->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
    layout->get_transform()->set_position(ClockPos.MenuPosTop);
    layout->get_gameObject()->get_transform()->GetParent()->set_eulerAngles(UnityEngine::Vector3(0, 0, 0));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
    layout->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0, 0, 0));
}

MAKE_HOOK_MATCH(PartyFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate, &PartyFreePlayFlowCoordinator::SinglePlayerLevelSelectionFlowCoordinatorDidActivate, void, PartyFreePlayFlowCoordinator* self, bool firstActivation, bool addedToHierarchy) {
    PartyFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate(self, firstActivation, addedToHierarchy);

    if (!getModConfig().InSong.GetValue() || Config.noTextAndHUD) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true PartyFreePlay");
    }

    Config.IsInSong = false;
    Config.InRotationMap = false;

    //layout->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
    layout->get_transform()->set_position(ClockPos.MenuPosTop);
    layout->get_gameObject()->get_transform()->GetParent()->set_eulerAngles(UnityEngine::Vector3(0, 0, 0));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
    layout->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0, 0, 0));
}

MAKE_HOOK_MATCH(SoloFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate, &SoloFreePlayFlowCoordinator::SinglePlayerLevelSelectionFlowCoordinatorDidActivate, void, SoloFreePlayFlowCoordinator* self, bool firstActivation, bool addedToHierarchy) {
    SoloFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate(self, firstActivation, addedToHierarchy);

    if (!getModConfig().InSong.GetValue() || Config.noTextAndHUD) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true SoloFreePlay");
    }

    Config.IsInSong = false;
    Config.InRotationMap = false;
    //    layout->get_transform()->set_position(UnityEngine::Vector3(0, Config.ClockY, Config.ClockZ));
    //layout->get_transform()->(UnityEngine::Quaternion(0, 0, 0, 1));
    layout->get_transform()->set_position(ClockPos.MenuPosTop);
    layout->get_gameObject()->get_transform()->GetParent()->set_eulerAngles(UnityEngine::Vector3(0, 0, 0));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
    layout->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0, 0, 0));
}

MAKE_HOOK_MATCH(PauseMenuManager_ShowMenu, &PauseMenuManager::ShowMenu, void, PauseMenuManager* self) {
    PauseMenuManager_ShowMenu(self);

    if (!getModConfig().InSong.GetValue() || Config.noTextAndHUD) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true PauseMenu");
    }
}

MAKE_HOOK_MATCH(PauseMenuManager_StartResumeAnimation, &PauseMenuManager::StartResumeAnimation, void, PauseMenuManager* self) {
    PauseMenuManager_StartResumeAnimation(self);

    if (!getModConfig().InSong.GetValue() || Config.noTextAndHUD) {
        canvas->get_gameObject()->SetActive(false);
        logger().info("SetActive false PauseMenu");
    }
}

// TODO: Use the below information wisely
// Check the beatmap characteristic and set the Config.InRotationMap to true if it requires 360 Movement.
MAKE_HOOK_MATCH(StandardLevelScenesTransitionSetupDataSO_InitAndSetupScenes, &StandardLevelScenesTransitionSetupDataSO::InitAndSetupScenes, void, StandardLevelScenesTransitionSetupDataSO* self, PlayerSpecificSettings* playerSpecificSettings, StringW backButtonText, bool startPaused) {
    StandardLevelScenesTransitionSetupDataSO_InitAndSetupScenes(self, playerSpecificSettings, backButtonText, startPaused);

    if (self->beatmapKey.beatmapCharacteristic->containsRotationEvents) {
        logger().debug("Loaded 360/90 Map");
        Config.InRotationMap = true;
    }
    else {
        logger().debug("Loaded Normal Map");
        Config.InRotationMap = false;
    }
}

MAKE_HOOK_MATCH(FlyingGameHUDRotation_FixedUpdate, &FlyingGameHUDRotation::FixedUpdate, void, GlobalNamespace::FlyingGameHUDRotation* self) {
    layout->get_gameObject()->get_transform()->GetParent()->set_eulerAngles(UnityEngine::Vector3(0, self->_yAngle, 0));
    FlyingGameHUDRotation_FixedUpdate(self);
}


MAKE_HOOK_MATCH(LobbySetupViewController_DidActivate, &LobbySetupViewController::DidActivate, void, LobbySetupViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {

    if (!getModConfig().InSong.GetValue() || Config.noTextAndHUD) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true Multiplayer Lobby");
    }
    layout->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0, 0, 0));

    auto MLobbyVCPosY = self->get_transform()->get_position().y;
    Config.InMPLobby = true;
    Config.IsInSong = false;
    Config.InMP = true;
    Config.InRotationMap = false;
//    logger().debug("%g", MLobbyVCPosY);
    MPLobbyClockPos(MLobbyVCPosY);
    if (Config.InMP == true) { logger().debug("CLobby, InMP is True"); }

    LobbySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_HOOK_MATCH(MultiplayerResultsViewController_DidActivate, &MultiplayerResultsViewController::DidActivate, void, MultiplayerResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    MultiplayerResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (!getModConfig().InSong.GetValue() || Config.noTextAndHUD) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true MPResultsVC");
    }
    layout->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0, 0, 0));

    auto MLobbyVCPosY = self->get_transform()->get_position().y;
    Config.InMPLobby = true;
    Config.IsInSong = false;
    Config.InMP = true;
    Config.InRotationMap = false;
    //  logger().debug("%g", MLobbyVCPosY);
    MPLobbyClockPos(MLobbyVCPosY);
    if (Config.InMP) { logger().debug("MRVC, InMP is True"); }
}

MAKE_HOOK_MATCH(MultiplayerResultsViewController_Init, &MultiplayerResultsViewController::Init, void, MultiplayerResultsViewController* self, MultiplayerResultsData* multiplayerResultsData, BeatmapKey beatmapKey, bool showBackToLobbyButton, bool showBackToMenuButton)
{
    MultiplayerResultsViewController_Init(self, multiplayerResultsData, beatmapKey, showBackToLobbyButton, showBackToMenuButton);
    if (!getModConfig().InSong.GetValue() || Config.noTextAndHUD) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true MPResultsVC");
    }
    layout->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0, 0, 0));

    auto MLobbyVCPosY = self->get_transform()->get_position().y;
    Config.InMPLobby = true;
    Config.IsInSong = false;
    Config.InMP = true;
    Config.InRotationMap = false;
    MPLobbyClockPos(MLobbyVCPosY);
    if (Config.InMP) { logger().debug("MRVC, InMP is True"); }
}
MAKE_HOOK_MATCH(MultiplayerLobbyController_DeactivateMultiplayerLobby, &MultiplayerLobbyController::DeactivateMultiplayerLobby, void, MultiplayerLobbyController* self) {
    MultiplayerLobbyController_DeactivateMultiplayerLobby(self);

    Config.InMPLobby = false;
    Config.IsInSong = false;
    //layout->get_transform()->set_rotation(UnityEngine::Quaternion(0, 0, 0, 1));
    layout->get_transform()->set_position(ClockPos.MenuPosTop);
    layout->get_gameObject()->get_transform()->GetParent()->set_eulerAngles(UnityEngine::Vector3(0, 0, 0));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(1, 1, 1));
    layout->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0, 0, 0));
}

#include <random>
#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
MAKE_HOOK_MATCH(ResultsViewController_DidActivate, &ResultsViewController::DidActivate, void, ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    std::vector<std::string> failTexts({ "Get Better", "fail", "lol", "learn2play", "no skills", "get skills", "loser", "hit bloq", "no comment", "you failed", "can you even play" });
    if (ClockPos.ap1 && self->_levelCompletionResults->levelEndStateType == LevelCompletionResults::LevelEndStateType::Failed) {
        ClockUpdater* clockUpdater = ClockUpdater::getInstance();
        if (clockUpdater) {
            std::random_device random_device;
            std::mt19937 engine{ random_device() };
            std::uniform_int_distribution<int> dist(0, failTexts.size() - 1);
            clockUpdater->ShowMessage(failTexts.at(dist(engine)));
        }
    }
}


// Called at the early stages of game loading
MOD_EXPORT void setup(CModInfo *info) {
    *info = modInfo.to_c();

    getModConfig().Init(modInfo);

    Paper::Logger::RegisterFileContextId(logger().tag);

    ClockModInit = true;

    logger().info("Completed ClockMod setup!");
}

// Called later on in the game loading - a good time to install function hooks
MOD_EXPORT void late_load() {
    il2cpp_functions::Init();
    BSML::Init();

    logger().debug("Init ModConfig...");

    //Init/Load Config
    getModConfig().Init(modInfo);

    logger().debug("Init ModConfig...Done");

    logger().debug("CheckTime...");
    time_t rawtime;
    time(&rawtime);
    struct tm* timeinfo = localtime(&rawtime);
    // Months start at 0 so 0 = January while days start at 1
    ClockPos.ap1 = (timeinfo && timeinfo->tm_mon == 3 && timeinfo->tm_mday == 1);
    logger().debug("Year is: %d Day is: %d Month is: %d time is %d:%d:%d", 
    timeinfo->tm_year, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    logger().debug("CheckTime...Done");

    logger().info("Installing Clockmod hooks...");

    auto hkLog = logger();

    custom_types::Register::AutoRegister();
    BSML::Register::RegisterSettingsMenu<ClockMod::ClockViewController*>("ClockMod");
    BSML::Register::RegisterMainMenu<ClockMod::ClockViewController*>("ClockMod", "ClockMod", "Open the ClockMod Settings");

    if (timeinfo && (timeinfo->tm_mon == 2 && timeinfo->tm_mday == 31) || (timeinfo->tm_mon == 3 && timeinfo->tm_mday == 1)) {
        INSTALL_HOOK(hkLog, ResultsViewController_DidActivate);
    }

    INSTALL_HOOK(hkLog, MainMenuViewController_DidActivate);
    INSTALL_HOOK(hkLog, CampaignFlowCoordinator_DidActivate);
    INSTALL_HOOK(hkLog, AudioTimeSyncController_StartSong);
    INSTALL_HOOK(hkLog, SoloFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate);
    INSTALL_HOOK(hkLog, FlyingGameHUDRotation_FixedUpdate);
    INSTALL_HOOK(hkLog, PartyFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate);
    INSTALL_HOOK(hkLog, PauseMenuManager_ShowMenu);
    INSTALL_HOOK(hkLog, PauseMenuManager_StartResumeAnimation);
    INSTALL_HOOK(hkLog, StandardLevelScenesTransitionSetupDataSO_InitAndSetupScenes);
    INSTALL_HOOK(hkLog, LobbySetupViewController_DidActivate);
    INSTALL_HOOK(hkLog, MultiplayerLobbyController_DeactivateMultiplayerLobby);
    INSTALL_HOOK(hkLog, MultiplayerResultsViewController_DidActivate);
    logger().info("Installed all ClockMod hooks!");
}
