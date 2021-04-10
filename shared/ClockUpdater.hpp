#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
using namespace UnityEngine;

DECLARE_CLASS_CODEGEN(ClockMod, ClockUpdater, UnityEngine::MonoBehaviour,

    DECLARE_METHOD(void, Update);

    REGISTER_FUNCTION(
        REGISTER_METHOD(Update);
    )
)