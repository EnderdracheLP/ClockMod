#include "ClockViewController.hpp"
#include "main.hpp"

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
using namespace UnityEngine::UI;
using namespace UnityEngine;
using namespace HMUI;
using namespace TMPro; // Added for color change


DEFINE_CLASS(ClockMod::ClockViewController);

// Part of Color Setting Code - Copy and Paste from original project
QuestUI::IncrementSetting* redSetting;
QuestUI::IncrementSetting* greenSetting;
QuestUI::IncrementSetting* blueSetting;
QuestUI::IncrementSetting* FontSizeSetting;

TextMeshProUGUI* clocktext;
UnityEngine::Color clock_color;

void InSongToggle(ClockMod::ClockViewController* parent, bool newValue) {
    getConfig().config["insong"].SetBool(newValue);
}
void FormatToggle(ClockMod::ClockViewController* parent, bool newValue) {
    getConfig().config["12Toggle"].SetBool(newValue);
}
void SecToggle(ClockMod::ClockViewController* parent, bool newValue) {
    getConfig().config["SecToggle"].SetBool(newValue);
}
//void SetFontSize(ClockMod::ClockViewController* parent, float newValue) {
//getConfig().config["FontSize"].SetFloat(newValue);
//}
void SetFontSize(ClockMod::ClockViewController* parent, float newValue) {
        getConfig().config["FontSize"].SetFloat(newValue);
    parent->ClockSize();
}
void BattToggle(ClockMod::ClockViewController* parent, bool newValue) {
    getConfig().config["BattToggle"].SetBool(newValue);
}

// Code for Clock Offset
void SetClockXOffset(ClockMod::ClockViewController* parent, float newValue) {
getConfig().config["ClockXOffset"].SetFloat(newValue);
}
void SetClockYOffset(ClockMod::ClockViewController* parent, float newValue) {
getConfig().config["ClockYOffset"].SetFloat(newValue);
}
void SetClockZOffset(ClockMod::ClockViewController* parent, float newValue) {
getConfig().config["ClockZOffset"].SetFloat(newValue);
}

// Code for color change
void SetRed(ClockMod::ClockViewController* parent, float newValue) {
    if (newValue < 0) {
        parent->redSetting->CurrentValue = 0;
        parent->redSetting->UpdateValue();
        getConfig().config["r"].SetFloat(0);

        parent->clock_color.r = 0;
    }
    if (newValue > 1) {
        parent->redSetting->CurrentValue = 1;
        parent->redSetting->UpdateValue();
        getConfig().config["r"].SetFloat(1);

        parent->clock_color.r = 1;
    }
    else {
        getConfig().config["r"].SetFloat(newValue);

        parent->clock_color.r = newValue;
    }
    parent->ClockColor();
}

void SetGreen(ClockMod::ClockViewController* parent, float newValue) {
    if (newValue < 0) {
        parent->greenSetting->CurrentValue = 0;
        parent->greenSetting->UpdateValue();
        getConfig().config["g"].SetFloat(0);

        parent->clock_color.g = 0;
    }
    if (newValue > 1) {
        parent->greenSetting->CurrentValue = 1;
        parent->greenSetting->UpdateValue();
        getConfig().config["g"].SetFloat(1);

        parent->clock_color.g = 1;
    }
    else {
        getConfig().config["g"].SetFloat(newValue);

        parent->clock_color.r = newValue;
    }
    parent->ClockColor();
}

