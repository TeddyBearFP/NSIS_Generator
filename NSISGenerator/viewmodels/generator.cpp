#include "generator.h"
#include "models/db.h"
#include "tblog.h"
#include <QStandardPaths>

Generator::Generator()
{
    // ***** Open database ***** //
    if ( DB::Inst()->connectToDataBase() )
    {
        DB::Inst()->moveToThread( QApplication::instance()->thread() );

        QVariantList paramList = DB::Inst()->getAllFromNSISParams();

        for ( int idx = 0; idx < paramList.size(); idx++ )
        {
            SystemParamItem *param = qvariant_cast<SystemParamItem *>( paramList.at( idx ) );
            qDebug() << param->value();
            if ( param->name() == "ProgramName" )
                m_programName = param->value();
            if ( param->name() == "Icon" )
                m_iconName = param->value();
            if ( param->name() == "Company" )
                m_company = param->value();
            if ( param->name() == "ProgramReleaseFile" )
                m_mainFile = param->value();
            if ( param->name() == "Version" )
                m_version = param->value();
        }
    }
    else
    {
        m_programName = "TBProgram2";
        m_version = "1.0.0.2";
        m_mainFile = "release2.zip";
        m_company = "G.C.L2.";
        m_iconName = "tbICON3_128x1282.ico";
        Logger( LOGWARN, "***** Cannot open database *****" );
    }

    setHowManyDrivers( 1 );

    m_welcomePage = true;
    m_directoryPage = true;
    m_instfilesPage = true;
    m_finishPage = true;
    m_vcredist_x86 = true;
    m_autoOpenProgram = true;
}

Generator::~Generator()
{
}

void Generator::setNsisPage( int page, bool checked )
{
    switch ( page )
    {
        case 0:
            m_welcomePage = checked;
            break;
        case 1:
            m_directoryPage = checked;
            break;
        case 2:
            m_instfilesPage = checked;
            break;
        case 3:
            m_finishPage = checked;
            break;
        case 4:
            m_vcredist_x86 = checked;
            break;
        case 5:
            m_autoOpenProgram = checked;
            break;
        case 6:
            m_askAutoReboot = checked;
            break;
        case 7:
            m_highestAuthority = checked;
            break;
    }
}

QString Generator::makeNsis()
{
    QString path = QStandardPaths::writableLocation( QStandardPaths::DesktopLocation ) + "/" + programName() + "_NSIS.nsi";

    QFile qFile( path );
    QTextStream out( &qFile );
    step0( &out );
    stepInstall( &out );
    stepUninstall( &out );
    if ( qFile.open( QIODevice::WriteOnly ) )
    {
        qFile.close();
    }
    return "OK";
}

