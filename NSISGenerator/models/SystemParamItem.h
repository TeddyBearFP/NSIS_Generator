#ifndef SYSTEMPARAMITEM_H
#define SYSTEMPARAMITEM_H

#include <QMetaType>
#include <QObject>

class SystemParamItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged )
    Q_PROPERTY( QString value READ value WRITE setValue NOTIFY valueChanged )

public:
    SystemParamItem()
    {
    }
    SystemParamItem( const QString &name, const QString &value, QObject *parent = 0 )
        : QObject( parent )
        , m_name( name )
        , m_value( value )
    {
    }

    QString name() const { return m_name; }
    QString value() const
    {
        return m_value;
    }

public slots:
    void setName( QString name )
    {
        if ( m_name == name )
            return;

        m_name = name;
        emit nameChanged( m_name );
    }

    void setValue( QString value )
    {
        if ( m_value == value )
            return;

        m_value = value;
        emit valueChanged( m_value );
    }

private:
    QString m_name;
    QString m_value;

signals:
    void nameChanged( QString name );
    void valueChanged( QString value );
};
#endif // SYSTEMPARAMITEM_H
