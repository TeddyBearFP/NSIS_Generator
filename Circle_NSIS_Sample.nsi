!include MUI2.nsh
!include x64.nsh
!include WordFunc.nsh

; The default installation directory
InstallDir $PROGRAMFILES\Crystalvue\Circle

!define NAME "Circle"
!define VERSION "1.0.0.2"
!define UNINSTALL "Uninstall.exe"
!define REGCOMPANY "G.C.L."

!define MAIN_FILE "release.zip"
!define DRIVER_0_X64 "DriverX64.zip"
!define DRIVER_0_X86 "DriverX86.zip"
!define COMPANYNAME "Crystalvue"
!define FILEDESCRIPTION "Circle Installer"
!define SHORTCUT "Circle"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "tbICON4_128x128.ico"
!define MUI_UNICON "tbICON4_128x128.ico"
!define APPLICATION_ICON "tbICON4_128x128.ico"
!define MUI_WELCOMEPAGE_TITLE "Welcome Install"
!define MUI_FINISHPAGE_TITLE "The install is completed"

; welcome page
!insertmacro MUI_PAGE_WELCOME
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

RequestExecutionLevel admin

;show the process of installation
ShowInstDetails show

;the installer title name
Name "${FILEDESCRIPTION}"

; The Installer file name
OutFile "${FILEDESCRIPTION}.exe"

;for uninstaller
UninstPage uninstConfirm
UninstPage instfiles
;--------------------------------
;Languages
!insertmacro MUI_LANGUAGE "English"
;--------------------------------
;Version Information
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "${NAME}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "${REGCOMPANY}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Copyright (C) 2020 ${REGCOMPANY} Inc. All rights reserved"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "${NAME} Installer"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${VERSION}"
VIProductVersion "1.0.0.0"
;------------------------------------
Section "Install"

CreateDirectory "$INSTDIR"
SetOutPath $INSTDIR

File ${MAIN_FILE}
ZipDLL::extractall "$INSTDIR\${MAIN_FILE}" "$INSTDIR"
CreateShortCut "$DESKTOP\${SHORTCUT}.lnk" "$INSTDIR\${SHORTCUT}.exe" "" "$INSTDIR\${MUI_ICON}" 

ExecWait '"$INSTDIR\vcredist_x86.exe" /q /norestart'

DetailPrint "Driver Setup..."

${If} ${RunningX64}
SetOutPath "$INSTDIR\DRIVER_0_X64"
File "${DRIVER_0_X64}"
ZipDLL::extractall "$INSTDIR\DRIVER_0_X64\${DRIVER_0_X64}" "$INSTDIR\DRIVER_0_X64"
ExecWait "win7DriverInstall.exe"

${Else}
SetOutPath "$INSTDIR\DRIVER_0_X86"
File "${DRIVER_0_X86}"
ZipDLL::extractall "$INSTDIR\DRIVER_0_X86\${DRIVER_0_X86}" "$INSTDIR\DRIVER_0_X86"
ExecWait "win7DriverInstall.exe"

${EndIf}

; Write the installation path into the registry
WriteRegStr HKLM Software\Circle "Install_Dir" "$INSTDIR"
; Write the uninstall keys for Windows
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Circle" "DisplayName" "${NAME}"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Circle" "DisplayIcon" '"$INSTDIR\${APPLICATION_ICON}$\"'
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Circle" "RegCompany" "${REGCOMPANY}"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Circle" "Publisher" "${REGCOMPANY}"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Circle" "DisplayVersion" "${VERSION}"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Circle" "UninstallString" '"$INSTDIR\${UNINSTALL}$\"'
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Circle" "NoModify" 1
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Circle" "NoRepair" 1
WriteUninstaller "$INSTDIR\${UNINSTALL}"

WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run\${NAME}" "${NAME}" "$INSTDIR\${SHORTCUT}.exe"
WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\${NAME}.exe" "~ RUNASADMIN"

SetOutPath $INSTDIR
RMDir /r $INSTDIR\DRIVER_0_X64
RMDir /r $INSTDIR\DRIVER_0_X86
Delete "$INSTDIR\${MAIN_FILE}"

MessageBox MB_YESNO|MB_ICONQUESTION|MB_TOPMOST "Please reboot computer" IDNO noreboot
Reboot
noreboot:

SectionEnd

Section "Uninstall"

${If} ${RunningX64}
SetOutPath "$INSTDIR\DRIVER_0_X64"
File "${DRIVER_0_X64}"
ZipDLL::extractall "$INSTDIR\DRIVER_0_X64\${DRIVER_0_X64}" "$INSTDIR\DRIVER_0_X64"
ExecWait "win7DriverInstall.exe /S /U Usb_iCamBase64.inf"

${Else}
SetOutPath "$INSTDIR\DRIVER_0_X86"
File "${DRIVER_0_X86}"
ZipDLL::extractall "$INSTDIR\DRIVER_0_X86\${DRIVER_0_X86}" "$INSTDIR\DRIVER_0_X86"
ExecWait "win7DriverInstall.exe /S /U Usb_iCamBase64.inf"

${EndIf}

SetOutPath $PROGRAMFILES
RMDir /r $INSTDIR
Delete "$DESKTOP\${SHORTCUT}.lnk"
DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Circle"

DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Run\${NAME}"

SectionEnd
