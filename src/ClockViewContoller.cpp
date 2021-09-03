#include "ClockViewController.hpp"
#include "main.hpp"
#include "ClockModConfig.hpp"
#include "ClockUpdater.hpp"
using namespace ClockMod;

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "System/Collections/IEnumerator.hpp"

#include "UnityEngine/WaitForSecondsRealtime.hpp"
//#include "UnityEngine/WaitForSeconds.hpp"

// UnityEngine stuff, very important!!!
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/Resources.hpp"

#include "HMUI/Touchable.hpp"
#include "HMUI/ScrollView.hpp"

#include "TMPro/TextMeshProUGUI.hpp" // Added for color change

//#include "GlobalNamespace/ColorChangeUIEventType.hpp" // Added for Colorpicker
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

namespace ClockMod {

    custom_types::Helpers::Coroutine ClockViewController::UpdateTimeText() {
        while (SettingsOpen) {
            char timeInformation[45];
            strftime(timeInformation, sizeof(timeInformation), "Your Timezone -  %Z    UTC offset -  %z", ClockUpdater::getInstance()->getTimeInfo());
            char UTCtime[40];
            strftime(UTCtime, sizeof(UTCtime), std::string("\r\n Current Time in UTC -  " + ClockUpdater::getTimeFormat()).c_str(), ClockUpdater::getInstance()->getTimeInfoUTC());
            //strftime(UTCtime, sizeof(UTCtime), std::string("\r\n Current Time in UTC -  " + ClockUpdater::getTimeFormat()).c_str(), gmtime(ClockUpdater::getInstance()->getRawTime()));
            if (TimeInfo && SettingsOpen)
                TimeInfo->set_text(il2cpp_utils::newcsstr(std::string(timeInformation) + UTCtime));
            co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSecondsRealtime::New_ctor(0.1));
        }
        co_return;
    }


    void ClockViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        SettingsOpen = true;
        if (firstActivation) {
    //        Config.InSettings = true;
            get_gameObject()->AddComponent<Touchable*>();
            GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
            Transform* parent = container->get_transform();

            ClockUpdater* instance = ClockMod::ClockUpdater::getInstance();
            if (instance) {
                char timeInformation[45];
                std::string timeFormat = "Your Timezone -  %Z\r\nUTC offset -  %z";
                strftime(timeInformation, sizeof(timeInformation), timeFormat.c_str(), instance->getTimeInfo());
                TimeInfo = BeatSaberUI::CreateText(parent, timeInformation, false);

                //lastChangedColor = getModConfig().ClockColor.GetValue();

                ColorPicker = BeatSaberUI::CreateColorPickerModal(parent, getModConfig().ClockColor.GetName(), getModConfig().ClockColor.GetValue(),
                    [instance](UnityEngine::Color value) {
                        //lastChangedColor = value;
                        getModConfig().ClockColor.SetValue(value);
                    },
                    [instance] {
                        instance->SetColor(getModConfig().ClockColor.GetValue());
                        //getModConfig().ClockColor.SetValue(lastChangedColor);
                    },
                    [instance](UnityEngine::Color value) {
                    instance->SetColor(value);
                    //getModConfig().ClockColor.SetValue(value);
                    }
                    );
            }

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

            //BeatSaberUI::CreateColorPicker(parent, getModConfig().ClockColor.GetName(), getModConfig().ClockColor.GetValue(),
            //    [](UnityEngine::Color value, GlobalNamespace::ColorChangeUIEventType eventType) {
            //        getModConfig().ClockColor.SetValue(value);
            //    });

            if (ColorPicker)
                QuestUI::BeatSaberUI::CreateUIButton(parent, "Change ClockColor", [this] {
                    ColorPicker->Show();
                    }
                );

        }
        StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(UpdateTimeText())));
    }
    void ClockViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
        SettingsOpen = false;
        //StopAllCoroutines();
        //Config.InSettings = false;
    }
}