void SetBlue(ClockMod::ClockViewController* parent, float newValue) {
    if (newValue < 0) {
        parent->blueSetting->CurrentValue = 0;
        parent->blueSetting->UpdateValue();
        getConfig().config["b"].SetFloat(0);

        parent->clock_color.b = 0;
    }
    if (newValue > 1) {
        parent->blueSetting->CurrentValue = 1;
        parent->blueSetting->UpdateValue();
        getConfig().config["b"].SetFloat(1);

        parent->clock_color.b = 1;
    }
    else {
        getConfig().config["b"].SetFloat(newValue);

        parent->clock_color.b = newValue;
    }
    parent->ClockColor();
}
void ClockMod::ClockViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {

        get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        // Show Clock during Song Play
        auto onInSongSettingChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
            classof(UnityEngine::Events::UnityAction_1<bool>*), this, InSongToggle
            );
        UnityEngine::UI::Toggle* InsongToggleObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Show During Song", getConfig().config["insong"].GetBool(), onInSongSettingChange);
        QuestUI::BeatSaberUI::AddHoverHint(InsongToggleObject->get_gameObject(), "If the Clock should be shown while playing a Beatmap.");
        // Change format 24/12
        auto onFormatSettingChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
            classof(UnityEngine::Events::UnityAction_1<bool>*), this, FormatToggle
            );
        UnityEngine::UI::Toggle* FormatToggleObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "24/12 Toggle", getConfig().config["12Toggle"].GetBool(), onFormatSettingChange);
        QuestUI::BeatSaberUI::AddHoverHint(FormatToggleObject->get_gameObject(), "If time should be in 12 or 24 Hour format.");
        // Show Seconds
        auto onSecSettingChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
            classof(UnityEngine::Events::UnityAction_1<bool>*), this, SecToggle
            );
        UnityEngine::UI::Toggle* SecToggleObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Show Seconds", getConfig().config["SecToggle"].GetBool(), onSecSettingChange);
        QuestUI::BeatSaberUI::AddHoverHint(SecToggleObject->get_gameObject(), "If Seconds should be displayed.");
        // Change Size
        auto onFontSizeChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*), this, SetFontSize);                                 //              Decimal places   Value Steps                              Min Value   Max Value
        QuestUI::IncrementSetting* FontIncrementObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Font Size", 1, 0.1, getConfig().config["FontSize"].GetFloat(), 1.0f, 5.0f, onFontSizeChange);
        QuestUI::BeatSaberUI::AddHoverHint(FontIncrementObject->get_gameObject(), "Changes the Font Size of the Clock (Default: 4)");
        // Show Battery Percentage
        auto onBattSettingChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
            classof(UnityEngine::Events::UnityAction_1<bool>*), this, BattToggle
            );
        UnityEngine::UI::Toggle* BattToggleObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Show Battery Percentage", getConfig().config["BattToggle"].GetBool(), onBattSettingChange);
        QuestUI::BeatSaberUI::AddHoverHint(BattToggleObject->get_gameObject(), "Displays Battery percentage next to the clock.");
        // Clock Offset Code
        // Change Pos X
        auto onClockXOffsetChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*), this, SetClockXOffset);                                 //              Decimal places   Value Steps                              Min Value   Max Value
        QuestUI::IncrementSetting* ClockXIncrementObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "In Song Clock X Offset", 1, 0.1, getConfig().config["ClockXOffset"].GetFloat(), -10.0f, 10.0f, onClockXOffsetChange);
        QuestUI::BeatSaberUI::AddHoverHint(ClockXIncrementObject->get_gameObject(), "Offsets the X (Left/Right) Position of the Clock");
        // Change Pos Y
        auto onClockYOffsetChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*), this, SetClockYOffset);                                 //              Decimal places   Value Steps                              Min Value   Max Value
        QuestUI::IncrementSetting* ClockYIncrementObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "In Song Clock Y Offset", 1, 0.1, getConfig().config["ClockYOffset"].GetFloat(), -10.0f, 10.0f, onClockYOffsetChange);
        QuestUI::BeatSaberUI::AddHoverHint(ClockYIncrementObject->get_gameObject(), "Offsets the Y (Up/Down) Position of the Clock");
                // Change Pos Z
        auto onClockZOffsetChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*), this, SetClockZOffset);                                 //              Decimal places   Value Steps                              Min Value   Max Value
        QuestUI::IncrementSetting* ClockZIncrementObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "In Song Clock Z Offset", 1, 0.1, getConfig().config["ClockZOffset"].GetFloat(), -10.0f, 10.0f, onClockZOffsetChange);
        QuestUI::BeatSaberUI::AddHoverHint(ClockZIncrementObject->get_gameObject(), "Offsets the Z (Forward/Backward) Position of the Clock");
        // Code for setting Color
        auto redChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*), this, SetRed
            );
        auto greenChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*), this, SetGreen
            );
        auto blueChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*), this, SetBlue
            );

        clocktext = UnityEngine::GameObject::Find(il2cpp_utils::createcsstr("Canvas"))->GetComponentInChildren<TextMeshProUGUI*>();
        clock_color = clocktext->m_Color;
        clock_color.r = getConfig().config["r"].GetFloat();
        clock_color.g = getConfig().config["g"].GetFloat();
        clock_color.b = getConfig().config["b"].GetFloat();
        clocktext->set_color(clock_color);

        redSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Red", 2, 0.05, getConfig().config["r"].GetFloat(), redChange);
        greenSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Green", 2, 0.05, getConfig().config["g"].GetFloat(), greenChange);
        blueSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Blue", 2, 0.05, getConfig().config["b"].GetFloat(), blueChange);

    }
}
// Write config
void ClockMod::ClockViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
    getConfig().Write();
}

void ClockMod::ClockViewController::ClockColor() {
    clock_color.r = getConfig().config["r"].GetFloat();
    clock_color.g = getConfig().config["g"].GetFloat();
    clock_color.b = getConfig().config["b"].GetFloat();
    clocktext->set_color(clock_color);
}

void ClockMod::ClockViewController::ClockSize() {
    clocktext->set_fontSize(getConfig().config["FontSize"].GetFloat());
}