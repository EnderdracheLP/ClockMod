#include "main.hpp"



#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/PauseMenuManager.hpp"
#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp" // Added by ELP
#include "GlobalNamespace/MultiplayerLobbyController.hpp"
#include "GlobalNamespace/HostLobbySetupViewController.hpp"
#include "GlobalNamespace/ClientLobbySetupViewController.hpp"
#include "GlobalNamespace/QuickPlaySetupViewController.hpp"
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
using namespace ClockMod;

#include "custom-types/shared/register.hpp"
using namespace custom_types;

ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& logger() {
    static auto logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}
// Clock Default Position
float ClockX = 0;
float ClockY = -1.7;
float ClockZ = 3.85;

UnityEngine::Canvas* canvas;
UnityEngine::UI::VerticalLayoutGroup* layout;

MAKE_HOOK_OFFSETLESS(MainMenuViewController_DidActivate, void, MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (firstActivation) {
        auto canvas_object = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("Canvas"));
        canvas = canvas_object->AddComponent<UnityEngine::Canvas*>();
        auto canvas_scaler = canvas_object->AddComponent<CanvasScaler*>();
        auto canvas_renderer = canvas_object->AddComponent<CanvasRenderer*>();

        canvas_object->AddComponent<CurvedCanvasSettings*>();
        canvas_object->get_transform()->set_position(UnityEngine::Vector3(0, 0.5, 3));
        canvas_object->get_transform()->set_localScale(UnityEngine::Vector3(0.1, 0.1, 0.1));

        Object::DontDestroyOnLoad(canvas_object);

        canvas->set_renderMode(UnityEngine::RenderMode::WorldSpace);

        layout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(canvas_object->get_transform());
        auto clock_text = QuestUI::BeatSaberUI::CreateText(layout->get_rectTransform(), "");

        layout->GetComponent<LayoutElement*>()->set_minWidth(7);
        layout->GetComponent<LayoutElement*>()->set_minHeight(80);
        layout->set_childAlignment(TMPro::TextAlignmentOptions::Center);
        layout->get_transform()->set_position(UnityEngine::Vector3(0, -1.7, 3.85));
//        float fontsize = getConfig().config["FontSize"].GetFloat();
        clock_text->set_fontSize(getConfig().config["FontSize"].GetFloat());

        clock_text->get_transform()->set_position(UnityEngine::Vector3(0, 0.5, 3.85));
        clock_text->get_gameObject()->AddComponent<ClockMod::ClockUpdater*>();

        auto clock_color = UnityEngine::Color::get_white();
        clock_color.r = getConfig().config["r"].GetFloat();
        clock_color.g = getConfig().config["g"].GetFloat();
        clock_color.b = getConfig().config["b"].GetFloat();
        clock_text->set_color(clock_color);
    }
    canvas->get_gameObject()->SetActive(true);
    layout->get_transform()->set_position(UnityEngine::Vector3(ClockX, ClockY, ClockZ));
}

//void ClockMod::ClockViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {

//        float fontsize = getConfig().config["FontSize"].GetFloat();
//        auto clock_text = get_gameObject()->GetComponent<TextMeshProUGUI*>();
//        clock_text->set_fontSize(fontsize);
        //    clock_text->get_transform()->set_position(UnityEngine::Vector3(0+ClockX, 1+ClockY, 12.6+ClockZ));
//}

MAKE_HOOK_OFFSETLESS(AudioTimeSyncController_StartSong, void, AudioTimeSyncController* self, float startTimeOffset) {
    AudioTimeSyncController_StartSong(self, startTimeOffset);

    if (getConfig().config["insong"].GetBool() == false) {
        canvas->get_gameObject()->SetActive(false);
        logger().info("SetActive false");
    }
    auto ClockXOff = getConfig().config["ClockXOffset"].GetFloat();
    auto ClockYOff = getConfig().config["ClockYOffset"].GetFloat();
    auto ClockZOff = getConfig().config["ClockZOffset"].GetFloat();
    layout->get_transform()->set_position(UnityEngine::Vector3(ClockX + ClockXOff, ClockY + ClockYOff, ClockZ + ClockZOff));
    logger().debug("Object Coords", "%g", ClockXOff, ClockYOff, ClockZOff);
}

MAKE_HOOK_OFFSETLESS(SoloFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate, void, SoloFreePlayFlowCoordinator* self, bool firstActivation, bool addedToHierarchy) {
    SoloFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate(self, firstActivation, addedToHierarchy);

    if (getConfig().config["insong"].GetBool() == false) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true");
    }
    layout->get_transform()->set_position(UnityEngine::Vector3(ClockX, ClockY, ClockZ));
}

