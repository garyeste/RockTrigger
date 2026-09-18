#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "samplebank.h"

#include <array>
#include <cstddef>
#include <cstdint>

namespace Steinberg::Vst::RockTrigger {

class RockTriggerProcessor final : public AudioEffect
{
public:
    RockTriggerProcessor ();
    ~RockTriggerProcessor () override = default;

    static FUnknown* createInstance (void*) { return static_cast<IAudioProcessor*> (new RockTriggerProcessor ()); }

    tresult PLUGIN_API initialize (FUnknown* context) override;
    tresult PLUGIN_API setBusArrangements (SpeakerArrangement* inputs, int32 numIns,
                                           SpeakerArrangement* outputs, int32 numOuts) override;
    tresult PLUGIN_API setupProcessing (ProcessSetup& setup) override;
    tresult PLUGIN_API setProcessing (TBool state) override;
    tresult PLUGIN_API canProcessSampleSize (int32 symbolicSampleSize) override;
    tresult PLUGIN_API process (ProcessData& data) override;
    tresult PLUGIN_API setState (IBStream* state) override;
    tresult PLUGIN_API getState (IBStream* state) override;
    uint32 PLUGIN_API getTailSamples () override;

private:
    struct Voice
    {
        bool active {false};
        int sampleIndex {0};
        size_t position {0};
        uint64_t serial {0};
    };

    template <typename SampleType>
    tresult processAudio (ProcessData& data);

    void applyParameterChanges (IParameterChanges* changes);
    void updateRetriggerSamples () noexcept;
    void trigger ();
    void resetRuntime ();
    bool anyVoiceActive () const noexcept;

    SampleBank sampleBank_;
    std::array<Voice, 12> voices_ {};
    uint64_t voiceSerial_ {1};

    double sampleRate_ {44100.0};
    double soundNormalized_ {0.0};
    double thresholdNormalized_ {0.6};
    double retriggerNormalized_ {0.0};
    double outputNormalized_ {0.5};
    int selectedSound_ {0};
    float thresholdLinear_ {0.063095734f}; // -24 dB
    float outputGain_ {1.f};
    float envelope_ {0.f};
    float releaseCoeff_ {0.f};
    int cooldownSamples_ {0};
    int cooldownRemaining_ {0};
    bool armed_ {true};
};

} // namespace Steinberg::Vst::RockTrigger
