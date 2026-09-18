#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace Steinberg::Vst::RockTrigger {

static const FUID kProcessorUID (0xF5DFA5DC, 0x94C4461F, 0x80391E58, 0x150063F1);
static const FUID kControllerUID (0x71713D93, 0x192A465B, 0xB9DE9A96, 0x960DD4EF);

enum ParamIds : ParamID
{
    kSoundId = 100,
    kThresholdId = 101,
    kRetriggerId = 102,
    kOutputId = 103,
};

constexpr int32 kNumSounds = 5;

constexpr double kThresholdMinDb = -60.0;
constexpr double kThresholdMaxDb = 0.0;
constexpr double kThresholdDefaultDb = -24.0;

constexpr double kRetriggerMinMs = 10.0;
constexpr double kRetriggerMaxMs = 500.0;
constexpr double kRetriggerDefaultMs = 35.0;
constexpr double kRetriggerDefaultNormalized =
    (kRetriggerDefaultMs - kRetriggerMinMs) / (kRetriggerMaxMs - kRetriggerMinMs);

constexpr double kOutputMinDb = -24.0;
constexpr double kOutputMaxDb = 24.0;
constexpr double kOutputDefaultDb = 0.0;
constexpr double kOutputDefaultNormalized =
    (kOutputDefaultDb - kOutputMinDb) / (kOutputMaxDb - kOutputMinDb);

} // namespace Steinberg::Vst::RockTrigger
