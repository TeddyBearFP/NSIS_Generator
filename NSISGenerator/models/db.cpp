#include "db.h"
#include <QFile>

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QThread>
#include <QtWidgets/QApplication>

//#include "models/tbtranslator.h"
#include "SystemParamItem.h"

#include "sharedinfo.h"
#include "tblog.h"

#define DATABASE_NAME "TeddyBearDB.db"
#define DATABASE_HOSTNAME "TeddyBearDB"

DB DB::inst;

DB::DB( QObject *parent )
    : QObject( parent )
{
}

DB::~DB()
{
}

bool DB::connectToDataBase()
{
    dbPath = "C:/ProgramData/GCLPrograms/NSISGenerator";

    if ( !QFileInfo::exists( dbPath + "//" + DATABASE_NAME ) )
    {
        Logger( LOGWARN, "DB::connectToDataBase Database not found. Try to restore it now." );
        return this->restoreDatabase();
    }
    else
        return this->openDatabase();
}

bool DB::openDatabase()
{
    db = QSqlDatabase::addDatabase( "QSQLITE" );
    qDebug() << QSqlDatabase::drivers();
    db.setHostName( DATABASE_HOSTNAME );
    db.setDatabaseName( dbPath + "//" + DATABASE_NAME );
    if ( db.open() )
    {
        sysParams = getParamsFromSystemParams();
        parameters = getAllFromSystemParams();
        SharedInfo::Inst()->setSysParams( sysParams );
        if ( !sysParams.isEmpty() )
        {
            checkUpdateDatabase();
        }
        return true;
    }
    else
    {
        Logger( LOGERROR, "DB::openDatabase Cannot open database." );
        return false;
    }
}

bool DB::checkUpdateDatabase()
{
    QSqlQuery query;
    QString fileName;
    sysParams = getParamsFromSystemParams();
    if ( !sysParams.contains( "DB_Version" ) )
    {
        fileName = ":database/V1_0.sql";
        if ( QFileInfo::exists( fileName ) )
        {
            QFile file( fileName );
            if ( file.open( QIODevice::ReadOnly ) )
            {
                QTextStream in( &file );
                while ( !in.atEnd() )
                {
                    QString line = in.readLine();
                    query.exec( line );
                }
                file.close();
            }
            else
            {
                Logger( LOGERROR, "DB::checkUpdateDatabase Cannot open schema" );
                return false;
            }
        }
        else
        {
            Logger( LOGERROR, "DB::checkUpdateDatabase schema not found" );
            return false;
        }
        sysParams = getParamsFromSystemParams();
        parameters = getAllFromSystemParams();
        SharedInfo::Inst()->setSysParams( sysParams );
    }
    int index = 0;
    QStringList versionList = { "1.0", "1.1", "1.2", "1.3", "1.4", "1.5", "1.6" };
    for ( int j = 0; j < versionList.size(); ++j )
    {
        if ( sysParams[ "DB_Version" ].toString() == versionList.at( j ) )
            index = j; //get current version index
    }

    while ( sysParams[ "DB_Version" ].toString() != versionList.at( versionList.length() - 1 ) )
    {
        index++;
        QString version = versionList.at( index );
        version = version.split( "." ).join( "_" );
        qDebug() << "version=" << version;
        fileName = ":database/V" + version + ".sql";
        if ( QFileInfo::exists( fileName ) )
        {
            QFile file( fileName );
            if ( file.open( QIODevice::ReadOnly ) )
            {
                QTextStream in( &file );
                while ( !in.atEnd() )
                {
                    QString line = in.readLine();
                    query.exec( line );
                }
                file.close();
            }
            else
            {
                Logger( LOGERROR, "DB::checkUpdateDatabase Cannot open schema" );
                return false;
            }
        }
        else
        {
            Logger( LOGERROR, "DB::checkUpdateDatabase schema not found" );
            return false;
        }
        sysParams = getParamsFromSystemParams();
        parameters = getAllFromSystemParams();
        SharedInfo::Inst()->setSysParams( sysParams );
    }
    return true;
}

