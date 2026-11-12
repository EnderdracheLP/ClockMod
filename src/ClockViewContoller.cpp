#include "ClockModConfig.hpp"
#include "ClockUpdater.hpp"
#include "ClockValues.hpp"
#include "ClockFormatting.hpp"
#include "ClockViewController.hpp"

#include "bsml/shared/BSML-Lite/Creation/Buttons.hpp"
#include "bsml/shared/BSML-Lite/Creation/Settings.hpp"
#include "bsml/shared/BSML-Lite/Creation/Text.hpp"
#include "custom-types/shared/delegate.hpp"
#include "HMUI/Touchable.hpp"
#include "TMPro/FontStyles.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

using namespace UnityEngine;
using namespace HMUI;

DEFINE_TYPE(ClockMod, ClockViewController);

namespace ClockMod {
    void ClockViewController::UpdateTimeText() {
        if (!SettingsOpen || !TimeInfo) return;
        auto instance = ClockUpdater::getInstance();
        if (!instance) {
            TimeInfo->set_text("Clock unavailable");
            return;
        }
        auto timeInformation = FormatTime(instance->getTimeInfo(), "Your Timezone -  %Z    UTC offset -  %z");
        auto utcTime = FormatTime(instance->getTimeInfoUTC(), "\r\nCurrent Time in UTC -  " + ClockUpdater::getTimeFormat());
        auto sessionTime = "\nSession Time -  " + ClockUpdater::getStopwatchString(instance->getSessionTimeSeconds());
        auto stopwatch1Time = "\nStopwatch 1 Time -  " + ClockUpdater::getStopwatchString(instance->getStopwatch1Seconds());
        auto stopwatch2Time = "\nStopwatch 2 Time -  " + ClockUpdater::getStopwatchString(instance->getStopwatch2Seconds());
        TimeInfo->set_text(timeInformation + utcTime + sessionTime + stopwatch1Time + stopwatch2Time);
    }

    void ClockViewController::Update() {
        if (!SettingsOpen) return;
        auto now = Time::get_unscaledTime();
        if (now < nextTimeUpdate) return;
        nextTimeUpdate = now + 0.1f;
        UpdateTimeText();
    }

    void ClockViewController::OnDisable() {
        SettingsOpen = false;
    }

    void ClockViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        SettingsOpen = true;
        if (firstActivation) {
            get_gameObject()->AddComponent<Touchable*>();
            auto container = BSML::Lite::CreateScrollableSettingsContainer(get_transform());
            auto parent = container->get_transform();
            TimeInfo = BSML::Lite::CreateText(parent, "", TMPro::FontStyles::Normal, {0,0}, {0,5*6});

            ColorPicker = BSML::Lite::CreateColorPickerModal(parent, getModConfig().ClockColor.GetName(), getModConfig().ClockColor.GetValue(),
                [](UnityEngine::Color value) {
                    getModConfig().ClockColor.SetValue(value);
                },
                [] {
                    if (auto instance = ClockUpdater::getInstance()) instance->SetColor(getModConfig().ClockColor.GetValue());
                },
                [](UnityEngine::Color value) {
                    if (auto instance = ClockUpdater::getInstance()) instance->SetColor(value);
                });

            auto stopwatch1PauseButton = BSML::Lite::CreateUIButton(parent, getModConfig().Stopwatch1Paused.GetValue() ? "Start" : "Pause", {70, -19.9}, {-5, 0}, nullptr);
            stopwatch1PauseButton->GetComponent<UI::LayoutElement*>()->set_ignoreLayout(true);
            stopwatch1PauseButton->get_transform()->set_localScale({0.9, 0.9, 0.9});
            // Keep callback targets in managed fields that the GC can trace.
            stopwatch1PauseButton->get_onClick()->AddListener(custom_types::MakeDelegate<Events::UnityAction*>(stopwatch1PauseButton,
                std::function<void(UI::Button*)>([](UI::Button* button) {
                    if (!UnityW(button)) return;
                    getModConfig().Stopwatch1Paused.SetValue(!getModConfig().Stopwatch1Paused.GetValue());
                    BSML::Lite::SetButtonText(button, getModConfig().Stopwatch1Paused.GetValue() ? "Start" : "Pause");
                })));
            auto stopwatch1ResetButton = BSML::Lite::CreateUIButton(parent, "Reset", {83.5, -19.9}, {-5, 0}, nullptr);
            stopwatch1ResetButton->GetComponent<UI::LayoutElement*>()->set_ignoreLayout(true);
            stopwatch1ResetButton->get_transform()->set_localScale({0.9, 0.9, 0.9});
            stopwatch1ResetButton->get_onClick()->AddListener(custom_types::MakeDelegate<Events::UnityAction*>(std::function<void()>([] {
                if (auto instance = ClockUpdater::getInstance()) instance->resetStopwatch1();
            })));

            auto stopwatch2PauseButton = BSML::Lite::CreateUIButton(parent, getModConfig().Stopwatch2Paused.GetValue() ? "Start" : "Pause", {70, -25.5}, {-5, 0}, nullptr);
            stopwatch2PauseButton->GetComponent<UI::LayoutElement*>()->set_ignoreLayout(true);
            stopwatch2PauseButton->get_transform()->set_localScale({0.9, 0.9, 0.9});
            stopwatch2PauseButton->get_onClick()->AddListener(custom_types::MakeDelegate<Events::UnityAction*>(stopwatch2PauseButton,
                std::function<void(UI::Button*)>([](UI::Button* button) {
                    if (!UnityW(button)) return;
                    getModConfig().Stopwatch2Paused.SetValue(!getModConfig().Stopwatch2Paused.GetValue());
                    BSML::Lite::SetButtonText(button, getModConfig().Stopwatch2Paused.GetValue() ? "Start" : "Pause");
                })));
            auto stopwatch2ResetButton = BSML::Lite::CreateUIButton(parent, "Reset", {83.5, -25.5}, {-5, 0}, nullptr);
            stopwatch2ResetButton->GetComponent<UI::LayoutElement*>()->set_ignoreLayout(true);
            stopwatch2ResetButton->get_transform()->set_localScale({0.9, 0.9, 0.9});
            stopwatch2ResetButton->get_onClick()->AddListener(custom_types::MakeDelegate<Events::UnityAction*>(std::function<void()>([] {
                if (auto instance = ClockUpdater::getInstance()) instance->resetStopwatch2();
            })));

            AddConfigValueDropdownEnum(parent, getModConfig().ClockType, clockTypeStrs);
            AddConfigValueToggle(parent, getModConfig().InSong);
            AddConfigValueToggle(parent, getModConfig().InReplay);
            AddConfigValueToggle(parent, getModConfig().TwelveToggle);
            AddConfigValueToggle(parent, getModConfig().SecToggle);
            AddConfigValueToggle(parent, getModConfig().BattToggle);
            AddConfigValueToggle(parent, getModConfig().RainbowClock);
            AddConfigValueIncrementFloat(parent, getModConfig().FontSize, 1.0f, 0.1f, 1.0f, 5.0f);
            AddConfigValueToggle(parent, getModConfig().ClockPosition);

            if (ColorPicker) {
                BSML::Lite::CreateUIButton(parent, "Change ClockColor", [this] {
                    ColorPicker->modalView->Show();
                });
            }
        }
        nextTimeUpdate = 0;
        UpdateTimeText();
    }

    void ClockViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
        SettingsOpen = false;
    }
}