bool Generator::step0( QTextStream *out )
{
    *out << QString( "!include MUI2.nsh\n" );
    *out << QString( "!include x64.nsh\n" );
    *out << QString( "!include WordFunc.nsh\n\n" );
    *out << QString( "; The default installation directory\n" );
    *out << QString( "InstallDir $PROGRAMFILES\\GCLPrograms\\" + programName() + "\n\n" );
    *out << QString( "!define NAME \"" + programName() + "\"\n" );
    *out << QString( "!define VERSION \"" + version() + "\"\n" );
    *out << QString( "!define UNINSTALL \"Uninstall.exe\"\n" );
    *out << QString( "!define REGCOMPANY \"" + company() + "\"\n\n" );
    *out << QString( "!define MAIN_FILE \"" + mainFile() + "\"\n" );
    QString driverName, x64_driver, x86_driver;

    for ( int i = 0; i < howManyDrivers(); i++ )
    {
        if ( !( m_driverX64List[ i ] == "" && m_driverX86List[ i ] == "" ) )
        {
            if ( m_driverX64List[ i ] != "" && m_driverX86List[ i ] != "" )
            {
                x64_driver = m_driverX64List[ i ].split( ";" )[ 0 ];
                x86_driver = m_driverX86List[ i ].split( ";" )[ 0 ];
                *out << QString( "!define DRIVER_" + QString::number( i ) + "_X64 \"" + x64_driver + "\"\n" );
                *out << QString( "!define DRIVER_" + QString::number( i ) + "_X86 \"" + x86_driver + "\"\n" );
            }
            else
            {
                if ( m_driverX64List[ i ] != "" )
                    driverName = m_driverX64List[ i ].split( ";" )[ 0 ];
                else
                    driverName = m_driverX86List[ i ].split( ";" )[ 0 ];
                *out << QString( "!define DRIVER_" + QString::number( i ) + " \"" + driverName + "\"\n" );
            }
        }
    }

    *out << QString( "!define COMPANYNAME \"G.C.L.\"\n" );
    *out << QString( "!define FILEDESCRIPTION \"" + programName() + " Installer\"\n" );
    *out << QString( "!define SHORTCUT \"" + programName() + "\"\n\n" );
    *out << QString( "; MUI Settings\n" );
    *out << QString( "!define MUI_ABORTWARNING\n" );
    *out << QString( "!define MUI_ICON \"" + iconName() + "\"\n" );
    *out << QString( "!define MUI_UNICON \"" + iconName() + "\"\n" );

    if ( iconName() != "" )
    {
        *out << QString( "!define APPLICATION_ICON \"" + iconName() + "\"\n" );
    }

    *out << QString( "!define MUI_WELCOMEPAGE_TITLE \"Welcome Install\"\n" );
    *out << QString( "!define MUI_FINISHPAGE_TITLE \"The install is completed\"\n\n" );

    *out << QString( "; welcome page\n" );
    if ( m_welcomePage )
        *out << QString( "!insertmacro MUI_PAGE_WELCOME\n" );
    else
        *out << QString( ";!insertmacro MUI_PAGE_WELCOME\n" );
    *out << QString( "; Directory page\n" );
    if ( m_directoryPage )
        *out << QString( "!insertmacro MUI_PAGE_DIRECTORY\n" );
    else
        *out << QString( ";!insertmacro MUI_PAGE_DIRECTORY\n" );
    *out << QString( "; Instfiles page\n" );
    if ( m_instfilesPage )
        *out << QString( "!insertmacro MUI_PAGE_INSTFILES\n" );
    else
        *out << QString( ";!insertmacro MUI_PAGE_INSTFILES\n" );
    *out << QString( "; Finish page\n" );
    if ( m_finishPage )
        *out << QString( "!insertmacro MUI_PAGE_FINISH\n\n" );
    else
        *out << QString( ";!insertmacro MUI_PAGE_FINISH\n\n" );

    *out << QString( "RequestExecutionLevel admin\n\n" );
    *out << QString( ";show the process of installation\n" );
    *out << QString( "ShowInstDetails show\n\n" );
    *out << QString( ";the installer title name\n" );
    *out << QString( "Name \"${FILEDESCRIPTION}\"\n\n" );
    *out << QString( "; The Installer file name\n" );
    *out << QString( "OutFile \"${FILEDESCRIPTION}.exe\"\n\n" );
    *out << QString( ";for uninstaller\n" );
    *out << QString( "UninstPage uninstConfirm\n" );
    *out << QString( "UninstPage instfiles\n" );
    *out << QString( ";--------------------------------\n" );
    *out << QString( ";Languages\n" );
    *out << QString( "!insertmacro MUI_LANGUAGE \"English\"\n" );
    *out << QString( ";--------------------------------\n" );
    *out << QString( ";Version Information\n" );
    *out << QString( "VIAddVersionKey /LANG=${LANG_ENGLISH} \"ProductName\" \"${NAME}\"\n" );
    *out << QString( "VIAddVersionKey /LANG=${LANG_ENGLISH} \"CompanyName\" \"${REGCOMPANY}\"\n" );
    *out << QString( "VIAddVersionKey /LANG=${LANG_ENGLISH} \"LegalCopyright\" \"Copyright (C) 2020 ${REGCOMPANY} Inc. All rights reserved\"\n" );
    *out << QString( "VIAddVersionKey /LANG=${LANG_ENGLISH} \"FileDescription\" \"${NAME} Installer\"\n" );
    *out << QString( "VIAddVersionKey /LANG=${LANG_ENGLISH} \"FileVersion\" \"${VERSION}\"\n" );

    *out << QString( "VIProductVersion \"1.0.0.0\"\n" );
    *out << QString( ";------------------------------------\n" );

    return true;
}

