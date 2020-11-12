#ifndef GENERATOR_H
#define GENERATOR_H

#include "models/SystemParamItem.h"
#include <QList>
#include <QQuickItem>

class Generator : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY( QString programName READ programName WRITE setProgramName NOTIFY programNameChanged )
    Q_PROPERTY( QString version READ version WRITE setVersion NOTIFY versionChanged )
    Q_PROPERTY( QString mainFile READ mainFile WRITE setMainFile NOTIFY mainFileChanged )
    Q_PROPERTY( QString company READ company WRITE setCompany NOTIFY companyChanged )
    Q_PROPERTY( QString iconName READ iconName WRITE setIconName NOTIFY iconNameChanged )
    Q_PROPERTY( int howManyDrivers READ howManyDrivers WRITE setHowManyDrivers NOTIFY howManyDriversChanged )
    Q_PROPERTY( QList<QString> driverX64List READ driverX64List WRITE setDriverX64List NOTIFY driverX64ListChanged )
    Q_PROPERTY( QList<QString> driverX86List READ driverX86List WRITE setDriverX86List NOTIFY driverX86ListChanged )
    Q_PROPERTY( QList<bool> usesWin7Installer READ usesWin7Installer WRITE setUsesWin7Installer NOTIFY usesWin7InstallerChanged )

public:
    Generator();
    ~Generator();

    bool m_welcomePage, m_directoryPage, m_instfilesPage, m_finishPage, m_vcredist_x86;
    bool m_autoOpenProgram, m_askAutoReboot, m_highestAuthority;

    Q_INVOKABLE void setNsisPage( int page, bool checked );
    Q_INVOKABLE QString makeNsis();

    bool step0( QTextStream *out );
    bool stepInstall( QTextStream *out );
    bool stepUninstall( QTextStream *out );
    QString diverInstallStr( QString diverName, QString infs, bool install, bool usesWin7 );

    void SetQListSize( int size );

    //====Q_PROPERTY====
    QString m_programName;
    QString programName();
    void setProgramName( QString programName );

    QString m_version;
    QString version();
    void setVersion( QString version );

    QString m_mainFile;
    QString mainFile();
    void setMainFile( QString mainFile );

    int m_howManyDrivers;
    int howManyDrivers();
    void setHowManyDrivers( int howManyDrivers );

    QList<QString> m_driverX64List;
    QList<QString> driverX64List();
    void setDriverX64List( QList<QString> driverX64List );

    QList<QString> m_driverX86List;
    QList<QString> driverX86List();
    void setDriverX86List( QList<QString> driverX86List );

    QList<bool> m_usesWin7Installer;
    QList<bool> usesWin7Installer();
    void setUsesWin7Installer( QList<bool> usesWin7Installer );

    QString m_company;
    QString company();
    void setCompany( QString company );

    QString m_iconName;
    QString iconName();
    void setIconName( QString iconName );
    //====Q_PROPERTY====

    Q_INVOKABLE void setDBProgramName( QString value );
    Q_INVOKABLE void setDBIcon( QString value );
    Q_INVOKABLE void setDBCompany( QString value );
    Q_INVOKABLE void setDBProgramReleaseFile( QString value );
    Q_INVOKABLE void setDBVersion( QString value );

private:
    SystemParamItem param;
signals:
    void programNameChanged();
    void versionChanged();
    void mainFileChanged();
    void companyChanged();
    void iconNameChanged();
    void howManyDriversChanged();
    void driverX64ListChanged();
    void driverX86ListChanged();
    void usesWin7InstallerChanged();
};

#endif // GENERATOR_H
