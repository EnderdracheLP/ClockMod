#include "ClockModConfig.hpp"
#include "ClockUpdater.hpp"
#include "ClockValues.hpp"
#include "ClockViewController.hpp"
using namespace ClockMod;

#include "bsml/shared/BSML-Lite/Creation/Buttons.hpp"
#include "bsml/shared/BSML-Lite/Creation/Settings.hpp"
#include "bsml/shared/BSML-Lite/Creation/Text.hpp"
#include "bsml/shared/BSML/Components/ModalColorPicker.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/macros.hpp"

#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/ViewController.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "TMPro/FontStyles.hpp"
#include "TMPro/TextMeshProUGUI.hpp" // Added for color change

// Unity engine stuff, very important!
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/WaitForSecondsRealtime.hpp"



using namespace UnityEngine;
using namespace HMUI;

#if !defined(DEFINE_TYPE)
#error Custom-types macro missing, make sure you have ran: 'qpm restore' and that you have a compatible version of custom-types
#endif

DEFINE_TYPE(ClockMod, ClockViewController);


namespace ClockMod {

    custom_types::Helpers::Coroutine ClockViewController::UpdateTimeText() {
        while (SettingsOpen) {
            char timeInformation[45];
            strftime(timeInformation, sizeof(timeInformation), "Your Timezone -  %Z    UTC offset -  %z", ClockUpdater::getInstance()->getTimeInfo());

            char UTCtime[40];
            strftime(UTCtime, sizeof(UTCtime), std::string("\r\nCurrent Time in UTC -  " + ClockUpdater::getTimeFormat()).c_str(), ClockUpdater::getInstance()->getTimeInfoUTC());
            //strftime(UTCtime, sizeof(UTCtime), std::string("\r\n Current Time in UTC -  " + ClockUpdater::getTimeFormat()).c_str(), gmtime(ClockUpdater::getInstance()->getRawTime()));
            
            std::string sessionTime = "\nSession Time -  " + ClockUpdater::getTimerString(ClockUpdater::getInstance()->getSessionTimeSeconds());

            if (TimeInfo && SettingsOpen)
                TimeInfo->set_text(std::string(timeInformation) + UTCtime + sessionTime);
            co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSecondsRealtime::New_ctor(0.1));
        }
        co_return;
    }


    void ClockViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        SettingsOpen = true;
        if (firstActivation) {
    //        Config.InSettings = true;
            get_gameObject()->AddComponent<Touchable*>();
            GameObject* container = BSML::Lite::CreateScrollableSettingsContainer(get_transform());
            Transform* parent = container->get_transform();

            ClockUpdater* instance = ClockMod::ClockUpdater::getInstance();
            if (instance) {
                char timeInformation[45];
                std::string timeFormat = "Your Timezone -  %Z\nUTC offset -  %z";
                strftime(timeInformation, sizeof(timeInformation), timeFormat.c_str(), instance->getTimeInfo());
                // We have to specify sizeDelta here otherwise things will overlap
                TimeInfo = BSML::Lite::CreateText(parent, std::string(timeInformation), TMPro::FontStyles::Normal, {0,0}, {0,20});
                // TimeInfo = BSML::Lite::CreateText(parent, std::string(timeInformation), TMPro::FontStyles::Normal);

                ColorPicker = BSML::Lite::CreateColorPickerModal(parent, getModConfig().ClockColor.GetName(), getModConfig().ClockColor.GetValue(),
                    [instance](UnityEngine::Color value) {
                        getModConfig().ClockColor.SetValue(value);
                    },
                    [instance] {
                        instance->SetColor(getModConfig().ClockColor.GetValue());
                    },
                    [instance](UnityEngine::Color value) {
                        instance->SetColor(value);
                    }
                    );
            }

            AddConfigValueDropdownEnum(parent, getModConfig().ClockType, clockTypeStrs);
            AddConfigValueToggle(parent, getModConfig().InSong);
            AddConfigValueToggle(parent, getModConfig().InReplay);
            AddConfigValueToggle(parent, getModConfig().TwelveToggle);
            AddConfigValueToggle(parent, getModConfig().SecToggle);
            AddConfigValueToggle(parent, getModConfig().BattToggle);
            AddConfigValueToggle(parent, getModConfig().RainbowClock);
            AddConfigValueIncrementFloat(parent, getModConfig().FontSize, 1.0f, 0.1f, 1.0f, 5.0f);
            AddConfigValueToggle(parent, getModConfig().ClockPosition);

    //        BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(parent, getModConfig().ClockXOffset, 1, 0.1f, -10.0f, 10.0f)->get_gameObject(), "Offsets the X (Left/Right) Position of the Clock");
    //        BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(parent, getModConfig().ClockYOffset, 1, 0.1f, -10.0f, 10.0f)->get_gameObject(), "Offsets the Y (Up/Down) Position of the Clock");
    //        BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(parent, getModConfig().ClockZOffset, 1, 0.1f, -10.0f, 10.0f)->get_gameObject(), "Offsets the Z (Forward/Backward) Position of the Clock");

            //BeatSaberUI::CreateColorPicker(parent, getModConfig().ClockColor.GetName(), getModConfig().ClockColor.GetValue(),
            //    [](UnityEngine::Color value, GlobalNamespace::ColorChangeUIEventType eventType) {
            //        getModConfig().ClockColor.SetValue(value);
            //    });

            if (ColorPicker)
                BSML::Lite::CreateUIButton(parent, "Change ClockColor", [this] {
                    ColorPicker->modalView->Show();
                    }
                );

        }
        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(UpdateTimeText()));
    }
    void ClockViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
        SettingsOpen = false;
        StopAllCoroutines();
        //Config.InSettings = false;
    }
}