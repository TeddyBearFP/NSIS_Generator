#include "sharedinfo.h"

SharedInfo SharedInfo::inst;

SharedInfo::SharedInfo( QObject *parent )
    : QObject( parent )
{
}

SharedInfo::~SharedInfo()
{
}

QMap<QString, QVariant> SharedInfo::getSysParams() const
{
    return sysParams;
}

void SharedInfo::setSysParams( const QMap<QString, QVariant> &value )
{
    sysParams = value;
}
