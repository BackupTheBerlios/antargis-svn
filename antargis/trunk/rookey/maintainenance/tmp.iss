; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=Antargis
AppVerName=Antargis 0.3
DefaultDirName={pf}\Antargis
DefaultGroupName=Antargis
UninstallDisplayIcon={app}\Antargis.exe
Compression=lzma/fast
SolidCompression=yes
OutputBaseFilename=Antargis-0.3-setup
AppPublisher=Antargis-Team
AppPublisherURL=antargis.berlios.de
AppVersion=0.3


[Files]
Source: "antargis-0.3\*"; DestDir: "{app}"; Flags:recursesubdirs

[Icons]
Name: "{group}\Antargis"; Filename: "{app}\Antargis.lnk" ; WorkingDir: "{app}"

[Languages]
Name: en; MessagesFile: "compiler:Default.isl"
;Name: es; MessagesFile: "compiler:Languages\Spanish.isl"
Name: de; MessagesFile: "compiler:Languages\German.isl"

[UninstallDelete]
Type: files; Name: "{app}\Antargis.lnk"
Type: files; Name: "{app}\error.txt"
Type: dirifempty; Name: "{app}"

[Code]
function NextButtonClick(CurPageID: Integer): Boolean;
var
  ResultCode: Integer;
begin
  case CurPageID of
    wpSelectDir:
      begin
        CreateDir(ExpandConstant('{app}'));
        CreateShellLink(
        ExpandConstant('{app}\Antargis.lnk'),
        'Antargis 0.3',
        ExpandConstant('{app}\antargis.exe'),
        ExpandConstant('"{app}\Antargis"'),
        ExpandConstant('{app}'),
          '',
          0,
          SW_SHOWNORMAL);
      end;
  end;

  Result := True;
end;


procedure AfterMyProgInstall(S: String);
begin
  MsgBox('AfterMyProgInstall:' #13#13 'Setup just installed ' + S + ' as ' + CurrentFileName + '.', mbInformation, MB_OK);
end;

