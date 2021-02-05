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

void ClockMod::ClockViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {

        get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        // Show Clock during Song Play
        UnityEngine::Events::UnityAction_1<bool>* onInSongSettingChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
            classof(UnityEngine::Events::UnityAction_1<bool>*), this, InSongToggle
            );
        UnityEngine::UI::Toggle* InsongToggleObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Show During Song", getConfig().config["insong"].GetBool(), onInSongSettingChange);
        QuestUI::BeatSaberUI::AddHoverHint(InsongToggleObject->get_gameObject(), "Whether or not to show the Clock while playing a Beatmap.");
        // Change format 24/12
        UnityEngine::Events::UnityAction_1<bool>* onFormatSettingChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
            classof(UnityEngine::Events::UnityAction_1<bool>*), this, FormatToggle
            );
        UnityEngine::UI::Toggle* FormatToggleObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "24/12 Toggle", getConfig().config["12Toggle"].GetBool(), onFormatSettingChange);
        QuestUI::BeatSaberUI::AddHoverHint(FormatToggleObject->get_gameObject(), "Whether to show time in 24 Hour or 12 Hour format.");
        // Show Seconds
        UnityEngine::Events::UnityAction_1<bool>* onSecSettingChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
            classof(UnityEngine::Events::UnityAction_1<bool>*), this, SecToggle
            );
        UnityEngine::UI::Toggle* SecToggleObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Show Seconds", getConfig().config["SecToggle"].GetBool(), onSecSettingChange);
        QuestUI::BeatSaberUI::AddHoverHint(SecToggleObject->get_gameObject(), "Whether or not to show seconds.");
    }
}
// Write config
void ClockMod::ClockViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
    getConfig().Write();
}