bool DB::restoreDatabase()
{
    if ( this->openDatabase() )
    {
        if ( this->createTables() )
        {
            sysParams = getParamsFromSystemParams();
            SharedInfo::Inst()->setSysParams( sysParams );
            checkUpdateDatabase();
            Logger( LOGINFO, "DB::restoreDatabase Database has been created." );
            return true;
        }
        else
        {
            Logger( LOGERROR, "DB::restoreDatabase Cannot create database." );
            return false;
        }
    }
    else
    {
        Logger( LOGERROR, "DB::restoreDatabase Cannot restore database." );
        return false;
    }
}

void DB::closeDataBase()
{
    db.close();
}

QVariantList DB::getAllFromSystemParams()
{
    QSqlQuery query;
    QVariantList paramList;
    query.prepare( "SELECT * FROM SystemParams" );
    if ( !query.exec() )
        Logger( LOGERROR, "DB::getAllFromSystemParams Cannot get system parameters because %s", qPrintable( query.lastError().text() ) );
    while ( query.next() )
    {
        SystemParamItem *param = new SystemParamItem();

        //Move to Main thread if the current thread is not main thread
        if ( QThread::currentThread() != QApplication::instance()->thread() )
            param->moveToThread( QApplication::instance()->thread() );
        param->setName( query.value( "Name" ).toString() );
        param->setValue( query.value( "Value" ).toString() );
        paramList.append( QVariant::fromValue( param ) );
    }

    return paramList;
}

QMap<QString, QVariant> DB::getParamsFromSystemParams()
{
    QSqlQuery query;
    QMap<QString, QVariant> paramMap;
    query.prepare( "SELECT * FROM SystemParams" );
    if ( !query.exec() )
        Logger( LOGERROR, "DB::getParamsFromSystemParams Cannot get system parameters because %s", qPrintable( query.lastError().text() ) );
    while ( query.next() )
        paramMap.insert( query.value( "Name" ).toString(), query.value( "Value" ).toString() );

    return paramMap;
}

QVariantList DB::getFromSystemParams( QString namePrefix )
{
    QSqlQuery query;
    QVariantList paramList;
    query.prepare( "SELECT * FROM SystemParams WHERE Name LIKE :NamePrefix" );
    query.bindValue( ":NamePrefix", namePrefix + "_%" );
    if ( !query.exec() )
        Logger( LOGERROR, "DB::getFromSystemParams Cannot get system parameters because %s", qPrintable( query.lastError().text() ) );
    while ( query.next() )
    {
        SystemParamItem *param = new SystemParamItem();
        param->setName( query.value( "Name" ).toString() );
        param->setValue( query.value( "Value" ).toString() );
        paramList.append( QVariant::fromValue( param ) );
    }
    return paramList;
}

QVariant DB::getSingleFromSystemParams( QString name )
{
    QSqlQuery query;
    query.prepare( "SELECT * FROM SystemParams WHERE Name = :Name" );
    query.bindValue( ":Name", name );
    if ( !query.exec() )
        Logger( LOGERROR, "DB::getSingleFromSystemParams Cannot get system parameter because %s", qPrintable( query.lastError().text() ) );

    if ( query.next() )
    {
        SystemParamItem *param = new SystemParamItem();
        param->setName( query.value( "Name" ).toString() );
        param->setValue( query.value( "Value" ).toString() );
        return QVariant::fromValue( param );
    }
    else
        return NULL;
}

bool DB::setToSystemParams( QVariant systemParam )
{
    QSqlQuery query;
    SystemParamItem *param = qvariant_cast<SystemParamItem *>( systemParam );
    query.prepare( "UPDATE SystemParams SET Value=:ParamValue WHERE Name = :ParamName" );
    query.bindValue( ":ParamName", param->name() );
    query.bindValue( ":ParamValue", param->value() );

    if ( !query.exec() )
    {
        Logger( LOGERROR, "DB::setToSystemParams Cannot set system parameter because %s", qPrintable( query.lastError().text() ) );
        return false;
    }
    else
    {
        sysParams[ param->name() ] = param->value();
        return true;
    }
}

QVariantList DB::getParameters() const
{
    return parameters;
}

void DB::setParameters( const QVariantList &value )
{
    parameters = value;
}

