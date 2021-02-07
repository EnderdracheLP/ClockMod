#include "ClockViewController.hpp"
#include "main.hpp"

#include "questui/shared/BeatSaberUI.hpp"

#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/GameObject.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/ScrollView.hpp"
using namespace UnityEngine::UI;
using namespace UnityEngine;
using namespace HMUI;

DEFINE_CLASS(ClockMod::ClockViewController);

void InSongToggle(ClockMod::ClockViewController* parent, bool newValue) {
    getConfig().config["insong"].SetBool(newValue);
}
void FormatToggle(ClockMod::ClockViewController* parent, bool newValue) {
    getConfig().config["12Toggle"].SetBool(newValue);
}
void SecToggle(ClockMod::ClockViewController* parent, bool newValue) {
    getConfig().config["SecToggle"].SetBool(newValue);
}
void SetFontSize(ClockMod::ClockViewController* parent, float newValue) {
getConfig().config["FontSize"].SetFloat(newValue);
}
void BattToggle(ClockMod::ClockViewController* parent, bool newValue) {
    getConfig().config["BattToggle"].SetBool(newValue);
}
//void SetClockXOffset(ClockMod::ClockViewController* parent, float newValue) {
//getConfig().config["ClockXOffset"].SetFloat(newValue);
//}
//void SetClockYOffset(ClockMod::ClockViewController* parent, float newValue) {
//getConfig().config["ClockYOffset"].SetFloat(newValue);
//}
//void SetClockZOffset(ClockMod::ClockViewController* parent, float newValue) {
//getConfig().config["ClockZOffset"].SetFloat(newValue);
//}
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
        QuestUI::IncrementSetting* FontIncrementObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Font Size", 1, 0.1, getConfig().config["FontSize"].GetFloat(), 1.0f, 8.0f, onFontSizeChange);
        QuestUI::BeatSaberUI::AddHoverHint(FontIncrementObject->get_gameObject(), "Changes the Font Size of the Clock (Default: 4)");
        // Show Battery Percentage
        auto onBattSettingChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
            classof(UnityEngine::Events::UnityAction_1<bool>*), this, BattToggle
            );
        UnityEngine::UI::Toggle* BattToggleObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Show Battery Percentage", getConfig().config["BattToggle"].GetBool(), onBattSettingChange);
        QuestUI::BeatSaberUI::AddHoverHint(BattToggleObject->get_gameObject(), "Displays Battery percentage next to the clock.");
        // Change Pos X
//        auto onClockXOffsetChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
//            classof(UnityEngine::Events::UnityAction_1<float>*), this, SetClockXOffset);                                 //              Decimal places   Value Steps                              Min Value   Max Value
//        QuestUI::IncrementSetting* ClockXIncrementObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Clock X Offset", 1, 0.1, getConfig().config["ClockXOffset"].GetFloat(), -10.0f, 10.0f, onClockXOffsetChange);
//        QuestUI::BeatSaberUI::AddHoverHint(ClockXIncrementObject->get_gameObject(), "Offsets the X Position of the Clock");
        // Change Pos Y
//        auto onClockYOffsetChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
//            classof(UnityEngine::Events::UnityAction_1<float>*), this, SetClockYOffset);                                 //              Decimal places   Value Steps                              Min Value   Max Value
//        QuestUI::IncrementSetting* ClockYIncrementObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Clock Y Offset", 1, 0.1, getConfig().config["ClockYOffset"].GetFloat(), -10.0f, 10.0f, onClockYOffsetChange);
//        QuestUI::BeatSaberUI::AddHoverHint(ClockYIncrementObject->get_gameObject(), "Offsets the Y Position of the Clock");
                // Change Pos Z
//        auto onClockZOffsetChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
//            classof(UnityEngine::Events::UnityAction_1<float>*), this, SetClockZOffset);                                 //              Decimal places   Value Steps                              Min Value   Max Value
//        QuestUI::IncrementSetting* ClockZIncrementObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Clock Z Offset", 1, 0.1, getConfig().config["ClockZOffset"].GetFloat(), -10.0f, 10.0f, onClockZOffsetChange);
//        QuestUI::BeatSaberUI::AddHoverHint(ClockZIncrementObject->get_gameObject(), "Offsets the Z Position of the Clock");
    }
}
// Write config
void ClockMod::ClockViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
    getConfig().Write();
}