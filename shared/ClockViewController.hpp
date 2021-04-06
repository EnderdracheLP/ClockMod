#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"
//#include "questui/shared/BeatSaberUI.hpp"
//#include "TMPro/TextMeshProUGUI.hpp"
//#include "UnityEngine/Color.hpp"

DECLARE_CLASS_CODEGEN(ClockMod, ClockViewController, HMUI::ViewController,

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
//    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);

    REGISTER_FUNCTION(ClockViewController,
        REGISTER_METHOD(DidActivate);
//        REGISTER_METHOD(DidDeactivate);
    )
);