QVariantList DB::getAllFromNSISParams()
{
    QSqlQuery query;
    QVariantList paramList;
    query.prepare( "SELECT * FROM NSISParams" );
    if ( !query.exec() )
        Logger( LOGERROR, "DB::getAllFromNSISParams Cannot get system parameters because %s", qPrintable( query.lastError().text() ) );
    while ( query.next() )
    {
        SystemParamItem *param = new SystemParamItem();

        //Move to Main thread if the current thread is not main thread
        if ( QThread::currentThread() != QApplication::instance()->thread() )
            param->moveToThread( QApplication::instance()->thread() );
        param->setName( query.value( "Key" ).toString() );
        param->setValue( query.value( "Value" ).toString() );
        paramList.append( QVariant::fromValue( param ) );
    }

    return paramList;
}

QMap<QString, QVariant> DB::getParamsFromNSISParams()
{
    QSqlQuery query;
    QMap<QString, QVariant> paramMap;
    query.prepare( "SELECT * FROM NSISParams" );
    if ( !query.exec() )
        Logger( LOGERROR, "DB::getParamsFromNSISParams Cannot get system parameters because %s", qPrintable( query.lastError().text() ) );
    while ( query.next() )
        paramMap.insert( query.value( "Key" ).toString(), query.value( "Value" ).toString() );

    return paramMap;
}

QVariantList DB::getFromNSISParams( QString namePrefix )
{
    QSqlQuery query;
    QVariantList paramList;
    query.prepare( "SELECT * FROM NSISParams WHERE Name LIKE :NamePrefix" );
    query.bindValue( ":NamePrefix", namePrefix + "_%" );
    if ( !query.exec() )
        Logger( LOGERROR, "DB::getFromNSISParams Cannot get system parameters because %s", qPrintable( query.lastError().text() ) );
    while ( query.next() )
    {
        SystemParamItem *param = new SystemParamItem();
        param->setName( query.value( "Key" ).toString() );
        param->setValue( query.value( "Value" ).toString() );
        paramList.append( QVariant::fromValue( param ) );
    }
    return paramList;
}

QVariant DB::getSingleFromNSISParams( QString name )
{
    QSqlQuery query;
    query.prepare( "SELECT * FROM NSISParams WHERE Name = :Name" );
    query.bindValue( ":Name", name );
    if ( !query.exec() )
        Logger( LOGERROR, "DB::getSingleFromNSISParams Cannot get system parameter because %s", qPrintable( query.lastError().text() ) );

    if ( query.next() )
    {
        SystemParamItem *param = new SystemParamItem();
        param->setName( query.value( "Key" ).toString() );
        param->setValue( query.value( "Value" ).toString() );
        return QVariant::fromValue( param );
    }
    else
        return NULL;
}

bool DB::setToNSISParams( QVariant systemParam )
{
    QSqlQuery query;
    SystemParamItem *param = qvariant_cast<SystemParamItem *>( systemParam );
    query.prepare( "UPDATE NSISParams SET Value=:ParamValue WHERE Key=:ParamName" );
    query.bindValue( ":ParamName", param->name() );
    query.bindValue( ":ParamValue", param->value() );

    if ( !query.exec() )
    {
        Logger( LOGERROR, "DB::setToNSISParams Cannot set NSIS parameter because %s", qPrintable( query.lastError().text() ) );
        return false;
    }
    else
    {
        sysParams[ param->name() ] = param->value();
        return true;
    }
}

bool DB::createTables()
{
    QSqlQuery query;
    QString fileName = ":database/V0_0.sql";
    if ( QFileInfo::exists( fileName ) )
    {
        QFile file( fileName );
        if ( file.open( QIODevice::ReadOnly ) )
        {
            QTextStream in( &file );
            while ( !in.atEnd() )
            {
                QString line = in.readLine();
                query.exec( line );
            }
            file.close();
        }
        else
        {
            Logger( LOGERROR, "DB::createTables Cannot open schema" );
            return false;
        }
    }
    else
    {
        Logger( LOGERROR, "DB::createTables schema not found" );
        return false;
    }
    return true;
}
