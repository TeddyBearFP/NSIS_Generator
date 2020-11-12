#ifndef SHAREDINFO_H
#define SHAREDINFO_H

#include <QFuture> //data source
#include <QImage>
#include <QObject>

#define SHAREDINFO SharedInfo::Inst()

class SharedInfo : public QObject
{
    Q_OBJECT
public:
    explicit SharedInfo( QObject *parent = nullptr );
    ~SharedInfo();
    static SharedInfo *Inst() { return &inst; }

    QMap<QString, QVariant> getSysParams() const;
    void setSysParams( const QMap<QString, QVariant> &value );

signals:

private:
    static SharedInfo inst;
    QMap<QString, QVariant> sysParams;

public slots:
};

#endif // SHAREDINFO_H
