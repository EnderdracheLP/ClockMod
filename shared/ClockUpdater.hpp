#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include <ctime>
#include <time.h>
using namespace UnityEngine;

DECLARE_CLASS_CODEGEN(ClockMod, ClockUpdater, UnityEngine::MonoBehaviour,

#ifndef DECLARE_METHOD
    DECLARE_INSTANCE_METHOD(void, Update);
#else
    DECLARE_METHOD(void, Update);
#endif

private:
    double time_counter = 0;

    clock_t this_time = clock();
    clock_t last_time = this_time;

    time_t rawtime;
    struct tm* timeinfo;


public:
    time_t getRawTime();
    struct tm* getTimeInfo();

#if defined(BS__1_16) && defined(REGISTER_FUNCTION)
    REGISTER_FUNCTION(
#elif defined(BS__1_13_2)
    REGISTER_FUNCTION(ClockUpdater,
#endif
#if defined(REGISTER_METHOD)
        REGISTER_METHOD(Update);
    )
#endif
)