bool Generator::stepInstall( QTextStream *out )
{
    *out << QString( "Section \"Install\"\n\n" );
    *out << QString( "CreateDirectory \"$INSTDIR\"\n" );
    *out << QString( "SetOutPath $INSTDIR\n\n" );
    *out << QString( "File ${MAIN_FILE}\n" );
    *out << QString( "ZipDLL::extractall \"$INSTDIR\\${MAIN_FILE}\" \"$INSTDIR\"\n" );

    if ( iconName() != "" )
        *out << QString( "CreateShortCut \"$DESKTOP\\${SHORTCUT}.lnk\" \"$INSTDIR\\${SHORTCUT}.exe\" \"\" \"$INSTDIR\\${APPLICATION_ICON}\" \n\n" );
    else
        *out << QString( "CreateShortCut \"$DESKTOP\\${SHORTCUT}.lnk\" \"$INSTDIR\\${SHORTCUT}.exe\"\n\n" );

    if ( m_vcredist_x86 )
        *out << QString( "ExecWait '\"$INSTDIR\\vcredist_x86.exe\" /q /norestart'\n\n" );

    if ( howManyDrivers() > 0 )
        *out << QString( "DetailPrint \"Driver Setup...\"\n\n" );

    QString drivers_Inf, x64_driver, x86_driver;

    for ( int i = 0; i < howManyDrivers(); i++ )
    {
        if ( !( m_driverX64List[ i ] == "" && m_driverX86List[ i ] == "" ) )
        {
            if ( m_driverX64List[ i ] != "" && m_driverX86List[ i ] != "" )
            {
                x64_driver = m_driverX64List[ i ].split( ";" )[ 0 ];
                x86_driver = m_driverX86List[ i ].split( ";" )[ 0 ];
                *out << QString( "${If} ${RunningX64}\n" );
                *out << diverInstallStr( "DRIVER_" + QString::number( i ) + "_X64", m_driverX64List[ i ].split( ";" )[ 1 ], true, m_usesWin7Installer[ i ] );
                *out << QString( "${Else}\n" );
                *out << diverInstallStr( "DRIVER_" + QString::number( i ) + "_X86", m_driverX86List[ i ].split( ";" )[ 1 ], true, m_usesWin7Installer[ i ] );
                *out << QString( "${EndIf}\n\n" );
            }
            else
            {
                if ( m_driverX64List[ i ] != "" )
                    *out << diverInstallStr( "DRIVER_" + QString::number( i ), m_driverX64List[ i ].split( ";" )[ 1 ], true, m_usesWin7Installer[ i ] );
                else
                    *out << diverInstallStr( "DRIVER_" + QString::number( i ), m_driverX86List[ i ].split( ";" )[ 1 ], true, m_usesWin7Installer[ i ] );
            }
        }
    }

    *out << QString( "; Write the installation path into the registry\n" );
    *out << QString( "WriteRegStr HKLM Software\\" + programName() + " \"Install_Dir\" " + "\"$INSTDIR\"\n" );
    *out << QString( "; Write the uninstall keys for Windows\n" );
    *out << QString( "WriteRegStr HKLM \"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + programName() + "\" \"DisplayName\" \"${NAME}\"\n" );
    *out << QString( "WriteRegStr HKLM \"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + programName() + "\" \"DisplayIcon\" " + "'\"$INSTDIR\\${APPLICATION_ICON}$\\\"'\n" );
    *out << QString( "WriteRegStr HKLM \"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + programName() + "\" \"RegCompany\" \"${REGCOMPANY}\"\n" );
    *out << QString( "WriteRegStr HKLM \"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + programName() + "\" \"Publisher\" \"${REGCOMPANY}\"\n" );
    *out << QString( "WriteRegStr HKLM \"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + programName() + "\" \"DisplayVersion\" \"${VERSION}\"\n" );
    *out << QString( "WriteRegStr HKLM \"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + programName() + "\" \"UninstallString\" " + "'\"$INSTDIR\\${UNINSTALL}$\\\"'\n" );
    *out << QString( "WriteRegDWORD HKLM \"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + programName() + "\" \"NoModify\"" + " 1\n" );
    *out << QString( "WriteRegDWORD HKLM \"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + programName() + "\" \"NoRepair\"" + " 1\n" );
    *out << QString( "WriteUninstaller \"$INSTDIR\\${UNINSTALL}\"\n\n" );

    if ( m_autoOpenProgram )
        *out << QString( "WriteRegStr HKCU \"Software\\Microsoft\\Windows\\CurrentVersion\\Run\\${NAME}\" \"${NAME}\" \"$INSTDIR\\${SHORTCUT}.exe\"\n\n" );

    if ( m_highestAuthority )
    {
        *out << QString( "WriteRegStr HKCU \"Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers\" \"$INSTDIR\\${NAME}.exe\" \"~ RUNASADMIN\"\n\n" );
        *out << QString( "WriteRegStr HKLM \"Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers\" \"$INSTDIR\\${NAME}.exe\" \"~ RUNASADMIN\"\n\n" );
    }
    *out << QString( "SetOutPath $INSTDIR\n" );
    for ( int i = 0; i < howManyDrivers(); i++ )
    {
        if ( !( m_driverX64List[ i ] == "" && m_driverX86List[ i ] == "" ) )
        {
            if ( m_driverX64List[ i ] != "" && m_driverX86List[ i ] != "" )
            {
                *out << QString( "RMDir /r $INSTDIR\\DRIVER_" + QString::number( i ) + "_X64\n" );
                *out << QString( "RMDir /r $INSTDIR\\DRIVER_" + QString::number( i ) + "_X86\n" );
            }
            else
            {
                *out << QString( "RMDir /r $INSTDIR\\DRIVER_" + QString::number( i ) + "\n" );
            }
        }
    }
    *out << QString( "Delete \"$INSTDIR\\${MAIN_FILE}\"\n\n" );

    if ( m_askAutoReboot )
    {
        *out << QString( "MessageBox MB_YESNO|MB_ICONQUESTION|MB_TOPMOST \"Please reboot computer\" IDNO noreboot\n" );
        *out << QString( "Reboot\n" );
        *out << QString( "noreboot:\n\n" );
    }
    *out << QString( "SectionEnd\n\n" );

    return true;
}

