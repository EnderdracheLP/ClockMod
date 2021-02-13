#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/Color.hpp"

DECLARE_CLASS_CODEGEN(ClockMod, ClockViewController, HMUI::ViewController,

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);

    DECLARE_METHOD(void, ClockColor);
    DECLARE_METHOD(void, ClockSize);
//    DECLARE_METHOD(void, ClockOffset);

    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, redSetting);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, greenSetting);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, blueSetting);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, FontSizeSetting);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, clocktext);
    DECLARE_INSTANCE_FIELD(UnityEngine::Color, clock_color);

    REGISTER_FUNCTION(ClockViewController,
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(DidDeactivate);
        REGISTER_METHOD(ClockColor);
        REGISTER_METHOD(ClockSize);
        REGISTER_FIELD(redSetting);
        REGISTER_FIELD(greenSetting);
        REGISTER_FIELD(blueSetting);
        REGISTER_FIELD(FontSizeSetting);
        REGISTER_FIELD(clocktext);
        REGISTER_FIELD(clock_color);
        // TODO: Get Clock Offest implemented here (I'm too lazy right now)
//        REGISTER_FIELD(ClockXOff);
//        REGISTER_FIELD(ClockYOff);
//        REGISTER_FIELD(ClockZOff);
    )

);