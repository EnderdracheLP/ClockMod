#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include <ctime>
#include <time.h>
#include "TMPro/TextMeshProUGUI.hpp"                // Text Stuff.
#include "UnityEngine/Transform.hpp"                // For Transform stuff
#include "UnityEngine/GameObject.hpp"               // For GameObject Stuff
using namespace UnityEngine;

DECLARE_CLASS_CODEGEN(ClockMod, ClockUpdater, UnityEngine::MonoBehaviour,

#ifndef DECLARE_METHOD
    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_INSTANCE_METHOD(void, Update);
#else
    DECLARE_METHOD(void, Start);
    DECLARE_METHOD(void, Update);
#endif

private:
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, text);
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, clockParent);
    double time_counter = 0;

    clock_t this_time = clock();
    clock_t last_time = this_time;

    time_t rawtime;
    struct tm* timeinfo;

    static ClockUpdater* instance;

    std::string _message;
    int messageShowing = 2;

public:
    const time_t getRawTime() const;
    struct tm* getTimeInfo();
    struct tm* getTimeInfoUTC();
    TMPro::TextMeshProUGUI* getTextMesh();
    void SetColor(UnityEngine::Color color);
    static ClockUpdater* getInstance();
    static std::string getTimeFormat();
    void ShowMessage(std::string message);

)