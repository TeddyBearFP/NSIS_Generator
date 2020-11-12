#ifndef DB_H
#define DB_H

#include <QDebug>
#include <QFile>
#include <QMap>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtWidgets/QApplication>

class DB : public QObject
{
    Q_OBJECT
public:
    static DB *Inst() { return &inst; }
    explicit DB( QObject *parent = 0 );
    ~DB();
    bool connectToDataBase();
    void closeDataBase();

    //access db systemparams table
    QVariantList getAllFromSystemParams();
    QMap<QString, QVariant> getParamsFromSystemParams();
    QVariantList getFromSystemParams( QString namePrefix );
    QVariant getSingleFromSystemParams( QString name );
    bool setToSystemParams( QVariant systemParam );

    //access db nsisparams table
    QVariantList getAllFromNSISParams();
    QMap<QString, QVariant> getParamsFromNSISParams();
    QVariantList getFromNSISParams( QString namePrefix );
    QVariant getSingleFromNSISParams( QString name );
    bool setToNSISParams( QVariant systemParam );

    QVariantList getParameters() const;
    void setParameters( const QVariantList &value );

private:
    static DB inst;
    QSqlDatabase db, db2;
    QString dbPath, dbPath2;
    QVariantList parameters;
    QMap<QString, QVariant> sysParams;

    bool openDatabase();
    bool checkUpdateDatabase();
    bool restoreDatabase();
    bool createTables();
};

#endif // DB_H
