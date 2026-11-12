#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ViewController.hpp"
#include "bsml/shared/BSML/Components/ModalColorPicker.hpp"
#include "custom-types/shared/macros.hpp"
#include "cordl_internals/unity-utils.hpp"

DECLARE_CLASS_CODEGEN(ClockMod, ClockViewController, HMUI::ViewController) {

    DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD_MATCH(void, DidDeactivate, &HMUI::ViewController::DidDeactivate, bool removedFromHierarchy, bool systemScreenDisabling);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, OnDisable);

    DECLARE_INSTANCE_FIELD(UnityW<BSML::ModalColorPicker>, ColorPicker);
    DECLARE_INSTANCE_FIELD(UnityW<HMUI::CurvedTextMeshPro>, TimeInfo);

    void UpdateTimeText();
    bool SettingsOpen = false;
    float nextTimeUpdate = 0;
};
