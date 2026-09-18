#define MyAppName "SuperPowers"
#define MyAppVersion "1.0.2"
#define MyAppPublisher "Gary Este"

[Setup]
AppId={{8A221B31-C7C1-4FA0-9A1B-92A29A5046E7}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={localappdata}\Programs\Common\VST3
DisableDirPage=yes
DisableProgramGroupPage=yes
PrivilegesRequired=lowest
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
OutputDir=output
OutputBaseFilename=SuperPowers-v{#MyAppVersion}-Windows-x64-Setup
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
UninstallDisplayName=SuperPowers VST3

[Dirs]
Name: "{userdocs}\RockTrigger Samples"

[Files]
Source: "..\dist\SuperPowers.vst3\*"; DestDir: "{localappdata}\Programs\Common\VST3\SuperPowers.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\samples\README.txt"; DestDir: "{userdocs}\RockTrigger Samples"; Flags: ignoreversion onlyifdoesntexist
Source: "..\samples\*.wav"; DestDir: "{userdocs}\RockTrigger Samples"; Flags: ignoreversion skipifsourcedoesntexist

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    Log('SuperPowers installed. Rescan VST3 plug-ins in the DAW if necessary.');
  end;
end;
