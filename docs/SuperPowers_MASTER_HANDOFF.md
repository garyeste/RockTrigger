# SuperPowers VST3 — MASTER HANDOFF / DEVELOPMENT INSTRUCTIONS

Date updated: 2026-09-19
Project: SuperPowers VST3
Repository: https://github.com/garyeste/RockTrigger
Platform: Windows x64
DAW confirmed by user: Studio One
Current public-facing version: 1.0.2

## Project goal

SuperPowers is a VST3 audio effect for drum/audio replacement. It is inserted directly on an audio track, analyzes the incoming signal as a trigger source, and replaces the original sound with one selected sample.

Factory sample categories:
- MODERN ROCK
- MODERN METAL
- ALTERNATIVE
- PROGRESSIVE ROCK
- TRAP ROCK

The old project name was RockTrigger. The user-facing product name, GUI, installer and VST3 bundle are now SuperPowers. Internal namespaces/files may still use RockTrigger where changing them is unnecessary.

## User workflow priority

The user wants:
- no manual Visual Studio work;
- no local compilation;
- GitHub Actions to produce the Windows build;
- a normal VST3 effect;
- a Windows EXE installer;
- future revisions delivered as ready-to-install artifacts.

Always prefer editing the existing GitHub project and building through Actions instead of giving source-only instructions.

## Current working DSP

The DSP behavior is confirmed working by the user.

- audio input is the trigger source;
- original input is replaced by the selected sample;
- mono and stereo are supported;
- up to 12 overlapping voices;
- new hits do not automatically cut earlier sample tails;
- sample loading/resampling happens outside process();
- no disk I/O or blocking operations should be added to process();
- missing selected sample currently falls back to safe source passthrough.

Parameters:
- Sound ParamID 100
- Threshold ParamID 101
- Retrigger ParamID 102
- Output ParamID 103

Ranges:
- Threshold: -60 dB to 0 dB, default -24 dB
- Retrigger: 10–500 ms, default 35 ms
- Output: -24 dB to +24 dB, default 0 dB

Sound values:
- MODERN ROCK
- MODERN METAL
- ALTERNATIVE
- PROGRESSIVE ROCK
- TRAP ROCK

Do not arbitrarily change ParamIDs because existing DAW projects/automation may depend on them.

State compatibility is important. Older states without Retrigger/Output must load safely with defaults.

## VST3 architecture rules

Canonical source: official Steinberg VST3 SDK / Developer Portal.

- Processor: IAudioProcessor / IComponent
- Controller: IEditController
- GUI: VSTGUI
- Processor must work independently from GUI.
- process() is realtime.
- No filesystem I/O in process().
- Avoid allocations in process().
- Avoid mutexes/blocking locks in process().
- GUI telemetry must be transferred safely outside the realtime audio path.

## Approved GUI direction

Dark premium audio-plugin interface.

Must preserve:
- exact user-provided GE logo;
- do not redesign the GE logo;
- GE logo at upper left;
- beside it:
  DRUMS
  BIGGER
  HARDER
  CLEANER
- centered product title: SuperPowers
- SuperPowers title is white and gothic/elegant;
- top right shows only: v 1.0.2
- no “SAMPLE REPLACER” text;
- no PREVIEW button;
- waveform should run automatically whenever audio is playing;
- black/dark overall theme;
- premium hardware/plugin feel.

The rejected Current/Neural-style experiment must not be restored.

## Current GUI layout

Top:
- GE logo
- DRUMS / BIGGER / HARDER / CLEANER
- SuperPowers
- v 1.0.2

Main:
- 5-way Sound selector
- Threshold knob
- Retrigger knob
- Output knob
- IN meter
- OUT meter
- INPUT waveform
- TRIGGERED waveform

Bottom currently includes sample information and legacy LOAD / BROWSE / RESET visual area. Do not present controls as functional unless they actually are.

## Important failed GUI version

The first v1.0.2 graphical build had major problems:
- IN meter was a static PNG;
- OUT meter was a static PNG;
- waveform was a static image;
- none showed actual DSP/audio;
- meters overlapped the waveform area;
- SuperPowers title bitmap rendered corrupted/garbled in Studio One;
- some bitmap/button styling was lost.

