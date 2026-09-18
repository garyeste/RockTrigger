cmake_minimum_required(VERSION 3.25.0)

project(SuperPowers
    VERSION 1.0.2.0
    DESCRIPTION "SuperPowers drum sample replacement VST3"
)

include(FetchContent)

set(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SMTG_ENABLE_VST3_HOSTING_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SMTG_ENABLE_VSTGUI_SUPPORT ON CACHE BOOL "" FORCE)
set(SMTG_CREATE_PLUGIN_LINK OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    vst3sdk
    GIT_REPOSITORY https://github.com/steinbergmedia/vst3sdk.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    GIT_SUBMODULES_RECURSE TRUE
)
FetchContent_MakeAvailable(vst3sdk)

smtg_enable_vst3_sdk()

smtg_add_vst3plugin(SuperPowers
    source/rocktriggerids.h
    source/rocktriggerprocessor.h
    source/rocktriggerprocessor.cpp
    source/rocktriggercontroller.h
    source/rocktriggercontroller.cpp
    source/rocktriggerentry.cpp
    source/samplebank.h
    source/samplebank.cpp
)

target_link_libraries(SuperPowers PRIVATE sdk vstgui_support)
target_compile_features(SuperPowers PRIVATE cxx_std_17)

smtg_target_add_plugin_resources(SuperPowers
    RESOURCES
        resource/rocktrigger.uidesc
        resource/ge_logo.png
        resource/superpowers_title.png
        resource/knob_face.png
        resource/waveform_panel.png
        resource/meter.png
)

if(SMTG_WIN)
    target_link_libraries(SuperPowers PRIVATE shell32 ole32)
endif()

smtg_target_configure_version_file(SuperPowers)
