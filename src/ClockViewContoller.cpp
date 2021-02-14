#include "ClockViewController.hpp"
#include "main.hpp"
#include "ClockModConfig.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/Color.hpp"

#include "HMUI/Touchable.hpp"
#include "HMUI/ScrollView.hpp"

#include "TMPro/TextMeshProUGUI.hpp" // Added for color change

#include "GlobalNamespace/ColorChangeUIEventType.hpp" // Added for Colorpicker
using namespace QuestUI;
using namespace UnityEngine;
using namespace HMUI;

DEFINE_CLASS(ClockMod::ClockViewController);

void ClockMod::ClockViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {
        get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        Transform* parent = container->get_transform();

        BeatSaberUI::AddHoverHint(AddConfigValueToggle(parent, getModConfig().InSong)->get_gameObject(), "If the Clock should be shown while playing a Beatmap.");

        BeatSaberUI::AddHoverHint(AddConfigValueToggle(parent, getModConfig().TwelveToggle)->get_gameObject(), "If time should be in 12 or 24 Hour format.");

        BeatSaberUI::AddHoverHint(AddConfigValueToggle(parent, getModConfig().SecToggle)->get_gameObject(), "If Seconds should be displayed.");

        BeatSaberUI::AddHoverHint(AddConfigValueToggle(parent, getModConfig().BattToggle)->get_gameObject(), "Displays Battery percentage next to the clock.");

        BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(parent, getModConfig().FontSize, 1, 0.1f, 1.0f, 5.0f)->get_gameObject(), "If Seconds should be displayed.");

        BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(parent, getModConfig().ClockXOffset, 1, 0.1f, -10.0f, 10.0f)->get_gameObject(), "Offsets the X (Left/Right) Position of the Clock");
        BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(parent, getModConfig().ClockYOffset, 1, 0.1f, -10.0f, 10.0f)->get_gameObject(), "Offsets the Y (Up/Down) Position of the Clock");
        BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(parent, getModConfig().ClockZOffset, 1, 0.1f, -10.0f, 10.0f)->get_gameObject(), "Offsets the Z (Forward/Backward) Position of the Clock");

        BeatSaberUI::AddHoverHint(BeatSaberUI::CreateColorPicker(parent, getModConfig().ClockColor.GetName(), getModConfig().ClockColor.GetValue(),
            [](UnityEngine::Color value, GlobalNamespace::ColorChangeUIEventType eventType) {
                getModConfig().ClockColor.SetValue(value);
            }), "The color of the clock text.");
    }
}