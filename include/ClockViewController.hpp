#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ViewController.hpp"
#include "bsml/shared/BSML/Components/ModalColorPicker.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(ClockMod, ClockViewController, HMUI::ViewController) {

    DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD_MATCH(void, DidDeactivate, &HMUI::ViewController::DidDeactivate, bool removedFromHierarchy, bool systemScreenDisabling);

    DECLARE_INSTANCE_FIELD(BSML::ModalColorPicker*, ColorPicker);
    DECLARE_INSTANCE_FIELD(HMUI::CurvedTextMeshPro*, TimeInfo);
    DECLARE_INSTANCE_FIELD(UnityEngine::Coroutine*, timeInfoRoutine);

    custom_types::Helpers::Coroutine UpdateTimeText();
    bool SettingsOpen;
};
