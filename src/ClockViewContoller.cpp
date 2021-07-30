#include "ClockViewController.hpp"
#include "main.hpp"
#include "ClockModConfig.hpp"
#include "ClockUpdater.hpp"
using namespace ClockMod;

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

// UnityEngine stuff, very important!!!
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Resources.hpp"

#include "HMUI/Touchable.hpp"
#include "HMUI/ScrollView.hpp"

#include "TMPro/TextMeshProUGUI.hpp" // Added for color change

#include "GlobalNamespace/ColorChangeUIEventType.hpp" // Added for Colorpicker
using namespace QuestUI;
using namespace UnityEngine;
using namespace HMUI;

#ifndef REGISTER_FUNCTION
DEFINE_TYPE(ClockMod, ClockViewController);
#elif defined(DEFINE_TYPE)
DEFINE_TYPE(ClockMod::ClockViewController);
#elif defined(DEFINE_CLASS)
DEFINE_CLASS(ClockMod::ClockViewController);
#endif


void ClockMod::ClockViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {
//        Config.InSettings = true;
        get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        Transform* parent = container->get_transform();

        char timeInformation[40];
        strftime(timeInformation, sizeof(timeInformation), "Your Timezone  %Z\nUTC offset  %z", UnityEngine::Resources::FindObjectsOfTypeAll<ClockUpdater*>()->values[0]->getTimeInfo());
        BeatSaberUI::CreateText(parent, timeInformation, false);


        AddConfigValueToggle(parent, getModConfig().InSong);
        AddConfigValueToggle(parent, getModConfig().TwelveToggle);
        AddConfigValueToggle(parent, getModConfig().SecToggle);
        AddConfigValueToggle(parent, getModConfig().BattToggle);
        AddConfigValueToggle(parent, getModConfig().RainbowClock);
        AddConfigValueIncrementFloat(parent, getModConfig().FontSize, 1.0f, 0.1f, 1.0f, 5.0f);
        AddConfigValueToggle(parent, getModConfig().ClockPosition);

//        BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(parent, getModConfig().ClockXOffset, 1, 0.1f, -10.0f, 10.0f)->get_gameObject(), "Offsets the X (Left/Right) Position of the Clock");
//        BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(parent, getModConfig().ClockYOffset, 1, 0.1f, -10.0f, 10.0f)->get_gameObject(), "Offsets the Y (Up/Down) Position of the Clock");
//        BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(parent, getModConfig().ClockZOffset, 1, 0.1f, -10.0f, 10.0f)->get_gameObject(), "Offsets the Z (Forward/Backward) Position of the Clock");

        BeatSaberUI::CreateColorPicker(parent, getModConfig().ClockColor.GetName(), getModConfig().ClockColor.GetValue(),
            [](UnityEngine::Color value, GlobalNamespace::ColorChangeUIEventType eventType) {
                getModConfig().ClockColor.SetValue(value);
            });
    }
}
//void ClockMod::ClockViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
//    Config.InSettings = false;
//}