#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
using namespace UnityEngine;

DECLARE_CLASS_CODEGEN(ClockMod, ClockUpdater, UnityEngine::MonoBehaviour,

#ifndef DECLARE_METHOD
    DECLARE_INSTANCE_METHOD(void, FixedUpdate);
    DECLARE_INSTANCE_METHOD(void, Start);
#else
    DECLARE_METHOD(void, FixedUpdate);
    DECLARE_METHOD(void, Start);
#endif
#if defined(BS__1_16) && defined(REGISTER_FUNCTION)
    REGISTER_FUNCTION(
#elif defined(BS__1_13_2)
    REGISTER_FUNCTION(ClockUpdater,
#endif
#if defined(REGISTER_METHOD)
        REGISTER_METHOD(FixedUpdate);
        REGISTER_METHOD(Start);
    )
#endif
)