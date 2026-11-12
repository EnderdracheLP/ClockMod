#pragma once
#include "TMPro/TextMeshProUGUI.hpp"  // Text Stuff.
#include "UnityEngine/GameObject.hpp" // For GameObject Stuff
#include "UnityEngine/Gradient.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"  // For Transform stuff
#include "custom-types/shared/macros.hpp"
#include "beatsaber-hook/shared/safeptr.hpp"
#include "cordl_internals/unity-utils.hpp"
#include <ctime>
#include <time.h>

using namespace UnityEngine;

DECLARE_CLASS_CODEGEN(ClockMod, ClockUpdater, UnityEngine::MonoBehaviour) {

    DECLARE_DEFAULT_CTOR();
    DECLARE_SIMPLE_DTOR();
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, OnDestroy);

private:
    DECLARE_INSTANCE_FIELD(UnityW<TMPro::TextMeshProUGUI>, text);
    DECLARE_INSTANCE_FIELD(UnityW<UnityEngine::Transform>, clockParent);
    DECLARE_INSTANCE_FIELD(UnityEngine::Gradient*, batteryGradient);
    double lastSessionTimeSeconds = 0;
    double nextTextUpdateTime = 0;
    double nextStopwatchSaveTime = 0;
    double nextBatteryUpdateTime = 0;
    std::string cachedBatteryText;

    time_t rawtime = 0;
    struct tm localTime{};
    struct tm utcTime{};
    struct tm* timeinfo = nullptr;

    double sessionTimeSeconds = 0;
    double stopwatch1Seconds = 0;
    double stopwatch2Seconds = 0;

    static safe_ptr<ClockUpdater*, true> instance;

    std::string _message;
    int messageShowing = 0;

public:
    const time_t getRawTime() const;
    struct tm* getTimeInfo();
    struct tm* getTimeInfoUTC();
    const double getSessionTimeSeconds() const;
    const double getStopwatch1Seconds() const;
    const double getStopwatch2Seconds() const;
    void resetStopwatch1();
    void resetStopwatch2();
    TMPro::TextMeshProUGUI* getTextMesh();
    void SetColor(UnityEngine::Color color);
    static ClockUpdater* getInstance();
    static std::string getTimeFormat();
    static std::string getStopwatchString(const double totalSeconds);
    void ShowMessage(std::string message, int duration = 4);

};