MAKE_HOOK_OFFSETLESS(PartyFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate, void, PartyFreePlayFlowCoordinator* self, bool firstActivation, bool addedToHierarchy) {
    PartyFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate(self, firstActivation, addedToHierarchy);

    if (getConfig().config["insong"].GetBool() == false) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true");
    }
    layout->get_transform()->set_position(UnityEngine::Vector3(ClockX, ClockY, ClockZ));
}

MAKE_HOOK_OFFSETLESS(PauseMenuManager_ShowMenu, void, PauseMenuManager* self) {
    PauseMenuManager_ShowMenu(self);

    if (getConfig().config["insong"].GetBool() == false) {
        canvas->get_gameObject()->SetActive(true);
        logger().info("SetActive true PauseMenu");
    }
}

MAKE_HOOK_OFFSETLESS(PauseMenuManager_StartResumeAnimation, void, PauseMenuManager* self) {
    PauseMenuManager_StartResumeAnimation(self);

    if (getConfig().config["insong"].GetBool() == false) {
        canvas->get_gameObject()->SetActive(false);
        logger().info("SetActive false PauseMenu");
    }
}

// Multiplayer Lobby Specific Code

//MAKE_HOOK_OFFSETLESS(MultiplayerLobbyController_ActivateMultiplayerLobby, void, MultiplayerLobbyController* self) {
//    MultiplayerLobbyController_ActivateMultiplayerLobby(self);

 //   layout->get_transform()->set_position(UnityEngine::Vector3(0, -0.05, 1.62));
 //   layout->get_transform()->set_localScale(UnityEngine::Vector3(0.35, 0.35, 0.35));
//}

MAKE_HOOK_OFFSETLESS(QuickPlaySetupViewController_DidActivate, void, QuickPlaySetupViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    QuickPlaySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    auto MLobbyVCPosY = UnityEngine::Object::FindObjectOfType<QuickPlaySetupViewController*>()->get_transform()->get_position().y;
    MLobbyVCPosY = MLobbyVCPosY - 1;
//    logger().debug("%g", MLobbyVCPosY);
    layout->get_transform()->set_position(UnityEngine::Vector3(0, MLobbyVCPosY, 1.62));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(0.35, 0.35, 0.35));
}

MAKE_HOOK_OFFSETLESS(ClientLobbySetupViewController_DidActivate, void, ClientLobbySetupViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    ClientLobbySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    auto MLobbyVCPosY = UnityEngine::Object::FindObjectOfType<ClientLobbySetupViewController*>()->get_transform()->get_position().y;
    MLobbyVCPosY = MLobbyVCPosY - 1;
//    logger().debug("%g", MLobbyVCPosY);
    layout->get_transform()->set_position(UnityEngine::Vector3(0, MLobbyVCPosY, 1.62));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(0.35, 0.35, 0.35));
}

MAKE_HOOK_OFFSETLESS(HostLobbySetupViewController_DidActivate, void, HostLobbySetupViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    HostLobbySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

        auto MLobbyVCPosY = UnityEngine::Object::FindObjectOfType<HostLobbySetupViewController*>()->get_transform()->get_position().y;
        MLobbyVCPosY = MLobbyVCPosY - 1;
//        logger().debug("%g", MLobbyVCPosY);
        layout->get_transform()->set_position(UnityEngine::Vector3(0, MLobbyVCPosY, 1.62));
        layout->get_transform()->set_localScale(UnityEngine::Vector3(0.35, 0.35, 0.35));
}

