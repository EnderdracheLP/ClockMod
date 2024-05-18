#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "HMUI/ViewController.hpp"
#include "UnityEngine/Color.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "bsml/shared/BSML/Components/ModalColorPicker.hpp"
//#include "questui/shared/BeatSaberUI.hpp"
//#include "TMPro/TextMeshProUGUI.hpp"

DECLARE_CLASS_CODEGEN(ClockMod, ClockViewController, HMUI::ViewController,

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);

    DECLARE_INSTANCE_FIELD(BSML::ModalColorPicker*, ColorPicker);
    DECLARE_INSTANCE_FIELD(HMUI::CurvedTextMeshPro*, TimeInfo);

    custom_types::Helpers::Coroutine UpdateTimeText();
    bool SettingsOpen;

    //UnityEngine::Color lastChangedColor;
);