Do not restore that implementation.

## Realtime GUI hotfix — confirmed working

The realtime hotfix was built, installed, and the user explicitly confirmed: “Все отлично выполнелось.”

This is now the known-good baseline.

Hotfix behavior:
- IN meter reads actual input peak telemetry from DSP;
- OUT meter reads actual output peak telemetry;
- INPUT waveform receives realtime input samples;
- TRIGGERED waveform receives realtime trigger/replacement activity;
- GUI refresh occurs on the UI side, not directly from process();
- meters were resized/repositioned to avoid waveform overlap;
- title/button/knob assets were rebuilt to avoid corrupted host rendering.

This architecture must be preserved.

## Realtime telemetry implementation

Hotfix introduced custom VSTGUI views and DSP-to-GUI telemetry.

Relevant source includes:
- source/telemetry.h
- source/customviews.h
- source/customviews.cpp
- updated rocktriggercontroller.*
- updated rocktriggerprocessor.*
- updated resource files

Do not replace these custom views with CView + static bitmap placeholders.

If realtime visuals are changed, preserve:
- thread safety;
- no direct UI calls from process();
- no blocking audio-thread operations;
- stable buffer ownership;
- UI-side redraw/update behavior.

## GitHub source of truth

Repository:
https://github.com/garyeste/RockTrigger

Branch:
main

Before future work:
1. inspect current main;
2. inspect latest successful GitHub Actions run;
3. modify the existing project;
4. do not start from scratch.

The repo uses payload/override/hotfix reconstruction in Actions. Always inspect `.github/workflows/build-windows.yml` before editing because a file changed only in reconstructed `work/` is not necessarily the source of truth.

## Build workflow

Workflow:
.github/workflows/build-windows.yml

Name:
Build SuperPowers Windows VST3 + Installer

Runner:
windows-2022

Stack:
- CMake
- Visual Studio 2022 toolchain on GitHub Actions
- Steinberg VST3 SDK
- VSTGUI
- Inno Setup

Outputs:
- SuperPowers.vst3
- SuperPowers-v1.0.2-Windows-x64-Setup.exe

## Last confirmed successful realtime-hotfix build

Known-good commit:
781a0bbe5702e44bc970bde7fade3def51d84da1

Commit message:
Remove duplicate hotfix application step

GitHub Actions run:
35378591454

Run number:
27

Result:
success

Artifacts:
- SuperPowers-v1.0.2-VST3-Windows-x64
- SuperPowers-v1.0.2-Windows-x64-Setup

The user confirmed this build works correctly in Studio One.

THIS IS THE CURRENT KNOWN-GOOD BASELINE.

## Hotfix payload

Repo folder:
hotfix/

The successful workflow uses the following ordered parts:
- superpowers-hotfix.part00.b64
- superpowers-hotfix.part01.b64
- superpowers-hotfix.part02.b64
- superpowers-hotfix.part03a.b64
- superpowers-hotfix.part03b1.b64
- superpowers-hotfix.part03b2.b64
- superpowers-hotfix.part04.b64
- superpowers-hotfix.part05.b64
- superpowers-hotfix.part06.b64

There are obsolete/intermediate hotfix parts in the folder. Use only the ordered list referenced by the current successful workflow. Do not concatenate every file in the folder.

## Build history / compatibility fixes already solved

Keep these fixes unless there is a specific reason to change them:
- Steinberg DECLARE_UID compatibility;
- current SDK string-macro compatibility;
- NOMINMAX before Windows headers;
- VSTGUI custom-view compatibility;
- removal of obsolete setTransparent(true) call;
- verified hotfix payload reconstruction order;
- removal of duplicate hotfix application step.

## Sample folder

Current Windows folder:
%USERPROFILE%\Documents\RockTrigger Samples\

Files:
- modern_rock.wav
- modern_metal.wav
- alternative.wav
- progressive_rock.wav
- trap_rock.wav

The product is now SuperPowers, but sample loading still uses the legacy RockTrigger Samples folder. Do not rename it without migration support in both loader and installer.

## Test process for future builds