MAKE_HOOK_OFFSETLESS(MultiplayerLobbyController_DeactivateMultiplayerLobby, void, MultiplayerLobbyController* self) {
    MultiplayerLobbyController_DeactivateMultiplayerLobby(self);

    layout->get_transform()->set_position(UnityEngine::Vector3(ClockX, ClockY, ClockZ));
    layout->get_transform()->set_localScale(UnityEngine::Vector3(1.0, 1.0, 1.0));
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo & info) {
    info.id = "clockmod";
    info.version = VERSION;
    modInfo = info;

    getConfig().Load(); // Load the config file
    logger().info("Completed setup!");

    rapidjson::Document::AllocatorType& allocator = getConfig().config.GetAllocator();
    if (!getConfig().config.HasMember("insong")) {
        getConfig().config.AddMember("insong", rapidjson::Value(0).SetBool(true), allocator);
    }
    if (!getConfig().config.HasMember("12Toggle")) {
        getConfig().config.AddMember("12Toggle", rapidjson::Value(0).SetBool(false), allocator);
    }
    if (!getConfig().config.HasMember("SecToggle")) {
        getConfig().config.AddMember("SecToggle", rapidjson::Value(0).SetBool(false), allocator);
    }
    if (!getConfig().config.HasMember("FontSize")) {
        getConfig().config.AddMember("FontSize", rapidjson::Value(0).SetFloat(4), allocator);
    }
    if (!getConfig().config.HasMember("BattToggle")) {
        getConfig().config.AddMember("BattToggle", rapidjson::Value(0).SetBool(false), allocator);
    }
    // Sets Clock Offset in Config
    if (!getConfig().config.HasMember("ClockXOffset")) {
        getConfig().config.AddMember("ClockXOffset", rapidjson::Value(0).SetFloat(0), allocator);
    }
    if (!getConfig().config.HasMember("ClockYOffset")) {
        getConfig().config.AddMember("ClockYOffset", rapidjson::Value(0).SetFloat(0), allocator);
    }
    if (!getConfig().config.HasMember("ClockZOffset")) {
        getConfig().config.AddMember("ClockZOffset", rapidjson::Value(0).SetFloat(0), allocator);
    }
    if (!getConfig().config.HasMember("r")) {
        getConfig().config.AddMember("r", rapidjson::Value().SetFloat(1), getConfig().config.GetAllocator());
    }
    if (!getConfig().config.HasMember("g")) {
        getConfig().config.AddMember("g", rapidjson::Value().SetFloat(1), getConfig().config.GetAllocator());
    }
    if (!getConfig().config.HasMember("b")) {
        getConfig().config.AddMember("b", rapidjson::Value().SetFloat(1), getConfig().config.GetAllocator());
    }
    getConfig().Write();
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    QuestUI::Init();

    logger().info("Installing Clockmod hooks...");

    Logger& hookLogger = logger();

    custom_types::Register::RegisterType<ClockMod::ClockUpdater>();
    custom_types::Register::RegisterType<ClockMod::ClockViewController>();
    QuestUI::Register::RegisterModSettingsViewController<ClockMod::ClockViewController*>(modInfo);

    INSTALL_HOOK_OFFSETLESS(hookLogger, MainMenuViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "MainMenuViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(hookLogger, AudioTimeSyncController_StartSong, il2cpp_utils::FindMethodUnsafe("", "AudioTimeSyncController", "StartSong", 1));
    INSTALL_HOOK_OFFSETLESS(hookLogger, SoloFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate, il2cpp_utils::FindMethodUnsafe("", "SoloFreePlayFlowCoordinator", "SinglePlayerLevelSelectionFlowCoordinatorDidActivate", 2));
    // Added by ELP
    INSTALL_HOOK_OFFSETLESS(hookLogger, PartyFreePlayFlowCoordinator_SinglePlayerLevelSelectionFlowCoordinatorDidActivate, il2cpp_utils::FindMethodUnsafe("", "PartyFreePlayFlowCoordinator", "SinglePlayerLevelSelectionFlowCoordinatorDidActivate", 2));
    INSTALL_HOOK_OFFSETLESS(hookLogger, PauseMenuManager_ShowMenu, il2cpp_utils::FindMethodUnsafe("", "PauseMenuManager", "ShowMenu", 0));
    INSTALL_HOOK_OFFSETLESS(hookLogger, PauseMenuManager_StartResumeAnimation, il2cpp_utils::FindMethodUnsafe("", "PauseMenuManager", "StartResumeAnimation", 0));
//    INSTALL_HOOK_OFFSETLESS(hookLogger, MultiplayerLobbyController_ActivateMultiplayerLobby, il2cpp_utils::FindMethodUnsafe("", "MultiplayerLobbyController", "ActivateMultiplayerLobby", 0));
    // Multiplayer specific Hooks
    INSTALL_HOOK_OFFSETLESS(hookLogger, HostLobbySetupViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "HostLobbySetupViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(hookLogger, ClientLobbySetupViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "ClientLobbySetupViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(hookLogger, QuickPlaySetupViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "QuickPlaySetupViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(hookLogger, MultiplayerLobbyController_DeactivateMultiplayerLobby, il2cpp_utils::FindMethodUnsafe("", "MultiplayerLobbyController", "DeactivateMultiplayerLobby", 0));

    logger().info("Installed all ClockMod hooks!");
}