; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "VDK"
#define MyAppVersion "2.0"
#define MyAppPublisher "BTicino"
#define MyAppURL "http://www.bticino.com"
#define MyAppExeName "bin/vdk.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{7742DF89-B17A-40C5-870F-854782B0A2C7}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppPublisher}\{#MyAppName}
DefaultGroupName={#MyAppPublisher}\{#MyAppName}
AllowNoIcons=yes
OutputDir=.
OutputBaseFilename=VirtualDevelopmentKitSetup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "..\BUILD_REL\MyOpenBagClient\release\MyOpenSimulator.exe"; DestDir: "{app}\bin"; Flags: ignoreversion; DestName:"vdk.exe"
Source: "..\BUILD_REL\libplant\release\libplant.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Qt\4.8.4\bin\QtCore4.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Qt\4.8.4\bin\QtGui4.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Qt\4.8.4\bin\QtNetwork4.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Qt\4.8.4\bin\QtXml4.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Qt\4.8.4\bin\QtXmlPatterns4.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\QtSDK\mingw\bin\libgcc_s_dw2-1.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\QtSDK\mingw\bin\mingwm10.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "..\BUILD_REL\myhome\*"; DestDir: "{app}\myhome"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