1. Fully close Studio One.
2. Install the new build over the existing version.
3. Reopen Studio One.
4. Rescan VST3 if needed.
5. Insert SuperPowers on an audio/drum track.
6. Play audio.
7. Verify IN meter.
8. Verify INPUT waveform.
9. Lower Threshold enough to trigger.
10. Verify TRIGGERED waveform.
11. Verify OUT meter.
12. Test all five Sound choices.
13. Test Retrigger.
14. Test Output.
15. Save/reopen a Studio One project to verify state persistence.

Closing the DAW matters because the old DLL can remain loaded.

## Current confirmed status

Confirmed by user:
- VST3 loads in Studio One;
- installer/build pipeline works;
- sample-replacement DSP works;
- GUI loads;
- IN realtime meter works;
- OUT realtime meter works;
- realtime waveform works;
- realtime hotfix installs successfully;
- user called the result “Все отлично выполнелось.”

Future work should therefore be refinement/enhancement, not a rewrite.

## Good next priorities

Possible future work:
- polish Sound selector;
- improve premium knob appearance;
- improve panel depth, shadows, highlights;
- refine typography and spacing;
- improve meter ballistics/peak hold;
- improve waveform styling;
- trigger markers;
- threshold line;
- waveform time scale;
- per-sample output trim;
- velocity-sensitive sample level;
- detector HPF/LPF;
- transient sensitivity;
- trigger offset/phase timing;
- original/replacement Mix control;
- round-robin variants;
- drag-and-drop samples;
- real LOAD/BROWSE/RESET actions;
- presets.

Do not add features just because they are common. User should approve behavior/UI changes.

## Design rules

1. Preserve exact GE logo.
2. Do not redraw GE unless explicitly requested.
3. SuperPowers remains white unless user changes direction.
4. Keep only version text in upper-right.
5. Keep DRUMS / BIGGER / HARDER / CLEANER near GE.
6. Do not reintroduce SAMPLE REPLACER.
7. Do not reintroduce PREVIEW.
8. Waveform runs automatically.
9. No fake/static functional meters.
10. No fake/static realtime waveform.
11. Functional UI must reflect real DSP state.
12. Prefer stable custom drawing over fragile bitmap stretching where appropriate.
13. Test bitmap dimensions/alpha behavior.
14. Avoid control overlap at 100% scaling.
15. Prioritize Studio One on Windows.

## Versioning

Current displayed version:
1.0.2

For the next meaningful public iteration:
- 1.0.3 for fixes/polish
- 1.1.0 for meaningful new functionality

Keep version synchronized across:
- plugin metadata;
- CMake;
- installer;
- GUI label;
- artifact names.

## Do not do

Do not:
- start a fresh VST project;
- replace working DSP without reason;
- remove realtime telemetry;
- replace active meters/waveforms with static PNGs;
- change ParamIDs;
- block realtime audio;
- do disk I/O in process();
- redesign the GE logo;
- rename sample folders without migration;
- assume successful compilation means Studio One rendering is correct;
- claim visual behavior is verified until user tests it.

## First steps in a future session

1. Read this file.
2. Open `garyeste/RockTrigger`.
3. Inspect `main`.
4. Inspect `.github/workflows/build-windows.yml`.
5. Confirm known-good run 35378591454.
6. Use commit 781a0bbe5702e44bc970bde7fade3def51d84da1 as functional baseline.
7. Make targeted changes only.
8. Push/build through GitHub Actions.
9. Inspect logs and fix failures.
10. Download successful installer/VST3 artifacts.
11. Give the user ready-to-install files.
12. Ask for a Studio One screenshot when visual verification is needed.

## Working style

The user is actively designing and testing the plugin. Preferred workflow:
- concrete;
- visual;
- iterative;
- minimal theory;
- make the change;
- compile it;
- give installer;
- user tests in Studio One;
- iterate from screenshots/feedback.

When the user says “сделай сборку”, do not merely provide code. Build through GitHub Actions whenever access is available.

## Stable baseline statement

SuperPowers v1.0.2 realtime-hotfix from GitHub Actions run 35378591454 is the first build in this design iteration that the user confirmed works correctly.

Treat it as the stable baseline for all future SuperPowers development.
