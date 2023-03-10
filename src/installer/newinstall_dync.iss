; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Notepad--"
#define MyAppVersion "1.23.2"
#define MyAppDesc MyAppName + MyAppVersion + "(Ndd主程序)"
#define MyAppPublisher "ndd开源组织"
#define MyAppURL "https://gitee.com/cxasm/notepad--"
#define MyAppExeName "Notepad--.exe"
#define MyAppAssocName "nddfile"
#define MyAppAssocExt ".txt"
#define MyAppAssocKey StringChange(MyAppAssocName, " ", "") + MyAppAssocExt

#define MyAppAssocExt1 ".ini"
#define MyAppAssocExt2 ".log"
#define MyAppAssocExt3 ".sh"
#define MyAppAssocExt4 ".h"
#define MyAppAssocExt5 ".c"
#define MyAppAssocExt6 ".cpp"
#define MyAppAssocExt7 ".py"
#define MyAppAssocExt8 ".xml"
#define MyAppAssocExt9 ".def"
#define MyAppAssocExt10 ".json"
#define MyAppAssocExt11 ".pro"
#define MyAppAssocExt12 ".csv"


[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{FA6189F1-03B8-44A2-BE8E-F6CD8E7857B6}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
ArchitecturesInstallIn64BitMode=x64
DefaultDirName={autopf}\{#MyAppName}
ChangesAssociations=yes
DisableProgramGroupPage=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
OutputDir=D:\CCNotePad\installer
OutputBaseFilename=Notepad--v1.22.0-Installer
SetupIconFile=D:\CCNotePad\Resources\edit\global\ndd.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern


[Languages]
Name: "ChineseSimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "D:\CCNotePad\x64\Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\CCNotePad\x64\Release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Registry]
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue; Components: main
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt1}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue ; Components: ass
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt2}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue; Components: ass
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt3}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue; Components: ass
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt4}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue; Components: ass
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt5}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue; Components: ass
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt6}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue; Components: ass
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt7}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue; Components: ass
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt8}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue; Components: ass
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt9}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue; Components: ass
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt10}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue; Components: ass
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt11}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue; Components: ass

Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}"; ValueType: string; ValueName: ""; ValueData: "{#MyAppAssocName}"; Flags: uninsdeletekey; Components: main
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\{#MyAppExeName},0"; Components: main
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1"""; Components: main

Root: HKA; Subkey: "Software\Classes\Applications\{#MyAppExeName}"; ValueType: string; ValueName: ""; ValueData: ""; Flags: uninsdeletekey; Components: main
Root: HKA; Subkey: "Software\Classes\Applications\{#MyAppExeName}\SupportedTypes"; ValueType: string; ValueName: ".myp"; ValueData: ""; Flags: uninsdeletekey; Components: main
;Root: HKA; Subkey: "Software\Classes\Applications\{#MyAppExeName}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1"""; Flags: uninsdeletekey; Components: main
Root: HKA; Subkey: "Software\Classes\*\shell\Notepad--"; ValueType: string; ValueName: ""; ValueData: "Edit with Notepad--"; Flags: uninsdeletekey; Components: main
Root: HKA; Subkey: "Software\Classes\*\shell\Notepad--"; ValueType: string; ValueName: "Icon"; ValueData: """{app}\{#MyAppExeName}""" ; Components: main
Root: HKA; Subkey: "Software\Classes\*\shell\Notepad--\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1""" ; Components: main



[Components]
Name: "main"; Description: "{#MyAppDesc}"; Types: full compact custom; Flags: fixed
Name: "ass"; Description: "关联打开常见文本类型"; Types: full


[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

