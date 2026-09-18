#include "rocktriggercontroller.h"
#include "rocktriggerids.h"
#include "rocktriggerprocessor.h"

#include "public.sdk/source/main/pluginfactory_constexpr.h"

#define stringPluginName "SuperPowers"
#define stringCompanyName "Gary Este"
#define stringCompanyWeb ""
#define stringCompanyEmail ""

BEGIN_FACTORY_DEF (stringCompanyName, stringCompanyWeb, stringCompanyEmail, 2)

DEF_CLASS (Steinberg::Vst::RockTrigger::kProcessorUID,
           Steinberg::PClassInfo::kManyInstances,
           kVstAudioEffectClass,
           stringPluginName,
           Steinberg::Vst::kDistributable,
           "Fx|Tools",
           "1.0.2",
           kVstVersionString,
           Steinberg::Vst::RockTrigger::RockTriggerProcessor::createInstance,
           nullptr)

DEF_CLASS (Steinberg::Vst::RockTrigger::kControllerUID,
           Steinberg::PClassInfo::kManyInstances,
           kVstComponentControllerClass,
           stringPluginName " Controller",
           0,
           "",
           "1.0.2",
           kVstVersionString,
           Steinberg::Vst::RockTrigger::RockTriggerController::createInstance,
           nullptr)

END_FACTORY