bool Generator::stepUninstall( QTextStream *out )
{
    *out << QString( "Section \"Uninstall\"\n\n" );
    QString drivers_Inf, x64_driver, x86_driver;
    for ( int i = 0; i < howManyDrivers(); i++ )
    {
        if ( !( m_driverX64List[ i ] == "" && m_driverX86List[ i ] == "" ) )
        {
            if ( m_driverX64List[ i ] != "" && m_driverX86List[ i ] != "" )
            {
                x64_driver = m_driverX64List[ i ].split( ";" )[ 0 ];
                x86_driver = m_driverX86List[ i ].split( ";" )[ 0 ];
                *out << QString( "${If} ${RunningX64}\n" );
                *out << diverInstallStr( "DRIVER_" + QString::number( i ) + "_X64", m_driverX64List[ i ].split( ";" )[ 1 ], false, m_usesWin7Installer[ i ] );
                *out << QString( "${Else}\n" );
                *out << diverInstallStr( "DRIVER_" + QString::number( i ) + "_X86", m_driverX64List[ i ].split( ";" )[ 1 ], false, m_usesWin7Installer[ i ] );
                *out << QString( "${EndIf}\n\n" );
            }
            else
            {
                if ( m_driverX64List[ i ] != "" )
                    *out << diverInstallStr( "DRIVER_" + QString::number( i ), m_driverX64List[ i ].split( ";" )[ 1 ], false, m_usesWin7Installer[ i ] );
                else
                    *out << diverInstallStr( "DRIVER_" + QString::number( i ), m_driverX86List[ i ].split( ";" )[ 1 ], false, m_usesWin7Installer[ i ] );
            }
        }
    }

    *out << QString( "SetOutPath $PROGRAMFILES\n" );
    *out << QString( "RMDir /r $INSTDIR\n" );
    *out << QString( "Delete \"$DESKTOP\\${SHORTCUT}.lnk\"\n" );
    *out << QString( "DeleteRegKey HKLM \"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + programName() + "\"\n\n" );
    if ( m_autoOpenProgram )
        *out << QString( "DeleteRegKey HKCU \"Software\\Microsoft\\Windows\\CurrentVersion\\Run\\${NAME}\"\n\n" );
    *out << QString( "SectionEnd\n" );

    return true;
}

QString Generator::diverInstallStr( QString diverName, QString infs, bool install, bool usesWin7 )
{
    QString returnStr, infStr = "";
    QList<QString> infList;
    returnStr = "SetOutPath \"$INSTDIR\\" + diverName + "\"\n";
    returnStr = returnStr + "File \"${" + diverName + "}\"\n";
    returnStr = returnStr + "ZipDLL::extractall \"$INSTDIR\\" + diverName + "\\${" + diverName + "}\" " + "\"$INSTDIR\\" + diverName + "\"\n";

    //    if ( infs.contains( "," ) > 0 )
    //    {
    //        infList = infs.split( "," );
    //        for ( int i = 0; i < infList.size(); i++ )
    //        {
    //            if ( infStr == "" )
    //                infStr = infList[ i ] + ".inf";
    //            else
    //                infStr = infStr + "," + infList[ i ] + ".inf";
    //        }
    //    }
    //    else
    //        infStr = infs + ".inf";

    if ( usesWin7 )
    {
        if ( install )
            returnStr = returnStr + "ExecWait \"win7DriverInstall.exe\"\n\n";
        else
        {
            if ( infs.contains( "," ) > 0 )
            {
                infList = infs.split( "," );
                for ( int i = 0; i < infList.size(); i++ )
                {
                    returnStr = returnStr + "ExecWait \"win7DriverInstall.exe /S /U " + infList[ i ] + "\"\n\n";
                }
            }
            else
                returnStr = returnStr + "ExecWait \"win7DriverInstall.exe /S /U " + infs + "\"\n\n";
        }
    }
    else
    {
        if ( infs.contains( ".exe" ) > 0 )
        {
            if ( install )
                returnStr = returnStr + "ExecWait \"" + infs + "\"\n\n";
            else
                returnStr = returnStr + "ExecWait \"" + infs + "\"\n\n";
        }
        else
        {
            if ( install )
                returnStr = returnStr + "ExecWait \"" + infs + ",DefaultInstall\"\n\n";
            else
                returnStr = returnStr + "ExecWait \"" + infs + ",DefaultunInstall\"\n\n";
        }
    }
    return returnStr;
}

