#include "rocktriggercontroller.h"
#include "rocktriggerids.h"

#include "base/source/fstreamer.h"
#include "public.sdk/source/vst/vstparameters.h"
#include "vstgui/plugin-bindings/vst3editor.h"

#include <cstring>

namespace Steinberg::Vst::RockTrigger {

using namespace VSTGUI;

tresult PLUGIN_API RockTriggerController::initialize (FUnknown* context)
{
    const auto result = EditControllerEx1::initialize (context);
    if (result != kResultOk)
        return result;

    auto* sound = new StringListParameter (USTRING ("Sound"), kSoundId);
    sound->appendString (USTRING ("MODERN ROCK"));
    sound->appendString (USTRING ("MODERN METAL"));
    sound->appendString (USTRING ("ALTERNATIVE"));
    sound->appendString (USTRING ("PROGRESSIVE ROCK"));
    sound->appendString (USTRING ("TRAP ROCK"));
    sound->setNormalized (0.0);
    parameters.addParameter (sound);

    auto* threshold = new RangeParameter (
        USTRING ("Threshold"),
        kThresholdId,
        USTRING ("dB"),
        kThresholdMinDb,
        kThresholdMaxDb,
        kThresholdDefaultDb,
        0,
        ParameterInfo::kCanAutomate);
    threshold->setPrecision (1);
    parameters.addParameter (threshold);

    auto* retrigger = new RangeParameter (
        USTRING ("Retrigger"),
        kRetriggerId,
        USTRING ("ms"),
        kRetriggerMinMs,
        kRetriggerMaxMs,
        kRetriggerDefaultMs,
        0,
        ParameterInfo::kCanAutomate);
    retrigger->setPrecision (0);
    parameters.addParameter (retrigger);

    auto* output = new RangeParameter (
        USTRING ("Output"),
        kOutputId,
        USTRING ("dB"),
        kOutputMinDb,
        kOutputMaxDb,
        kOutputDefaultDb,
        0,
        ParameterInfo::kCanAutomate);
    output->setPrecision (1);
    parameters.addParameter (output);

    return result;
}

tresult PLUGIN_API RockTriggerController::setComponentState (IBStream* state)
{
    if (!state)
        return kResultFalse;

    IBStreamer stream (state, kLittleEndian);
    float sound = 0.f;
    float threshold = 0.6f;
    float retrigger = static_cast<float> (kRetriggerDefaultNormalized);
    float output = static_cast<float> (kOutputDefaultNormalized);

    if (!stream.readFloat (sound) || !stream.readFloat (threshold))
        return kResultFalse;

    // Backward compatible with older states.
    stream.readFloat (retrigger);
    stream.readFloat (output);

    setParamNormalized (kSoundId, sound);
    setParamNormalized (kThresholdId, threshold);
    setParamNormalized (kRetriggerId, retrigger);
    setParamNormalized (kOutputId, output);
    return kResultOk;
}

IPlugView* PLUGIN_API RockTriggerController::createView (FIDString name)
{
    if (name && std::strcmp (name, ViewType::kEditor) == 0)
        return new VST3Editor (this, "view", "rocktrigger.uidesc");
    return nullptr;
}

} // namespace Steinberg::Vst::RockTrigger
