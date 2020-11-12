#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "../TBLog/tblog.h"
#include "mainwindow.h"
#include "models/SystemParamItem.h"
#include "models/db.h"
#include "models/sharedinfo.h"
#include "viewmodels/generator.h"
#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QLockFile>
#include <QMessageBox>
#include <QQmlContext>
#include <QQuickView>
#include <QThreadPool>
#include <windows.h>

bool checkUnique()
{
    // 創建 handleMutex 並設指定名稱
    HANDLE m_hMutex = CreateMutex( nullptr, FALSE, L"TeddyBearFP" );
    // 檢查錯誤代碼
    if ( GetLastError() == ERROR_ALREADY_EXISTS )
    {
        // 如果已有mutex存在則釋放 handle Mutex 並初始handle Mutex 指標位址
        CloseHandle( m_hMutex );
        m_hMutex = nullptr;
        // 程式退出
        return false;
    }
    else
        return true;
}

int main( int argc, char *argv[] )
{
    QCoreApplication::setAttribute( Qt::AA_EnableHighDpiScaling );

    QGuiApplication app( argc, argv );

    app.setWindowIcon( QIcon( ":/icons/tbICON3_128x128.ico" ) );
    qmlRegisterType<Generator>( "Generator", 1, 0, "Generator" );
    //設定字體
    QFontDatabase::addApplicationFont( ":/fonts/Roboto-Regular.ttf" );
    app.setFont( QFont( "Roboto" ) );

    QString logPath = "C:/ProgramData/GCLPrograms/NSISGenerator";
    QDir dir( logPath );
    if ( !dir.exists( logPath ) )
        dir.mkpath( logPath );
    SetLogPath( (char *) qPrintable( logPath ) );
    SetLogLevel( LOGINFO );
    Logger( LOGINFO, "***** GCL NSIS Generator Started *****" );

    QThreadPool::globalInstance()->setMaxThreadCount( 20 );

    MainWindow mainWindow;
    mainWindow.rootContext()->setContextProperty( "sharedInfo", SHAREDINFO );
    //Take the app path as a property
    mainWindow.rootContext()->setContextProperty( "applicationDirPath", QGuiApplication::applicationDirPath() );

    //防止再度開啟程式
    if ( !checkUnique() ) return 0; // 離開程式

    mainWindow.load( QUrl( QStringLiteral( "qrc:/main.qml" ) ) );

    if ( mainWindow.rootObjects().isEmpty() )
    {
        Logger( LOGINFO, "***** GCL NSIS Generator Ended abnormally *****" );
        return -1;
    }
    int ret = app.exec();

    Logger( LOGINFO, "***** GCL NSIS Generator Ended *****" );
    return ret;
}