void Generator::SetQListSize( int size )
{
    QList<QString> defList;
    QList<bool> defusesWin7;
    defList.clear();
    defusesWin7.clear();
    m_driverX64List.clear();
    m_driverX86List.clear();
    m_usesWin7Installer.clear();
    for ( int i = 0; i < size; i++ )
    {
        defList.append( "" );
        defusesWin7.append( false );
    }
    setDriverX64List( defList );
    setDriverX86List( defList );
    setUsesWin7Installer( defusesWin7 );
}

QString Generator::programName()
{
    return m_programName;
}

void Generator::setProgramName( QString programName )
{
    if ( m_programName == programName )
        return;
    m_programName = programName;
    emit programNameChanged();
}

QString Generator::version()
{
    return m_version;
}

void Generator::setVersion( QString version )
{
    if ( m_version == version )
        return;
    m_version = version;
    emit versionChanged();
}

QString Generator::mainFile()
{
    return m_mainFile;
}

void Generator::setMainFile( QString mainFile )
{
    if ( m_mainFile == mainFile )
        return;
    m_mainFile = mainFile;
    emit mainFileChanged();
}

int Generator::howManyDrivers()
{
    return m_howManyDrivers;
}

void Generator::setHowManyDrivers( int howManyDrivers )
{
    if ( m_howManyDrivers == howManyDrivers )
        return;
    m_howManyDrivers = howManyDrivers;
    SetQListSize( m_howManyDrivers );
    emit howManyDriversChanged();
}

QList<QString> Generator::driverX64List()
{
    return m_driverX64List;
}

void Generator::setDriverX64List( QList<QString> driverX64List )
{
    if ( m_driverX64List == driverX64List )
        return;
    m_driverX64List = driverX64List;
    emit driverX64ListChanged();
}

QList<QString> Generator::driverX86List()
{
    return m_driverX86List;
}

void Generator::setDriverX86List( QList<QString> driverX86List )
{
    if ( m_driverX86List == driverX86List )
        return;
    m_driverX86List = driverX86List;
    emit driverX86ListChanged();
}

QList<bool> Generator::usesWin7Installer()
{
    return m_usesWin7Installer;
}

void Generator::setUsesWin7Installer( QList<bool> usesWin7Installer )
{
    if ( m_usesWin7Installer == usesWin7Installer )
        return;
    m_usesWin7Installer = usesWin7Installer;
    emit usesWin7InstallerChanged();
}

QString Generator::company()
{
    return m_company;
}

void Generator::setCompany( QString company )
{
    if ( m_company == company )
        return;
    m_company = company;
    emit companyChanged();
}

QString Generator::iconName()
{
    return m_iconName;
}

void Generator::setIconName( QString iconName )
{
    if ( m_iconName == iconName )
        return;
    m_iconName = iconName;
    emit iconNameChanged();
}

void Generator::setDBProgramName( QString value )
{
    param.setName( "ProgramName" );
    param.setValue( value );

    DB::Inst()->setToNSISParams( QVariant::fromValue<SystemParamItem *>( &param ) );
}

void Generator::setDBIcon( QString value )
{
    param.setName( "Icon" );
    param.setValue( value );

    DB::Inst()->setToNSISParams( QVariant::fromValue<SystemParamItem *>( &param ) );
}

void Generator::setDBCompany( QString value )
{
    param.setName( "Company" );
    param.setValue( value );

    DB::Inst()->setToNSISParams( QVariant::fromValue<SystemParamItem *>( &param ) );
}

void Generator::setDBProgramReleaseFile( QString value )
{
    param.setName( "ProgramReleaseFile" );
    param.setValue( value );

    DB::Inst()->setToNSISParams( QVariant::fromValue<SystemParamItem *>( &param ) );
}

void Generator::setDBVersion( QString value )
{
    param.setName( "Version" );
    param.setValue( value );

    DB::Inst()->setToNSISParams( QVariant::fromValue<SystemParamItem *>( &param ) );
}
